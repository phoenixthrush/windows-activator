#ifndef HWID_H
#define HWID_H

#include <windows.h>
#include <stdio.h>
#include "webview/webview.h"

// Windows activation license key structure
typedef struct
{
    const char *edition_name;
    const char *edition_id;
    const char *product_key;
} windows_license_key_t;

// Windows activation functions
char *windows_get_edition_id(void);
const char *windows_get_license_key_for_edition(const char *edition_id);
char *windows_get_os_product_pfn(void);
void windows_set_compatibility_mode(const char *exe_path);
int windows_generate_genuine_ticket(const char *exe_path, const char *output_dir);

// WebView callback
void webview_windows_activation_callback(const char *seq, const char *req, void *arg);

#endif // HWID_H
