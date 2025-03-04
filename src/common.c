#include <stdio.h>
#include <stdlib.h>
#include "webview/webview.h"

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

void credits_cb(const char *seq, const char *req, void *arg)
{
    (void)seq;
    (void)req;
    (void)arg;

#ifdef _WIN32
    run_command("/c start https://www.phoenixthrush.com");
#elif __APPLE__
    system("open https://www.phoenixthrush.com");
#elif __linux__
    system("xdg-open https://www.phoenixthrush.com");
#else
    printf("Unsupported OS\n");
#endif
}

void quit_cb(const char *seq, const char *req, void *arg)
{
    (void)seq;
    (void)req;
    webview_t w = (webview_t)arg;
    webview_terminate(w);
}
