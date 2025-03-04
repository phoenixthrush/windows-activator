// this will be optimised and rewritten

#ifndef HWID_H
#define HWID_H

#include <windows.h>
#include <stdio.h>
#include "webview/webview.h"

char *get_edition_id();
char *get_license_key(const char *edition_id);
char *get_os_product_pfn();
void set_compatibility_mode(const char *exe_path);
void activate_cb(const char *seq, const char *req, void *arg);

#endif // HWID_H
