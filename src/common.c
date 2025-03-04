#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webview/webview.h"

typedef struct
{
    webview_t w;
} context_t;

#ifdef _WIN32
#include <windows.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

int is_admin()
{
    BOOL isAdmin = FALSE;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;

    if (AllocateAndInitializeSid(
            &NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &AdministratorsGroup))
    {
        CheckTokenMembership(NULL, AdministratorsGroup, &isAdmin);
        FreeSid(AdministratorsGroup);
    }

    return isAdmin;
}

void run_command(const char *command)
{
    SHELLEXECUTEINFO shExecInfo = {sizeof(SHELLEXECUTEINFO)};
    shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shExecInfo.lpVerb = "runas";
    shExecInfo.lpFile = "cmd.exe";
    shExecInfo.lpParameters = command;
    shExecInfo.nShow = SW_HIDE;

    if (ShellExecuteEx(&shExecInfo))
    {
        WaitForSingleObject(shExecInfo.hProcess, INFINITE);
        CloseHandle(shExecInfo.hProcess);
    }
    else
    {
        DWORD dwError = GetLastError();
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                      NULL, dwError, 0, (LPSTR)&lpMsgBuf, 0, NULL);
        MessageBox(NULL, (LPCSTR)lpMsgBuf, "Error", MB_OK | MB_ICONERROR);
        LocalFree(lpMsgBuf);
    }
}

int download_file(const char *url, const char *destination)
{
    if (FAILED(URLDownloadToFileA(NULL, url, destination, 0, NULL)))
    {
        MessageBoxA(NULL, "Download failed", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    MessageBoxA(NULL, "Download successful", "Info", MB_OK | MB_ICONINFORMATION);
    return 0;
}

int extract_tar(const char *tar_file, const char *target_dir)
{
    char command[512];
    snprintf(command, sizeof(command), "/c tar -xf %s -C %s", tar_file, target_dir);
    run_command(command);
    return 0;
}
#else
#include <unistd.h>
#endif

void quit_cb(const char *seq, const char *req, void *arg)
{
    (void)seq;
    (void)req;
    webview_t w = (webview_t)arg;
    webview_terminate(w);
}

void open_browser_cb(const char *seq, const char *req, void *arg)
{
    (void)seq;
    context_t *context = (context_t *)arg;

    if (req && strlen(req) > 2)
    {
        // remove first and last character
        size_t url_len = strlen(req) - 2;
        char *url = (char *)malloc(url_len + 1);

        // null terminate
        strncpy(url, req + 1, url_len);
        url[url_len] = '\0';

        // alllocate space for command using longest one
        size_t command_len = strlen("xdg-open ") + strlen(url) + 1;
        char *command = (char *)malloc(command_len);

#ifdef _WIN32
        snprintf(command, command_len, "/c start %s", url);
        run_command(command);
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