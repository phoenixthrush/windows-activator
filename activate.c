// using HWID Activation from https://massgrave.dev/hwid

#include <windows.h>
#include <stdio.h>
typedef struct
{
    char *edition;
    char *editionKey;
    char *productKey;
} WindowsKey;

WindowsKey keys[] = {
    {"Education", "Education", "YNMGQ-8RYV3-4PGQ3-C8XTP-7CFBY"},
    {"Education N", "EducationN", "84NGF-MHBT6-FXBX8-QWJK7-DRR8H"},
    {"Enterprise", "Enterprise", "XGVPP-NMH47-7TTHJ-W3FW7-8HV2C"},
    {"Enterprise N", "EnterpriseN", "3V6Q6-NQXCX-V8YXR-9QCYV-QPFCT"},
    {"Enterprise LTSB 2015", "EnterpriseS", "FWN7H-PF93Q-4GGP8-M8RF3-MDWWW"},
    {"Enterprise LTSB 2016", "EnterpriseS", "NK96Y-D9CD8-W44CQ-R8YTK-DYJWX"},
    {"Enterprise LTSC 2019", "EnterpriseS", "43TBQ-NH92J-XKTM7-KT3KK-P39PB"},
    {"Enterprise N LTSB 2015", "EnterpriseSN", "NTX6B-BRYC2-K6786-F6MVQ-M7V2X"},
    {"Enterprise N LTSB 2016", "EnterpriseSN", "2DBW3-N2PJG-MVHW3-G7TDK-9HKR4"},
    {"Home", "Core", "YTMG3-N6DKC-DKB77-7M9GH-8HVX7"},
    {"Home N", "CoreN", "4CPRK-NM3K3-X6XXQ-RXX86-WXCHW"},
    {"Home China", "CoreCountrySpecific", "N2434-X9D7W-8PF6X-8DV9T-8TYMD"},
    {"Home Single Language", "CoreSingleLanguage", "BT79Q-G7N6G-PGBYW-4YWX6-6F4BT"},
    {"IoT Enterprise", "IoTEnterprise", "XQQYW-NFFMW-XJPBH-K8732-CKFFD"},
    {"IoT Enterprise Subscription", "IoTEnterpriseK", "P8Q7T-WNK7X-PMFXY-VXHBG-RRK69"},
    {"IoT Enterprise LTSC 2021", "IoTEnterpriseS", "QPM6N-7J2WJ-P88HH-P3YRH-YY74H"},
    {"IoT Enterprise LTSC 2024", "IoTEnterpriseS", "CGK42-GYN6Y-VD22B-BX98W-J8JXD"},
    {"IoT Enterprise LTSC Subscription 2024", "IoTEnterpriseSK", "N979K-XWD77-YW3GB-HBGH6-D32MH"},
    {"Pro", "Professional", "VK7JG-NPHTM-C97JM-9MPGT-3V66T"},
    {"Pro N", "ProfessionalN", "2B87N-8KFHP-DKV6R-Y2C8J-PKCKT"},
    {"Pro Education", "ProfessionalEducation", "8PTT6-RNW4C-6V7J2-C2D3X-MHBPB"},
    {"Pro Education N", "ProfessionalEducationN", "GJTYN-HDMQY-FRR76-HVGC7-QPF8P"},
    {"Pro for Workstations", "ProfessionalWorkstation", "DXG7C-N36C4-C4HTG-X4T3X-2YV77"},
    {"Pro N for Workstations", "ProfessionalWorkstationN", "WYPNQ-8C467-V2W6J-TX4WX-WT2RQ"},
    {"S", "Cloud", "V3WVW-N2PV2-CGWC3-34QGF-VMJ2C"},
    {"S N", "CloudN", "NH9J3-68WK7-6FB93-4K3DF-DJ4F6"},
    {"SE", "CloudEdition", "KY7PN-VR6RX-83W6Y-6DDYQ-T6R4W"},
    {"SE N", "CloudEditionN", "K9VKN-3BGWV-Y624W-MCRMQ-BHDCD"},
    {"Team", "PPIPro", "XKCNC-J26Q9-KFHD2-FKTHY-KD72Y"}};

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

char *get_edition_id()
{
    static char edition[256];
    HKEY hKey;
    DWORD size = sizeof(edition);

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueEx(hKey, "EditionID", NULL, NULL, (LPBYTE)edition, &size) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return edition;
        }
        RegCloseKey(hKey);
    }
    return NULL;
}

char *get_license_key(const char *edition_id)
{
    if (!edition_id)
    {
        return NULL;
    }

    for (size_t i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
    {
        if (strcmp(edition_id, keys[i].editionKey) == 0)
        {
            return keys[i].productKey;
        }
    }

    return NULL;
}

char *get_os_product_pfn()
{
    static char osProductPfn[256];
    HKEY hKey;
    DWORD size = sizeof(osProductPfn);

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueEx(hKey, "OSProductPfn", NULL, NULL, (LPBYTE)osProductPfn, &size) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return osProductPfn;
        }
        RegCloseKey(hKey);
    }
    return NULL;
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