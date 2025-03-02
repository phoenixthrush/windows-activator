#include <windows.h>
#include "common.h"
#include "webview/webview.h"
#include "common.h"

struct vmwareKeys
{
    char *product;
    char *key;
};

struct vmwareKeys vmware_keys[] = {
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

int get_edition_index_vmware(const char *edition)
{
    for (int i = 0; i < sizeof(vmware_keys) / sizeof(vmware_keys[0]); ++i)
    {
        if (strcmp(vmware_keys[i].product, edition) == 0)
        {
            return i;
        }
    }
    return -1;
}

void vmware_cb(const char *seq, const char *req, void *arg)
{
    (void)seq;
    (void)req;
    webview_t w = (webview_t)arg;

    const char *edition = "VMware Workstation Pro 17";

    int edition_index = get_edition_index_vmware(edition);
    if (edition_index == -1)
    {
        MessageBoxA(NULL, "Edition not found!", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(w);
        exit(1);
    }

    // TODO: dynamically fetch latest link
    MessageBoxA(NULL, "Download VMware Workstation Pro", "Info", MB_OK | MB_ICONINFORMATION);
    download_file("https://softwareupdate.vmware.com/cds/vmw-desktop/ws/17.6.2/24409262/windows/core/VMware-workstation-17.6.2-24409262.exe.tar", "C:/Windows/Temp/VMware-workstation-17.6.2-24409262.exe.tar");

    MessageBoxA(NULL, "Extract VMware Workstation Pro", "Info", MB_OK | MB_ICONINFORMATION);
    if (extract_tar("C:/Windows/Temp/VMware-workstation-17.6.2-24409262.exe.tar", "C:/Windows/Temp/") != 0)
    {
        MessageBoxA(NULL, "Extraction failed!", "Error", MB_OK | MB_ICONERROR);
    }

    MessageBoxA(NULL, "Install and Activate", "Info", MB_OK | MB_ICONINFORMATION);
    char command[512];
    sprintf(command, "/c C:/Windows/Temp/VMware-workstation-17.6.2-24409262.exe --add-product --product-key %s --passive", vmware_keys[edition_index].key);
    run_command(command);
}