#include <windows.h>
#include <stdio.h>
#include "webview/webview.h"
#include "common.h"
#include "hwid.h"

// Windows license keys database
static const windows_license_key_t windows_license_keys[] = {
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

static const size_t windows_license_keys_count = sizeof(windows_license_keys) / sizeof(windows_license_keys[0]);

char *windows_get_edition_id(void)
{
    static char edition[256];
    HKEY registry_key;
    DWORD size = sizeof(edition);

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &registry_key) == ERROR_SUCCESS)
    {
        if (RegQueryValueEx(registry_key, "EditionID", NULL, NULL, (LPBYTE)edition, &size) == ERROR_SUCCESS)
        {
            RegCloseKey(registry_key);
            return edition;
        }
        RegCloseKey(registry_key);
    }
    return NULL;
}

const char *windows_get_license_key_for_edition(const char *edition_id)
{
    if (!edition_id)
    {
        return NULL;
    }

    for (size_t i = 0; i < windows_license_keys_count; i++)
    {
        if (strcmp(edition_id, windows_license_keys[i].edition_id) == 0)
        {
            return windows_license_keys[i].product_key;
        }
    }

    return NULL;
}

char *windows_get_os_product_pfn(void)
{
    static char os_product_pfn[256];
    HKEY registry_key;
    DWORD size = sizeof(os_product_pfn);

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_READ, &registry_key) == ERROR_SUCCESS)
    {
        if (RegQueryValueEx(registry_key, "OSProductPfn", NULL, NULL, (LPBYTE)os_product_pfn, &size) == ERROR_SUCCESS)
        {
            RegCloseKey(registry_key);
            return os_product_pfn;
        }
        RegCloseKey(registry_key);
    }
    return NULL;
}

void windows_set_compatibility_mode(const char *exe_path)
{
    HKEY registry_key;
    LONG result;

    result = RegCreateKeyExA(HKEY_CURRENT_USER,
                             "Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers",
                             0, NULL, REG_OPTION_NON_VOLATILE,
                             KEY_WRITE, NULL, &registry_key, NULL);

    if (result != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to open or create registry key.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Set compatibility mode for Windows XP SP3
    result = RegSetValueExA(registry_key,
                            exe_path,
                            0,
                            REG_SZ,
                            (const BYTE *)"WINXPSP3",
                            (DWORD)(strlen("WINXPSP3") + 1));

    if (result != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to set compatibility mode.", "Error", MB_OK | MB_ICONERROR);
    }

    RegCloseKey(registry_key);
}

void webview_windows_activation_callback(const char *seq, const char *req, void *arg)
{
    (void)seq;
    (void)req;
    webview_t webview = (webview_t)arg;

#ifdef _WIN32
    // Check for internet connectivity
    if (!system_check_internet_connectivity())
    {
        MessageBoxA(NULL, "No internet connection detected", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(webview);
        exit(1);
    }

    if (system_check_admin_privileges())
    {
        MessageBoxA(NULL, "Running as administrator", "Info", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxA(NULL, "Not running as administrator", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(webview);
        exit(1);
    }

    char *edition = windows_get_edition_id();
    if (edition)
    {
        char message[512];
        snprintf(message, sizeof(message), "Edition ID: %s", edition);
        MessageBoxA(NULL, message, "Info", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxA(NULL, "Failed to retrieve Edition ID", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(webview);
        exit(1);
    }

    const char *license_key = windows_get_license_key_for_edition(edition);
    if (license_key)
    {
        char message[512];
        snprintf(message, sizeof(message), "Matching License Key: %s", license_key);
        MessageBoxA(NULL, message, "Info", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxA(NULL, "No matching license key found", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(webview);
        exit(1);
    }

    MessageBoxA(NULL, "Apply generic Retail/OEM/MAK Key", "Info", MB_OK | MB_ICONINFORMATION);
    char command[512];
    snprintf(command, sizeof(command), "/c slmgr /ipk %s", license_key);
    system_execute_command_elevated(command);

    MessageBoxA(NULL, "Create folder at C:\\Files if not exists", "Info", MB_OK | MB_ICONINFORMATION);
    const char *files_directory = "C:\\Files";
    if (GetFileAttributes(files_directory) == INVALID_FILE_ATTRIBUTES)
    {
        CreateDirectory(files_directory, NULL);
    }

    MessageBoxA(NULL, "Download MS ADK file to C:\\Files\nThis will take some time, please wait :)", "Info", MB_OK | MB_ICONINFORMATION);
    system_download_file("https://download.microsoft.com/download/9/A/E/9AE69DD5-BA93-44E0-864E-180F5E700AB4/adk/Installers/14f4df8a2a7fc82a4f415cf6a341415d.cab", "C:/Files/14f4df8a2a7fc82a4f415cf6a341415d.cab");

    MessageBoxA(NULL, "Expand ADK files to C:\\Files\nThis will take some time, please wait :)", "Info", MB_OK | MB_ICONINFORMATION);
    system_execute_command_elevated("/c expand C:\\Files\\14f4df8a2a7fc82a4f415cf6a341415d.cab -F:* C:\\Files\\");

    if (rename("C:\\Files\\filf8377e82b29deadca67bc4858ed3fba9", "C:\\Files\\gatherosstate.exe") != 0)
    {
        MessageBoxA(NULL, "Failed to rename C:\\Files\\filf8377e82b29deadca67bc4858ed3fba9 to C:\\Files\\gatherosstate.exe", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(webview);
        exit(1);
    }

    MessageBoxA(NULL, "Patch C:\\Files\\gatherosstate.exe based on GamersOsState", "Info", MB_OK | MB_ICONINFORMATION);
    if (system_patch_gatherosstate_file("C:\\Files\\gatherosstate.exe", "C:\\Files\\gatherosstatemodified.exe") != 0)
    {
        MessageBoxA(NULL, "Failed to patch gatherosstate.exe", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(webview);
        exit(1);
    }

    MessageBoxA(NULL, "Set compatibility layer of C:\\Files\\gatherosstatemodified.exe to Windows XP SP3", "Info", MB_OK | MB_ICONINFORMATION);
    windows_set_compatibility_mode("C:\\Files\\gatherosstatemodified.exe");

    MessageBoxA(NULL, "Generate C:\\Files\\GenuineTicket.xml", "Info", MB_OK | MB_ICONINFORMATION);
    if (windows_generate_genuine_ticket("C:\\Files\\gatherosstatemodified.exe", "C:\\Files") != 0)
    {
        MessageBoxA(NULL, "Failed to generate genuine ticket", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(webview);
        exit(1);
    }

    MessageBoxA(NULL, "Copy C:\\Files\\GenuineTicket.xml to C:\\ProgramData\\Microsoft\\Windows\\ClipSVC\\GenuineTicket\\GenuineTicket.xml", "Info", MB_OK | MB_ICONINFORMATION);
    if (!CopyFile("C:\\Files\\GenuineTicket.xml", "C:\\ProgramData\\Microsoft\\Windows\\ClipSVC\\GenuineTicket\\GenuineTicket.xml", FALSE))
    {
        MessageBoxA(NULL, "Failed to copy the file", "Error", MB_OK | MB_ICONERROR);
    }

    MessageBoxA(NULL, "Apply C:\\Files\\GenuineTicket.xml", "Info", MB_OK | MB_ICONINFORMATION);
    system_execute_command_elevated("/c clipup -v -o");

    MessageBoxA(NULL, "Activate Windows", "Info", MB_OK | MB_ICONINFORMATION);
    system_execute_command_elevated("/c slmgr /ato");

    MessageBoxA(NULL, "Delete leftovers", "Info", MB_OK | MB_ICONINFORMATION);
    system_delete_directory_recursive("C:\\Files");
#else
    printf("Unsupported OS\n");
#endif
}

int windows_generate_genuine_ticket(const char *exe_path, const char *output_dir)
{
    // Get OSProductPfn from registry
    char os_product_pfn[256];
    HKEY registry_key;
    DWORD size = sizeof(os_product_pfn);

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_READ, &registry_key) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to open registry key for OSProductPfn", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    if (RegQueryValueEx(registry_key, "OSProductPfn", NULL, NULL, (LPBYTE)os_product_pfn, &size) != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to read OSProductPfn from registry", "Error", MB_OK | MB_ICONERROR);
        RegCloseKey(registry_key);
        return 1;
    }

    RegCloseKey(registry_key);

    // Build command to run gatherosstate
    char command[1024];
    snprintf(command, sizeof(command), "/c \"%s\" /c Pfn=%s`;PKeyIID=465145217131314304264339481117862266242033457260311819664735280",
             exe_path, os_product_pfn);

    // Change to output directory for execution
    SetCurrentDirectory(output_dir);

    system_execute_command_elevated(command);
    return 0;
}
