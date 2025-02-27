#include <windows.h>
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

DWORD WINAPI RunCommandThread(LPVOID lpParam)
{
    const char *command = (const char *)lpParam;

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

    return 0;
}

void run_command(const char *command)
{
    HANDLE hThread = CreateThread(
        NULL,
        0,
        RunCommandThread,
        (LPVOID)command,
        0,
        NULL);

    if (hThread != NULL)
    {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    else
    {
        DWORD dwError = GetLastError();
        char errorMessage[512];
        snprintf(
            errorMessage, sizeof(errorMessage),
            "CreateThread failed with error %lu\nCommand: %s",
            dwError, command);
        MessageBox(NULL, errorMessage, "Error", MB_OK | MB_ICONERROR);
    }
}