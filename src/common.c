#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webview/webview.h"
#include "common.h"

#ifdef _WIN32
#include <windows.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

int system_check_admin_privileges(void)
{
    BOOL is_admin = FALSE;
    SID_IDENTIFIER_AUTHORITY nt_authority = SECURITY_NT_AUTHORITY;
    PSID administrators_group;

    if (AllocateAndInitializeSid(
            &nt_authority, 2, SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &administrators_group))
    {
        CheckTokenMembership(NULL, administrators_group, &is_admin);
        FreeSid(administrators_group);
    }

    return is_admin;
}

void system_execute_command_elevated(const char *command)
{
    SHELLEXECUTEINFO shell_exec_info = {sizeof(SHELLEXECUTEINFO)};
    shell_exec_info.fMask = SEE_MASK_NOCLOSEPROCESS;
    shell_exec_info.lpVerb = "runas";
    shell_exec_info.lpFile = "cmd.exe";
    shell_exec_info.lpParameters = command;
    shell_exec_info.nShow = SW_HIDE;

    if (ShellExecuteEx(&shell_exec_info))
    {
        WaitForSingleObject(shell_exec_info.hProcess, INFINITE);
        CloseHandle(shell_exec_info.hProcess);
    }
    else
    {
        DWORD error = GetLastError();
        LPVOID message_buffer;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                      NULL, error, 0, (LPSTR)&message_buffer, 0, NULL);
        MessageBox(NULL, (LPCSTR)message_buffer, "Error", MB_OK | MB_ICONERROR);
        LocalFree(message_buffer);
    }
}

int system_download_file(const char *url, const char *destination)
{
    if (FAILED(URLDownloadToFileA(NULL, url, destination, 0, NULL)))
    {
        MessageBoxA(NULL, "Download failed", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    MessageBoxA(NULL, "Download successful", "Info", MB_OK | MB_ICONINFORMATION);
    return 0;
}

int system_extract_tar_archive(const char *tar_file, const char *target_dir)
{
    char command[512];
    snprintf(command, sizeof(command), "/c tar -xf %s -C %s", tar_file, target_dir);
    system_execute_command_elevated(command);
    return 0;
}
#else
#include <unistd.h>
#endif

void webview_quit_callback(const char *seq, const char *req, void *arg)
{
    (void)seq;
    (void)req;
    webview_t w = (webview_t)arg;
    webview_terminate(w);
}

void webview_open_browser_callback(const char *seq, const char *req, void *arg)
{
    (void)seq;
    app_context_t *context = (app_context_t *)arg;

    if (req && strlen(req) > 2)
    {
        // Remove first and last character (quotes)
        size_t url_len = strlen(req) - 2;
        char *url = (char *)malloc(url_len + 1);

        // Copy URL and null terminate
        strncpy(url, req + 1, url_len);
        url[url_len] = '\0';

        // Allocate space for command using longest one
        size_t command_len = strlen("xdg-open ") + strlen(url) + 1;
        char *command = (char *)malloc(command_len);

#ifdef _WIN32
        snprintf(command, command_len, "/c start %s", url);
        system_execute_command_elevated(command);
#elif __APPLE__
        snprintf(command, command_len, "open %s", url);
        system(command);
#elif __linux__
        snprintf(command, command_len, "xdg-open %s", url);
        system(command);
#else
        printf("Unsupported OS\n");
#endif

        // Free the allocated memory
        free(url);
        free(command);
    }
}

#ifdef _WIN32
int system_check_internet_connectivity(void)
{
    DWORD flags;
    return InternetGetConnectedState(&flags, 0) ? 1 : 0;
}

int system_patch_gatherosstate_file(const char *input_file, const char *output_file)
{
    // Patch definitions - offsets and new bytes from decoded PowerShell
    typedef struct
    {
        long offset;
        unsigned char new_byte;
    } patch_entry_t;

    patch_entry_t patches[] = {
        {0x16A98, 0xB8}, // mov eax, 1
        {0x16A99, 0x01},
        {0x16A9A, 0x00},
        {0x16A9B, 0x00},
        {0x16A9C, 0x00},
        {0x16A9D, 0xC3}, // ret
        {0x13D9A, 0x48}, // lea rax, [rip+0x2c6f]
        {0x13D9B, 0x8D},
        {0x13D9C, 0x05},
        {0x13D9D, 0x6F},
        {0x13D9E, 0x2C},
        {0x13D9F, 0x00},
        {0x13DA0, 0x00},
        {0x13DA1, 0xC3}, // ret
        {0x14FA0, 0x48}, // xor eax, eax; ret
        {0x14FA1, 0x31},
        {0x14FA2, 0xC0},
        {0x14FA3, 0xC3}};

    FILE *input = fopen(input_file, "rb");
    if (!input)
    {
        MessageBoxA(NULL, "Failed to open input file for reading", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Get file size
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);

    // Read entire file into memory
    unsigned char *file_data = malloc(file_size);
    if (!file_data)
    {
        MessageBoxA(NULL, "Failed to allocate memory for file data", "Error", MB_OK | MB_ICONERROR);
        fclose(input);
        return 1;
    }

    size_t bytes_read = fread(file_data, 1, file_size, input);
    fclose(input);

    if (bytes_read != file_size)
    {
        MessageBoxA(NULL, "Failed to read complete file", "Error", MB_OK | MB_ICONERROR);
        free(file_data);
        return 1;
    }

    // Apply patches
    int patches_count = sizeof(patches) / sizeof(patches[0]);
    for (int i = 0; i < patches_count; i++)
    {
        if (patches[i].offset < file_size)
        {
            file_data[patches[i].offset] = patches[i].new_byte;
        }
    }

    // Write patched file
    FILE *output = fopen(output_file, "wb");
    if (!output)
    {
        MessageBoxA(NULL, "Failed to open output file for writing", "Error", MB_OK | MB_ICONERROR);
        free(file_data);
        return 1;
    }

    size_t bytes_written = fwrite(file_data, 1, file_size, output);
    fclose(output);
    free(file_data);

    if (bytes_written != file_size)
    {
        MessageBoxA(NULL, "Failed to write complete patched file", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    return 0;
}

int system_delete_directory_recursive(const char *directory_path)
{
    WIN32_FIND_DATAA find_data;
    HANDLE find_handle;
    char search_path[MAX_PATH];
    char file_path[MAX_PATH];

    // Create search pattern
    snprintf(search_path, sizeof(search_path), "%s\\*", directory_path);

    find_handle = FindFirstFileA(search_path, &find_data);
    if (find_handle == INVALID_HANDLE_VALUE)
    {
        return 1; // Directory doesn't exist or can't be accessed
    }

    do
    {
        // Skip current and parent directory entries
        if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0)
        {
            continue;
        }

        // Build full path
        snprintf(file_path, sizeof(file_path), "%s\\%s", directory_path, find_data.cFileName);

        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // Recursively delete subdirectory
            system_delete_directory_recursive(file_path);
            RemoveDirectoryA(file_path);
        }
        else
        {
            // Delete file
            SetFileAttributesA(file_path, FILE_ATTRIBUTE_NORMAL);
            DeleteFileA(file_path);
        }
    } while (FindNextFileA(find_handle, &find_data) != 0);

    FindClose(find_handle);

    // Remove the directory itself
    return RemoveDirectoryA(directory_path) ? 0 : 1;
}
#else
int system_check_internet_connectivity(void)
{
    // Stub for non-Windows platforms
    return 1;
}

int system_patch_gatherosstate_file(const char *input_file, const char *output_file)
{
    // Stub for non-Windows platforms
    (void)input_file;
    (void)output_file;
    return 0;
}

int system_delete_directory_recursive(const char *directory_path)
{
    // Stub for non-Windows platforms
    (void)directory_path;
    return 0;
}
#endif