#ifndef VMWARE_H
#define VMWARE_H

#include <windows.h>
#include "webview/webview.h"

// VMware license key structure
typedef struct
{
    const char *product_name;
    const char *license_key;
} vmware_license_key_t;

// VMware activation functions
int vmware_find_edition_index(const char *edition);

// WebView callback
void webview_vmware_activation_callback(const char *seq, const char *req, void *arg);

#endif // VMWARE_H
