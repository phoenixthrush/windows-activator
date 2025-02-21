#ifndef ACTIVATE_H
#define ACTIVATE_H

#include <windows.h>
#include <stdio.h>

int is_admin();
char *get_edition_id();
char *get_license_key(const char *edition_id);
void activate_key(const char *key);
char *get_os_product_pfn();
void run_command(const char *command);

#endif // ACTIVATE_H
