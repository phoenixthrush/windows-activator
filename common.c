#include <windows.h>

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

// TODO: return status on success or failure
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
}