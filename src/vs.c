#include <windows.h>
#include "common.h"
#include "webview/webview.h"
#include "vs.h"

// Visual Studio license keys database
static const visual_studio_license_key_t visual_studio_license_keys[] = {
    {"Visual Studio 2022 Professional", "TD244-P4NB7-YQ6XK-Y8MMM-YWV2J"},
    {"Visual Studio 2022 Enterprise", "VHF9H-NXBBB-638P6-6JHCY-88JWH"},
    {"Visual Studio 2019 Professional", "NYWVH-HT4XC-R2WYW-9Y3CM-X4V3Y"},
    {"Visual Studio 2019 Enterprise", "BF8Y8-GN2QH-T84XB-QVY3B-RC4DF"}};

static const size_t visual_studio_license_keys_count = sizeof(visual_studio_license_keys) / sizeof(visual_studio_license_keys[0]);

int visual_studio_find_edition_index(const char *edition)
{
    for (size_t i = 0; i < visual_studio_license_keys_count; ++i)
    {
        if (strcmp(visual_studio_license_keys[i].product_name, edition) == 0)
        {
            return (int)i;
        }
    }
    return -1;
}

void webview_visual_studio_activation_callback(const char *seq, const char *req, void *arg)
{
    (void)seq;
    webview_t webview = (webview_t)arg;

    // Parse edition from request JSON or use default
    const char *edition = "Visual Studio 2022 Professional"; // Default edition

    // If request contains edition selection, parse it
    if (req && strlen(req) > 2)
    {
        // Simple JSON parsing - look for "edition":"value" pattern
        const char *edition_start = strstr(req, "\"edition\":\"");
        if (edition_start)
        {
            edition_start += strlen("\"edition\":\"");
            const char *edition_end = strchr(edition_start, '"');
            if (edition_end)
            {
                size_t edition_len = edition_end - edition_start;
                if (edition_len < 256)
                { // Safety check
                    static char selected_edition[256];
                    strncpy(selected_edition, edition_start, edition_len);
                    selected_edition[edition_len] = '\0';
                    edition = selected_edition;
                }
            }
        }
    }

    int edition_index = visual_studio_find_edition_index(edition);
    if (edition_index == -1)
    {
        MessageBoxA(NULL, "Edition not found!", "Error", MB_OK | MB_ICONERROR);
        webview_terminate(webview);
        exit(1);
    }

    MessageBoxA(NULL, "Download Visual Studio", "Info", MB_OK | MB_ICONINFORMATION);
    system_download_file("https://aka.ms/vs/17/release/vs_professional.exe", "C:/Windows/Temp/vs_installer.exe");

    char command[512];
    snprintf(command, sizeof(command), "/c \"C:/Windows/Temp/vs_installer.exe\"");
    system_execute_command_elevated(command);

    const char *selected_key = visual_studio_license_keys[edition_index].license_key;
    char message[512];
    snprintf(message, sizeof(message),
             "Use this key after installation is complete:\n\n%s\n\nThis key will be copied to your clipboard.",
             selected_key);
    MessageBoxA(NULL, message, "License Key", MB_OK | MB_ICONINFORMATION);

    // Copy key to clipboard
    if (OpenClipboard(NULL))
    {
        EmptyClipboard();
        HGLOBAL global_memory = GlobalAlloc(GMEM_MOVEABLE, strlen(selected_key) + 1);
        if (global_memory)
        {
            char *clipboard_data = (char *)GlobalLock(global_memory);
            strcpy(clipboard_data, selected_key);
            GlobalUnlock(global_memory);
            SetClipboardData(CF_TEXT, global_memory);
        }
        CloseClipboard();

        MessageBoxA(NULL, "Install and Activate", "Info", MB_OK | MB_ICONINFORMATION);
        char install_command[512];
        snprintf(install_command, sizeof(install_command), "/c C:/Windows/Temp/vs_installer.exe --add-product --product-key %s --passive", visual_studio_license_keys[edition_index].license_key);
        system_execute_command_elevated(install_command);
    }
}