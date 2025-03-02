#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include "webview/webview.h"

void credits_cb(const char *seq, const char *req, void *arg);
void quit_cb(const char *seq, const char *req, void *arg);

#ifdef _WIN32
#include <windows.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

int is_admin();
void run_command(const char *command);
int download_file(const char *url, const char *destination);
int extract_tar(const char *tar_file, const char *target_dir);
#endif

#endif // COMMON_H
