#ifndef VS_H
#define VS_H

#include "webview/webview.h"

// The UI reaches this module through one well-defined callback.
void webview_visual_studio_activation_callback(const char *seq, const char *req, void *arg);

#endif // VS_H
