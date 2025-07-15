#ifndef VS_H
#define VS_H

#include <windows.h>
#include "webview/webview.h"

// Visual Studio license key structure
typedef struct
{
    const char *product_name;
    const char *license_key;
} visual_studio_license_key_t;

// Visual Studio activation functions
int visual_studio_find_edition_index(const char *edition);

// WebView callback
void webview_visual_studio_activation_callback(const char *seq, const char *req, void *arg);

#endif // VS_H
