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
    // TODO: check for internet connectivity
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

    // activate_key(key);  // DEBUG
    MessageBoxA(NULL, "DEBUG: Skipping activation", "Debug", MB_OK | MB_ICONINFORMATION);

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

    // TODO: Generate it from scratch instead
    char url[512];
    snprintf(url, sizeof(url), "https://gist.github.com/phoenixthrush/4c295a1176298a04acd2353aaef8a71e/raw/aeebac03771684b8e04867d76a01604df63076d6/%s.xml", osProductPfn);
    MessageBoxA(NULL, url, "Downloading to C:\\ProgramData\\Microsoft\\Windows\\ClipSVC\\GenuineTicket", MB_OK | MB_ICONINFORMATION);

    const char *save_path = "C:\\ProgramData\\Microsoft\\Windows\\ClipSVC\\GenuineTicket\\file.xml";

    if (download_file(url, save_path))
    {
        MessageBoxA(NULL, "Download successful", "Info", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBoxA(NULL, "Download failed", "Error", MB_OK | MB_ICONERROR);
    }

    // apply the ticket
    // run_command("/c clipup -v -o");
    MessageBoxA(NULL, "DEBUG: Skipping ticket application", "Debug", MB_OK | MB_ICONINFORMATION);

    // activate Windows
    // run_command("/c slmgr /ato");
    MessageBoxA(NULL, "DEBUG: Skipping activation", "Debug", MB_OK | MB_ICONINFORMATION);

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
