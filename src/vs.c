#include <windows.h>
#include "common.h"

struct vsKeys
{
    char *product;
    char *key;
};

struct vsKeys vs_keys[] = {
    {"Visual Studio 2022 Professional", "TD244-P4NB7-YQ6XK-Y8MMM-YWV2J"},
    {"Visual Studio 2022 Enterprise", "VHF9H-NXBBB-638P6-6JHCY-88JWH"},
    {"Visual Studio 2019 Professional", "NYWVH-HT4XC-R2WYW-9Y3CM-X4V3Y"},
    {"Visual Studio 2019 Enterprise", "BF8Y8-GN2QH-T84XB-QVY3B-RC4DF"}};

int get_edition_index_vs(const char *edition)
{
    for (int i = 0; i < sizeof(vs_keys) / sizeof(vs_keys[0]); ++i)
    {
        if (strcmp(vs_keys[i].product, edition) == 0)
        {
            return i;
        }
    }
    return -1;
}

int activate_vs()
{
    // TODO: add edition selection
    const char *edition = "Visual Studio 2022 Professional";

    int edition_index = get_edition_index_vs(edition);
    if (edition_index == -1)
    {
        MessageBoxA(NULL, "Edition not found!", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    MessageBoxA(NULL, "Download Visual Studio", "Info", MB_OK | MB_ICONINFORMATION);
    download_file("https://aka.ms/vs/17/release/vs_professional.exe", "C:/Windows/Temp/vs_installer.exe");

    MessageBoxA(NULL, "Install and Activate", "Info", MB_OK | MB_ICONINFORMATION);
    char command[512];
    sprintf(command, "/c C:/Windows/Temp/vs_installer.exe --add-product --product-key %s --passive", vs_keys[edition_index].key);
    run_command(command);

    return 0;
}
