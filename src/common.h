#ifndef COMMON_H
#define COMMON_H

#include "webview/webview.h"

// Shared callbacks used by the application shell.
void webview_quit_callback(const char *seq, const char *req, void *arg);
void webview_open_browser_callback(const char *seq, const char *req, void *arg);

#ifdef _WIN32
// Windows helpers shared by the individual product modules.
int system_check_admin_privileges(void);
int system_check_internet_connectivity(void);
void system_execute_command_elevated(const char *command);
int system_download_file(const char *url, const char *destination);
int system_extract_tar_archive(const char *tar_file, const char *target_dir);
int system_patch_gatherosstate_file(const char *input_file, const char *output_file);
int system_delete_directory_recursive(const char *path);
#endif

#endif
