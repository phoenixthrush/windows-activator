#ifndef HWID_H
#define HWID_H

#include <windows.h>
#include <stdio.h>

char *get_edition_id();
char *get_license_key(const char *edition_id);
char *get_os_product_pfn();
int is_admin();
void run_command(const char *command);
void set_compatibility_mode(const char *exe_path);

#endif // HWID_H
