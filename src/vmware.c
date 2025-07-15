#include <windows.h>
#include "common.h"
#include "webview/webview.h"
#include "vmware.h"

// VMware license keys database
static const vmware_license_key_t vmware_license_keys[] = {
    {"VMware Workstation Pro 17", "MC60H-DWHD5-H80U9-6V85M-8280D"},
    {"VMware Workstation Pro 17", "4A4RR-813DK-M81A9-4U35H-06KND"},
    {"VMware Workstation Pro 17", "NZ4RR-FTK5H-H81C1-Q30QH-1V2LA"},
    {"VMware Workstation Pro 17", "JU090-6039P-08409-8J0QH-2YR7F"},
    {"VMware Workstation Pro 17", "4Y09U-AJK97-089Z0-A3054-83KLA"},
    {"VMware Workstation Pro 17", "4C21U-2KK9Q-M8130-4V2QH-CF810"},
    {"VMware Workstation Pro 17", "HY45K-8KK96-MJ8E0-0UCQ4-0UH72"},
    {"VMware Workstation Pro 17", "JC0D8-F93E4-HJ9Q9-088N6-96A7F"},
    {"VMware Workstation Pro 17", "NG0RK-2DK9L-HJDF8-1LAXP-1ARQ0"},
    {"VMware Workstation Pro 17", "0U2J0-2E19P-HJEX1-132Q2-8AKK6"},
    {"VMware Fusion 13", "4A4RR-813DK-M81A9-4U35H-06KND"},
    {"VMware Fusion 13", "NZ4RR-FTK5H-H81C1-Q30QH-1V2LA"},
    {"VMware Fusion 13", "4C21U-2KK9Q-M8130-4V2QH-CF810"},
    {"VMware Fusion 13", "MC60H-DWHD5-H80U9-6V85M-8280D"},
    {"VMware Fusion 13", "JU090-6039P-08409-8J0QH-2YR7F"},
    {"VMware Fusion 13", "4Y09U-AJK97-089Z0-A3054-83KLA"}};

static const size_t vmware_license_keys_count = sizeof(vmware_license_keys) / sizeof(vmware_license_keys[0]);

int vmware_find_edition_index(const char *edition)
{
    for (size_t i = 0; i < vmware_license_keys_count; ++i)
    {
        if (strcmp(vmware_license_keys[i].product_name, edition) == 0)
        {
            return (int)i;
        }
    }
    return -1;
}

void webview_vmware_activation_callback(const char *seq, const char *req, void *arg)
{
    (void)seq;
    (void)req;
    webview_t webview = (webview_t)arg;

    const char *edition = "VMware Workstation Pro 17";

    int edition_index = vmware_find_edition_index(edition);
    if (edition_index == -1)
    {
        MessageBoxA(NULL, "Edition not found!", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(webview);
        exit(1);
    }

    // Dynamically fetch latest download link or use fallback
    const char *download_url = "https://softwareupdate.vmware.com/cds/vmw-desktop/ws/17.6.2/24409262/windows/core/VMware-workstation-17.6.2-24409262.exe.tar";
    const char *installer_file = "VMware-workstation-17.6.2-24409262.exe.tar";
    const char *extracted_exe = "VMware-workstation-17.6.2-24409262.exe";

    // In a full implementation, this could query VMware's API for the latest version
    // For now, use the hardcoded latest known version

    MessageBoxA(NULL, "Download VMware Workstation Pro", "Info", MB_OK | MB_ICONINFORMATION);
    char download_path[MAX_PATH];
    snprintf(download_path, sizeof(download_path), "C:/Windows/Temp/%s", installer_file);
    system_download_file(download_url, download_path);

    MessageBoxA(NULL, "Extract VMware Workstation Pro", "Info", MB_OK | MB_ICONINFORMATION);
    if (system_extract_tar_archive(download_path, "C:/Windows/Temp/") != 0)
    {
        MessageBoxA(NULL, "Extraction failed!", "Error", MB_OK | MB_ICONERROR);
    }

    MessageBoxA(NULL, "Install and Activate", "Info", MB_OK | MB_ICONINFORMATION);
    char command[512];
    snprintf(command, sizeof(command), "/c C:/Windows/Temp/%s --add-product --product-key %s --passive", extracted_exe, vmware_license_keys[edition_index].license_key);
    system_execute_command_elevated(command);
}