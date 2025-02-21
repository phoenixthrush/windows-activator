#include "webview/webview.h"

#define INCBIN_PREFIX g_
#include "incbin.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// #include "html.c"
INCBIN(index_html, "site/index.html");

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
    // this definitely needs to be implemented by me in the future... lol
    // system("powershell -ExecutionPolicy Bypass -NoProfile -Command \"& ([ScriptBlock]::Create((irm https://get.activated.win))) /HWID\"");

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

    activate_key(key);

    // include the universal tickets using incbin
    // (Get-ItemProperty HKLM:\SYSTEM\CurrentControlSet\Control\ProductOptions).OSProductPfn
    // copy matching ticket into C:\ProgramData\Microsoft\Windows\ClipSVC\GenuineTicket
    // clipup -v -o
    // slmgr /ato
    // maybe verify if it worked using slmgr /xpr.

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

    /*
    // null terminate raw data
    char html[site_index_html_len + 1];
    strncpy(html, (const char *)site_index_html, site_index_html_len);
    html[site_index_html_len] = '\0';
    */

    // Debug: Check if the INCBIN worked
    printf("g_index_html_len: %zu\n", g_index_html_len); // Debug: Print length of the embedded HTML
    printf("g_index_html: %p\n", g_index_html);          // Debug: Print pointer to the embedded HTML

    webview_set_html(w, html);

    webview_bind(w, "activate", activate_cb, NULL);
    webview_bind(w, "credits", credits_cb, NULL);
    webview_bind(w, "quit", quit_cb, w);

    webview_run(w);
    webview_destroy(w);

    return 0;
}
