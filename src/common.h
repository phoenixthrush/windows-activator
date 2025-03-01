#ifndef COMMON_H
#define COMMON_H

#include <windows.h>
#include <stdio.h>
#include <urlmon.h>

#pragma comment(lib, "urlmon.lib")

int is_admin();
void run_command(const char *command);
int download_file(const char *url, const char *destination);
int extract_tar(const char *tar_file, const char *target_dir);

#endif // COMMON_H
