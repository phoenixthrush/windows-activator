#ifndef OHOOK_H
#define OHOOK_H

#include <windows.h>
#include <stdio.h>
#include "webview/webview.h"

// Office license key structure
typedef struct
{
    const char *version;
    const char *product;
    const char *license_key;
    const char *key_type;
} office_license_key_t;

// Office activation functions
char *office_get_edition(void);
const char *office_get_license_key_for_product(const char *product_name);
int office_create_sppc_dll(const char *path);
int office_create_unattended_xml(void);

// WebView callback
void webview_office_activation_callback(const char *seq, const char *req, void *arg);

#endif // OHOOK_H
