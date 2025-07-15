#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "webview/webview.h"
#include "common.h"

#ifdef _WIN32
#include <windows.h>
#include "hwid.h"
#include "ohook.h"
#include "vs.h"
#include "vmware.h"
#endif

extern unsigned char index_modified[];
extern unsigned int index_modified_len;

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

    webview_t webview = webview_create(0, NULL);
    app_context_t context = {.webview = webview};

    webview_set_title(webview, "Universal Activation Toolkit");
    webview_set_size(webview, 800, 550, WEBVIEW_HINT_FIXED);
    webview_set_html(webview, (const char *)index_modified);

#ifdef _WIN32
    webview_bind(webview, "activate", webview_windows_activation_callback, NULL);
    webview_bind(webview, "ohook", webview_office_activation_callback, NULL);
    webview_bind(webview, "vs", webview_visual_studio_activation_callback, NULL);
    webview_bind(webview, "vmware", webview_vmware_activation_callback, NULL);
#endif
    webview_bind(webview, "quit", webview_quit_callback, webview);
    webview_bind(webview, "open_browser", webview_open_browser_callback, &context);

    webview_run(webview);
    webview_destroy(webview);

    return 0;
}
