#ifndef OHOOK_H
#define OHOOK_H

#include "webview/webview.h"

// The implementation details are intentionally private to ohook.c.
void webview_office_activation_callback(const char *seq, const char *req, void *arg);

#endif // OHOOK_H
