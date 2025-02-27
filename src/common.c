#include <windows.h>
#include <shlobj.h>
#include <stdio.h>

int is_admin()
{
    BOOL isAdmin = FALSE;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;

    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &AdministratorsGroup))
    {
        CheckTokenMembership(NULL, AdministratorsGroup, &isAdmin);
        FreeSid(AdministratorsGroup);
    }

    return isAdmin;
}

int request_admin()
{
    if (!IsUserAnAdmin())
    {
        WCHAR szPath[MAX_PATH];
        GetModuleFileNameW(NULL, szPath, MAX_PATH);
        SHELLEXECUTEINFOW ShExInfo = {0};
        ShExInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
        ShExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        ShExInfo.hwnd = NULL;
        ShExInfo.lpVerb = L"runas";
        ShExInfo.lpFile = szPath;
        ShExInfo.lpParameters = NULL;
        ShExInfo.lpDirectory = NULL;
        ShExInfo.nShow = SW_SHOWNORMAL;
        ShellExecuteExW(&ShExInfo);
        return 0;
    }
    return 1;
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
        char errorMessage[512];
        snprintf(
            errorMessage, sizeof(errorMessage),
            "ShellExecuteEx failed with error %lu\nCommand: %s",
            dwError, command);
        MessageBox(NULL, errorMessage, "Error", MB_OK | MB_ICONERROR);
    }
}