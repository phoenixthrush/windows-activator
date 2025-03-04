// this will be optimised and rewritten

#ifndef OHOOK_H
#define OHOOK_H

#include <windows.h>
#include <stdio.h>
#include "webview/webview.h"
#include "common.h"

char *get_office_edition();
char *getLicenseKey(char *productName);
int create_sppc(const char *path);
int create_unattended_office_xml();
void ohook_cb(const char *seq, const char *req, void *arg);

#endif // OHOOK_H
