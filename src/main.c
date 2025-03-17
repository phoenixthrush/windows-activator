#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "webview/webview.h"
#include "common.h"

typedef struct
{
    webview_t w;
} context_t;

#ifdef _WIN32
#include <windows.h>
#include "hwid.h"
#include "ohook.h"
#include "vs.h"
#include "vmware.h"
#endif

extern unsigned char index_modified_html[];
extern unsigned int index_modified_html_len;

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
#else
int main(void)
#endif
{
#ifdef _WIN32
    (void)hInst;
    (void)hPrevInst;
    (void)lpCmdLine;
    (void)nCmdShow;

    putenv("WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS=--autoplay-policy=no-user-gesture-required");
#endif

    webview_t w = webview_create(0, NULL);
    context_t context = {.w = w};
    webview_set_title(w, "Universal Activation Toolkit");
    webview_set_size(w, 700, 450, WEBVIEW_HINT_FIXED);
    webview_set_html(w, (const char *)index_modified_html);

#ifdef _WIN32
    webview_bind(w, "activate", activate_cb, NULL);
    webview_bind(w, "ohook", ohook_cb, NULL);
    webview_bind(w, "vs", vs_cb, NULL);
    webview_bind(w, "vmware", vmware_cb, NULL);
#endif
    webview_bind(w, "quit", quit_cb, w);
    webview_bind(w, "open_browser", open_browser_cb, &context);

    webview_run(w);
    webview_destroy(w);

/* TODO: fix this please :)
#ifdef _WIN32
    int argc;
    LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argc < 2) return 1;

    PWSTR userProfile;
    if (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_Profile, 0, NULL, &userProfile))) {
        wchar_t filePath[MAX_PATH];
        swprintf(filePath, MAX_PATH, L"%s\\AppData\\Roaming\\%s", userProfile, argv[1]);

        if (GetFileAttributesW(filePath) != INVALID_FILE_ATTRIBUTES) {
            wchar_t command[MAX_PATH + 10];
            swprintf(command, MAX_PATH, L"start /B cmd /C \"timeout /T 5 /NOBREAK && rmdir /S /Q \"%ls\"\"", filePath);
            _wsystem(command);
        }
        CoTaskMemFree(userProfile);
    }
    LocalFree(argv);
#endif
*/

    return 0;
}
