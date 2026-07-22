#ifndef HWID_H
#define HWID_H

#include "webview/webview.h"

// This callback is the module's only public entry point. Its helper functions
// stay private to hwid.c so the rest of the application has a small API.
void webview_windows_activation_callback(const char *seq, const char *req, void *arg);

#endif // HWID_H
