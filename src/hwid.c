// this will be optimised and rewritten

#include <windows.h>
#include <stdio.h>
#include "webview/webview.h"
#include "common.h"
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

void set_compatibility_mode(const char *exe_path)
{
    HKEY hKey;
    LONG lResult;

    lResult = RegCreateKeyExA(HKEY_CURRENT_USER,
                              "Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers",
                              0, NULL, REG_OPTION_NON_VOLATILE,
                              KEY_WRITE, NULL, &hKey, NULL);

    if (lResult != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to open or create registry key.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    // set compatibility mode for Windows XP SP3
    lResult = RegSetValueExA(hKey,
                             exe_path,
                             0,
                             REG_SZ,
                             (const BYTE *)"WINXPSP3",
                             (DWORD)(strlen("WINXPSP3") + 1));

    if (lResult != ERROR_SUCCESS)
    {
        MessageBoxA(NULL, "Failed to set compatibility mode.", "Error", MB_OK | MB_ICONERROR);
    }

    RegCloseKey(hKey);
}

void activate_cb(const char *seq, const char *req, void *arg)
{
    (void)seq;
    (void)req;
    webview_t w = (webview_t)arg;

#ifdef _WIN32
    // TODO: add check for internet connectivity
    // TODO: replace run_command() with actual C code where applicable

    if (is_admin())
    {
        MessageBoxA(NULL, "Running as administrator", "Info", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxA(NULL, "Not running as administrator", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(w);
        exit(1);
    }

    /*
    MessageBoxA(NULL, "Changing Windows Region To USA ", "Info", MB_OK | MB_ICONINFORMATION);
    run_command("/c powershell -EncodedCommand ZgBvAHIAIAAvAGYAIAAiAHMAawBpAHAAPQAyACAAdABvAGsAZQBuAHMAPQAyACoAIgAgACUAJQBhACAAaQBuACAAKAAnAHIAZQBnACAAcQB1AGUAcgB5ACAAIgBIAEsAQwBVAFwAQwBvAG4AdAByAG8AbAAgAFAAYQBuAGUAbABcAEkAbgB0AGUAcgBuAGEAdABpAG8AbgBhAGwAXABHAGUAbwAiACAALwB2ACAATgBhAG0AZQAgACUAbgB1AGwANgAlACcAKQAgAGQAbwAgAHMAZQB0ACAAIgBuAGEAbQBlAD0AJQAlAGIAIgAKAGYAbwByACAALwBmACAAIgBzAGsAaQBwAD0AMgAgAHQAbwBrAGUAbgBzAD0AMgAqACIAIAAlACUAYQAgAGkAbgAgACgAJwByAGUAZwAgAHEAdQBlAHIAeQAgACIASABLAEMAVQBcAEMAbwBuAHQAcgBvAGwAIABQAGEAbgBlAGwAXABJAG4AdABlAHIAbgBhAHQAaQBvAG4AYQBsAFwARwBlAG8AIgAgAC8AdgAgAE4AYQB0AGkAbwBuACAAJQBuAHUAbAA2ACUAJwApACAAZABvACAAcwBlAHQAIAAiAG4AYQB0AGkAbwBuAD0AJQAlAGIAIgAKAAoAcwBlAHQAIAByAGUAZwBpAG8AbgBjAGgAYQBuAGcAZQA9AAoAaQBmACAAbgBvAHQAIAAiACUAbgBhAG0AZQAlACIAPQA9ACIAVQBTACIAIAAoAAoAcwBlAHQAIAByAGUAZwBpAG8AbgBjAGgAYQBuAGcAZQA9ADEACgAlAHAAcwBjACUAIAAiAFMAZQB0AC0AVwBpAG4ASABvAG0AZQBMAG8AYwBhAHQAaQBvAG4AIAAtAEcAZQBvAEkAZAAgADIANAA0ACIAIAAlAG4AdQBsACUACgBpAGYAIAAhAGUAcgByAG8AcgBsAGUAdgBlAGwAIQAgAEUAUQBVACAAMAAgACgACgBlAGMAaABvACAAQwBoAGEAbgBnAGkAbgBnACAAVwBpAG4AZABvAHcAcwAgAFIAZQBnAGkAbwBuACAAVABvACAAVQBTAEEAIAAgACAAIAAgACAAIAAgACAAIABbAFMAdQBjAGMAZQBzAHMAZgB1AGwAXQAKACkAIABlAGwAcwBlACAAKAAKAGMAYQBsAGwAIAA6AGQAawBfAGMAbwBsAG8AcgAgACUAUgBlAGQAJQAgACIAQwBoAGEAbgBnAGkAbgBnACAAVwBpAG4AZABvAHcAcwAgAFIAZQBnAGkAbwBuACAAVABvACAAVQBTAEEAIAAgACAAIAAgACAAIAAgACAAIABbAEYAYQBpAGwAZQBkAF0AIgAKACkACgApAA==");
    */

    char *edition = get_edition_id();
    if (edition)
    {
        char msg[512];
        snprintf(msg, sizeof(msg), "Edition ID: %s", edition);
        MessageBoxA(NULL, msg, "Info", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxA(NULL, "Failed to retrieve Edition ID", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(w);
        exit(1);
    }

    char *key = get_license_key(edition);
    if (key)
    {
        char msg[512];
        snprintf(msg, sizeof(msg), "Matching License Key: %s", key);
        MessageBoxA(NULL, msg, "Info", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxA(NULL, "No matching license key found", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(w);
        exit(1);
    }

    MessageBoxA(NULL, "Apply generic Retail/OEM/MAK Key", "Info", MB_OK | MB_ICONINFORMATION);
    char command[512];
    snprintf(command, sizeof(command), "/c slmgr /ipk %s", key);
    run_command(command);

    // using pre generated universal ticket
    /*
    char *osProductPfn = get_os_product_pfn();
    if (osProductPfn)
    {
        char msg[512];
        snprintf(msg, sizeof(msg), "OSProductPfn: %s", osProductPfn);
        MessageBoxA(NULL, msg, "Info", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxA(NULL, "Failed to read OSProductPfn", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(w);
        exit(1);
    }

    char command[2048];
    snprintf(command, sizeof(command),
             "/c powershell -Command \"Invoke-WebRequest -Uri 'https://gist.github.com/phoenixthrush/4c295a1176298a04acd2353aaef8a71e/raw/aeebac03771684b8e04867d76a01604df63076d6/%s.xml' -OutFile 'C:\\\\ProgramData\\\\Microsoft\\\\Windows\\\\ClipSVC\\\\GenuineTicket.xml'\"",
             osProductPfn);

    run_command(command);
    */

    MessageBoxA(NULL, "Create folder at C:\\Files if not exists", "Info", MB_OK | MB_ICONINFORMATION);
    const char *dir = "C:\\Files";
    if (GetFileAttributes(dir) == INVALID_FILE_ATTRIBUTES)
    {
        CreateDirectory(dir, NULL);
    }

    MessageBoxA(NULL, "Download MS ADK file to C:\\Files\nThis will take some time, please wait :)", "Info", MB_OK | MB_ICONINFORMATION);
    // run_command("/c powershell -c Start-BitsTransfer -Source \"https://download.microsoft.com/download/9/A/E/9AE69DD5-BA93-44E0-864E-180F5E700AB4/adk/Installers/14f4df8a2a7fc82a4f415cf6a341415d.cab\" -Destination \"C:\\Files\\14f4df8a2a7fc82a4f415cf6a341415d.cab\"");
    download_file("https://download.microsoft.com/download/9/A/E/9AE69DD5-BA93-44E0-864E-180F5E700AB4/adk/Installers/14f4df8a2a7fc82a4f415cf6a341415d.cab", "C:/Files/14f4df8a2a7fc82a4f415cf6a341415d.cab");

    MessageBoxA(NULL, "Expand ADK files to C:\\Files\nThis will take some time, please wait :)", "Info", MB_OK | MB_ICONINFORMATION);
    run_command("/c expand C:\\Files\\14f4df8a2a7fc82a4f415cf6a341415d.cab -F:* C:\\Files\\");

    if (rename("C:\\Files\\filf8377e82b29deadca67bc4858ed3fba9", "C:\\Files\\gatherosstate.exe") != 0)
    {
        MessageBoxA(NULL, "Failed to rename C:\\Files\\filf8377e82b29deadca67bc4858ed3fba9 to C:\\Files\\gatherosstate.exe", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(w);
        exit(1);
    }

    MessageBoxA(NULL, "Patch C:\\Files\\gatherosstate.exe based on GamersOsState", "Info", MB_OK | MB_ICONINFORMATION);
    run_command("/c powershell -EncodedCommand JABiAHkAdABlAHMAIAAgAD0AIABbAFMAeQBzAHQAZQBtAC4ASQBPAC4ARgBpAGwAZQBdADoAOgBSAGUAYQBkAEEAbABsAEIAeQB0AGUAcwAoACIAQwA6AFwARgBpAGwAZQBzAFwAZwBhAHQAaABlAHIAbwBzAHMAdABhAHQAZQAuAGUAeABlACIAKQAKACQAYgB5AHQAZQBzAFsAMwAyADAAXQAgAD0AIAAwAHgAZgA4AAoAJABiAHkAdABlAHMAWwAzADIAMQBdACAAPQAgADAAeABmAGIACgAkAGIAeQB0AGUAcwBbADMAMgAyAF0AIAA9ACAAMAB4ADAANQAKACQAYgB5AHQAZQBzAFsAMwAyADQAXQAgAD0AIAAwAHgAMAAzAAoAJABiAHkAdABlAHMAWwAxADMANgA3ADIAXQAgAD0AIAAwAHgAMgA1AAoAJABiAHkAdABlAHMAWwAxADMANgA3ADQAXQAgAD0AIAAwAHgANwAzAAoAJABiAHkAdABlAHMAWwAxADMANgA3ADYAXQAgAD0AIAAwAHgAMwBiAAoAJABiAHkAdABlAHMAWwAxADMANgA3ADgAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAxADMANgA4ADAAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAxADMANgA4ADIAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAxADMANgA4ADQAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADIANwA0ADgAXQAgAD0AIAAwAHgAZQA5AAoAJABiAHkAdABlAHMAWwAzADIANwA0ADkAXQAgAD0AIAAwAHgAOQBlAAoAJABiAHkAdABlAHMAWwAzADIANwA1ADAAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADIANwA1ADEAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADIANwA1ADIAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADIAOAA5ADQAXQAgAD0AIAAwAHgAOABiAAoAJABiAHkAdABlAHMAWwAzADIAOAA5ADUAXQAgAD0AIAAwAHgANAA0AAoAJABiAHkAdABlAHMAWwAzADIAOAA5ADcAXQAgAD0AIAAwAHgANgA0AAoAJABiAHkAdABlAHMAWwAzADIAOAA5ADgAXQAgAD0AIAAwAHgAOAA1AAoAJABiAHkAdABlAHMAWwAzADIAOAA5ADkAXQAgAD0AIAAwAHgAYwAwAAoAJABiAHkAdABlAHMAWwAzADIAOQAwADAAXQAgAD0AIAAwAHgAMABmAAoAJABiAHkAdABlAHMAWwAzADIAOQAwADEAXQAgAD0AIAAwAHgAOAA1AAoAJABiAHkAdABlAHMAWwAzADIAOQAwADIAXQAgAD0AIAAwAHgAMQBjAAoAJABiAHkAdABlAHMAWwAzADIAOQAwADMAXQAgAD0AIAAwAHgAMAAyAAoAJABiAHkAdABlAHMAWwAzADIAOQAwADQAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADIAOQAwADYAXQAgAD0AIAAwAHgAZQA5AAoAJABiAHkAdABlAHMAWwAzADIAOQAwADcAXQAgAD0AIAAwAHgAMwBjAAoAJABiAHkAdABlAHMAWwAzADIAOQAwADgAXQAgAD0AIAAwAHgAMAAxAAoAJABiAHkAdABlAHMAWwAzADIAOQAwADkAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADIAOQAxADAAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADIAOQAxADEAXQAgAD0AIAAwAHgAOAA1AAoAJABiAHkAdABlAHMAWwAzADIAOQAxADIAXQAgAD0AIAAwAHgAZABiAAoAJABiAHkAdABlAHMAWwAzADIAOQAxADMAXQAgAD0AIAAwAHgANwA1AAoAJABiAHkAdABlAHMAWwAzADIAOQAxADQAXQAgAD0AIAAwAHgAZQBiAAoAJABiAHkAdABlAHMAWwAzADIAOQAxADUAXQAgAD0AIAAwAHgAZQA5AAoAJABiAHkAdABlAHMAWwAzADIAOQAxADYAXQAgAD0AIAAwAHgANgA5AAoAJABiAHkAdABlAHMAWwAzADIAOQAxADcAXQAgAD0AIAAwAHgAZgBmAAoAJABiAHkAdABlAHMAWwAzADIAOQAxADgAXQAgAD0AIAAwAHgAZgBmAAoAJABiAHkAdABlAHMAWwAzADIAOQAxADkAXQAgAD0AIAAwAHgAZgBmAAoAJABiAHkAdABlAHMAWwAzADMAMAA5ADQAXQAgAD0AIAAwAHgAZQA5AAoAJABiAHkAdABlAHMAWwAzADMAMAA5ADUAXQAgAD0AIAAwAHgAOAAwAAoAJABiAHkAdABlAHMAWwAzADMAMAA5ADYAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADMAMAA5ADcAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADMAMAA5ADgAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADMANAA0ADkAXQAgAD0AIAAwAHgANgA0AAoAJABiAHkAdABlAHMAWwAzADMANQA3ADYAXQAgAD0AIAAwAHgAOABkAAoAJABiAHkAdABlAHMAWwAzADMANQA3ADcAXQAgAD0AIAAwAHgANQA0AAoAJABiAHkAdABlAHMAWwAzADMANQA3ADkAXQAgAD0AIAAwAHgAMgA0AAoAJABiAHkAdABlAHMAWwAzADMANQA4ADAAXQAgAD0AIAAwAHgAZQA5AAoAJABiAHkAdABlAHMAWwAzADMANQA4ADEAXQAgAD0AIAAwAHgANQA1AAoAJABiAHkAdABlAHMAWwAzADMANQA4ADIAXQAgAD0AIAAwAHgAMAAxAAoAJABiAHkAdABlAHMAWwAzADMANQA4ADMAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADMANQA4ADQAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADMAOQA3ADgAXQAgAD0AIAAwAHgAYwAzAAoAJABiAHkAdABlAHMAWwAzADQAMQA4ADkAXQAgAD0AIAAwAHgANQA5AAoAJABiAHkAdABlAHMAWwAzADQAMQA5ADAAXQAgAD0AIAAwAHgAZQBiAAoAJABiAHkAdABlAHMAWwAzADQAMQA5ADEAXQAgAD0AIAAwAHgAMgA4AAoAJABiAHkAdABlAHMAWwAzADQAMgAzADgAXQAgAD0AIAAwAHgAZQA5AAoAJABiAHkAdABlAHMAWwAzADQAMgAzADkAXQAgAD0AIAAwAHgANABmAAoAJABiAHkAdABlAHMAWwAzADQAMgA0ADAAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADQAMgA0ADEAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADQAMgA0ADIAXQAgAD0AIAAwAHgAMAAwAAoAJABiAHkAdABlAHMAWwAzADQAMwA0ADYAXQAgAD0AIAAwAHgAMgA0AAoAJABiAHkAdABlAHMAWwAzADQAMwA3ADYAXQAgAD0AIAAwAHgAZQBiAAoAJABiAHkAdABlAHMAWwAzADQAMwA3ADcAXQAgAD0AIAAwAHgANgAzAAoAWwBTAHkAcwB0AGUAbQAuAEkATwAuAEYAaQBsAGUAXQA6ADoAVwByAGkAdABlAEEAbABsAEIAeQB0AGUAcwAoACIAQwA6AFwARgBpAGwAZQBzAFwAZwBhAHQAaABlAHIAbwBzAHMAdABhAHQAZQBtAG8AZABpAGYAaQBlAGQALgBlAHgAZQAiACwAIAAkAGIAeQB0AGUAcwApAA==");

    MessageBoxA(NULL, "Set compatibility layer of C:\\Files\\gatherosstatemodified.exe to Windows XP SP3", "Info", MB_OK | MB_ICONINFORMATION);
    set_compatibility_mode("C:\\Files\\gatherosstatemodified.exe");

    MessageBoxA(NULL, "Generate C:\\Files\\GenuineTicket.xml", "Info", MB_OK | MB_ICONINFORMATION);
    run_command("/c powershell -c $value = (Get-ItemProperty HKLM:\\SYSTEM\\CurrentControlSet\\Control\\ProductOptions).OSProductPfn; C:\\Files\\gatherosstatemodified.exe /c Pfn=$value`;PKeyIID=465145217131314304264339481117862266242033457260311819664735280");

    MessageBoxA(NULL, "Copy C:\\Files\\GenuineTicket.xml to C:\\ProgramData\\Microsoft\\Windows\\ClipSVC\\GenuineTicket\\GenuineTicket.xml", "Info", MB_OK | MB_ICONINFORMATION);
    if (!CopyFile("C:\\Files\\GenuineTicket.xml", "C:\\ProgramData\\Microsoft\\Windows\\ClipSVC\\GenuineTicket\\GenuineTicket.xml", FALSE))
    {
        MessageBoxA(NULL, "Failed to copy the file", "Error", MB_OK | MB_ICONERROR);
    }

    MessageBoxA(NULL, "Apply C:\\Files\\GenuineTicket.xml", "Info", MB_OK | MB_ICONINFORMATION);
    run_command("/c clipup -v -o");

    MessageBoxA(NULL, "Activate Windows", "Info", MB_OK | MB_ICONINFORMATION);
    run_command("/c slmgr /ato");

    MessageBoxA(NULL, "Delete leftovers", "Info", MB_OK | MB_ICONINFORMATION);
    run_command("/c powershell -c Remove-Item -Path C:\\Files -Recurse -Force");
#else
    printf("Unsupported OS\n");
#endif
}
