#ifndef COMMON_H
#define COMMON_H

#include <windows.h>
#include <shlobj.h>
#include <stdio.h>

int is_admin();
int request_admin();
void run_command(const char *command);

#endif // COMMON_H
