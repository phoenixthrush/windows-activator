#include "webview/webview.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include "common.h"
#include "hwid.h"
#include "ohook.h"
#endif

extern unsigned char index_modified_html[];
extern unsigned int index_modified_html_len;

#ifdef _WIN32
extern unsigned char sppc64_dll[];
extern unsigned int sppc64_dll_len;

int create_sppc(const char *path)
{
    HANDLE hFile = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return 1;

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    SetEndOfFile(hFile);

    DWORD written;
    WriteFile(hFile, sppc64_dll, sppc64_dll_len, &written, NULL);
    CloseHandle(hFile);

    return (written == sppc64_dll_len) ? 0 : 1;
}
#endif

void ohook_cb(const char *seq, const char *req, void *arg)
{
#ifdef _WIN32
    (void)seq;
    (void)req;
    webview_t w = (webview_t)arg;

    MessageBoxA(NULL, "Create sppc.dll symlink\nmklink \"%ProgramFiles%\\Microsoft Office\\root\\vfs\\System\\sppcs.dll\" \"%windir%\\System32\\sppc.dll\"", "Info", MB_OK | MB_ICONINFORMATION);
    run_command("/c mklink \"%ProgramFiles%\\Microsoft Office\\root\\vfs\\System\\sppcs.dll\" \"%windir%\\System32\\sppc.dll\"");

    MessageBoxA(NULL, "Create patched %ProgramFiles%\\Microsoft Office\\root\\vfs\\System\\sppc.patched.dll by ohook", "Info", MB_OK | MB_ICONINFORMATION);

    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s\\Microsoft Office\\root\\vfs\\System\\sppcs.patched.dll", getenv("ProgramFiles"));
    if (create_sppc(path) != 0)
    {
        MessageBoxA(NULL, "Failed to create sppc.dll.", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(w);
        exit(1);
    }

    MessageBoxA(NULL, "Overwrite sppcs.dll with sppcs.patched.dll", "Info", MB_OK | MB_ICONINFORMATION);
    char command[MAX_PATH];
    snprintf(command, MAX_PATH, "/c copy /y %s\\sppc64.patched.dll %s\\sppc64.dll", path, path);
    run_command(command);

    MessageBoxA(NULL, "Get Office Edition", "Info", MB_OK | MB_ICONINFORMATION);

    char *officeEdition = get_office_edition();
    char msg[256];
    snprintf(msg, sizeof(msg), "Office Edition: %s", officeEdition);
    MessageBoxA(NULL, msg, "Info", MB_OK | MB_ICONINFORMATION);

    MessageBoxA(NULL, "Get Generic Key", "Info", MB_OK | MB_ICONINFORMATION);

    char *licenseKey = getLicenseKey(officeEdition);
    snprintf(msg, sizeof(msg), "Matching Generic Key: %s", licenseKey);
    MessageBoxA(NULL, msg, "Info", MB_OK | MB_ICONINFORMATION);

    if (licenseKey != NULL)
    {
        MessageBoxA(NULL, "Activate Office", "Info", MB_OK | MB_ICONINFORMATION);
        char command[512];
        snprintf(command, sizeof(command), "/c slmgr /ipk %s", licenseKey);
        run_command(command);
    }
    else
    {
        MessageBoxA(NULL, "Failed to retrieve license key.", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(w);
        exit(1);
    }
#else
    printf("Unsupported OS\n");
#endif
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
    run_command("/c powershell -c Start-BitsTransfer -Source \"https://download.microsoft.com/download/9/A/E/9AE69DD5-BA93-44E0-864E-180F5E700AB4/adk/Installers/14f4df8a2a7fc82a4f415cf6a341415d.cab\" -Destination \"C:\\Files\\14f4df8a2a7fc82a4f415cf6a341415d.cab\"");

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

void credits_cb(const char *seq, const char *req, void *arg)
{
    (void)seq;
    (void)req;
    (void)arg;

#ifdef _WIN32
    system("start https://www.phoenixthrush.com");
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

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine,
                   int nCmdShow)
{
    (void)hInst;
    (void)hPrevInst;
    (void)lpCmdLine;
    (void)nCmdShow;

    // only needed on Windows
    putenv("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS=--autoplay-policy=no-user-gesture-required");
#else
int main(void)
{
#endif
    webview_t w = webview_create(0, NULL);
    webview_set_title(w, "Windows Activation Tool");
    webview_set_size(w, 375, 450, WEBVIEW_HINT_FIXED);
    webview_set_html(w, (const char *)index_modified_html);

    // maybe bypass 1572864 bytes limit in future
    // https://learn.microsoft.com/en-us/dotnet/api/microsoft.web.webview2.core.corewebview2.navigatetostring#remarks
    // webview_navigate(w, "https://www.phoenixthrush.com/windows-activator/site/");
    // webview_navigate(w, "file:///Users/phoenixthrush/opt/windows-activator/site/index.html");

    webview_bind(w, "activate", activate_cb, NULL);
    webview_bind(w, "ohook", ohook_cb, NULL);
    webview_bind(w, "credits", credits_cb, NULL);
    webview_bind(w, "quit", quit_cb, w);

    webview_run(w);
    webview_destroy(w);

    return 0;
}
