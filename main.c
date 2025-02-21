#include "webview/webview.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "index.modified.c"

#ifdef _WIN32
#include <windows.h>
#include "activate.h"
#endif

void activate_cb(const char *seq, const char *req, void *arg)
{
    (void)seq;
    (void)req;
    webview_t w = (webview_t)arg;

#ifdef _WIN32
    if (is_admin())
    {
        printf("Running as administrator\n");
    }
    else
    {
        printf("Not running as administrator\n");
        webview_terminate(w);
        exit(1);
    }

    char *edition = get_edition_id();
    if (edition)
    {
        printf("Edition ID: %s\n", edition);
    }
    else
    {
        printf("Failed to retrieve Edition ID\n");
        webview_terminate(w);
        exit(1);
    }

    char *key = get_license_key(edition);
    if (key)
    {
        printf("Matching License Key: %s\n", key);
    }
    else
    {
        printf("No matching license key found\n");
        webview_terminate(w);
        exit(1);
    }

    // activate_key(key);  # DEBUG
    printf("DEBUG: Skipping activation for now\n");

    char *osProductPfn = get_os_product_pfn();
    if (osProductPfn)
    {
        printf("OSProductPfn: %s\n", osProductPfn);
    }
    else
    {
        printf("Failed to read OSProductPfn\n");
        webview_terminate(w);
        exit(1);
    }

    // TODO: include the universal tickets using incbin
    //       copy matching ticket into C:\ProgramData\Microsoft\Windows\ClipSVC\GenuineTicket

    // apply the ticket
    // run_command("/c clipup -v -o");

    // activate Windows
    // run_command("/c slmgr /ato");

    // TODO: verify if it worked using slmgr /xpr.

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
#else
int main(void)
{
#endif
    webview_t w = webview_create(0, NULL);
    webview_set_title(w, "Windows Activation Tool - phoenixthrush");
    webview_set_size(w, 375, 450, WEBVIEW_HINT_FIXED);
    webview_set_html(w, (const char *)index_modified_html);

    webview_bind(w, "activate", activate_cb, NULL);
    webview_bind(w, "credits", credits_cb, NULL);
    webview_bind(w, "quit", quit_cb, w);

    webview_run(w);
    webview_destroy(w);

    return 0;
}
