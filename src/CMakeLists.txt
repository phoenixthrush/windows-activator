cmake_minimum_required(VERSION 3.16)
project(activator LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include(FetchContent)
include(ExternalProject)

FetchContent_Declare(
    webview
    GIT_REPOSITORY https://github.com/webview/webview
    GIT_TAG master)
FetchContent_MakeAvailable(webview)
 
#message(STATUS "Downloading MinGW-w64 winlibs, this will take a moment...")

#ExternalProject_Add(
#    winlibs_x86_64
#    URL https://github.com/brechtsanders/winlibs_mingw/releases/download/14.2.0posix-19.1.7-12.0.0-msvcrt-r3/winlibs-x86_64-posix-seh-gcc-14.2.0-mingw-w64msvcrt-12.0.0-r3.zip
#    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/_deps/winlibs
#    SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/winlibs/x86_64
#    CONFIGURE_COMMAND ""
#    BUILD_COMMAND ""
#    INSTALL_COMMAND ${CMAKE_COMMAND} -E unzip ${CMAKE_BINARY_DIR}/_deps/winlibs/winlibs-x86_64-posix-seh-gcc-14.2.0-mingw-w64msvcrt-12.0.0-r3.zip
#)

#ExternalProject_Add(
#    winlibs_i686
#    URL https://github.com/brechtsanders/winlibs_mingw/releases/download/14.2.0posix-19.1.7-12.0.0-msvcrt-r3/winlibs-i686-posix-dwarf-gcc-14.2.0-mingw-w64msvcrt-12.0.0-r3.zip
#    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/_deps/winlibs
#    SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/winlibs/i686
#    CONFIGURE_COMMAND ""
#    BUILD_COMMAND ""
#    INSTALL_COMMAND ${CMAKE_COMMAND} -E unzip ${CMAKE_BINARY_DIR}/_deps/winlibs/winlibs-i686-posix-dwarf-gcc-14.2.0-mingw-w64msvcrt-12.0.0-r3.zip
#)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    ExternalProject_Add(
        ohook
        GIT_REPOSITORY https://github.com/asdcorp/ohook
        GIT_TAG principalis
        PREFIX ${CMAKE_BINARY_DIR}/_deps/ohook
        CONFIGURE_COMMAND ""
        BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/_deps/ohook/src/ohook/ && powershell -EncodedCommand JABtAGkAbgBnAHcAMwAyAFAAYQB0AGgAIAA9ACAAUwBwAGwAaQB0AC0AUABhAHQAaAAgACgARwBlAHQALQBDAG8AbQBtAGEAbgBkACAAZwBjAGMAKQAuAFMAbwB1AHIAYwBlAAoAJABtAGkAbgBnAHcAMwAyAFAAYQB0AGgAIAA9ACAAJABtAGkAbgBnAHcAMwAyAFAAYQB0AGgAIAArACAAJwBcACcACgAkAG0AaQBuAGcAdwAzADIAUABhAHQAaAAgAD0AIAAkAG0AaQBuAGcAdwAzADIAUABhAHQAaAAgAC0AcgBlAHAAbABhAGMAZQAgACcAXABcACcALAAgACcALwAnAAoACgAkAG0AaQBuAGcAdwA2ADQAUABhAHQAaAAgAD0AIABTAHAAbABpAHQALQBQAGEAdABoACAAKABHAGUAdAAtAEMAbwBtAG0AYQBuAGQAIABnAGMAYwApAC4AUwBvAHUAcgBjAGUACgAkAG0AaQBuAGcAdwA2ADQAUABhAHQAaAAgAD0AIAAkAG0AaQBuAGcAdwA2ADQAUABhAHQAaAAgACsAIAAnAFwAJwAKACQAbQBpAG4AZwB3ADYANABQAGEAdABoACAAPQAgACQAbQBpAG4AZwB3ADYANABQAGEAdABoACAALQByAGUAcABsAGEAYwBlACAAJwBcAFwAJwAsACAAJwAvACcACgAKACgARwBlAHQALQBDAG8AbgB0AGUAbgB0ACAATQBhAGsAZQBmAGkAbABlACkAIAAtAHIAZQBwAGwAYQBjAGUAIAAiAEMAOgAvAG0AaQBuAGcAdwAzADIALwBiAGkAbgAvACIALAAgACQAbQBpAG4AZwB3ADMAMgBQAGEAdABoACAALQByAGUAcABsAGEAYwBlACAAIgBDADoALwBtAGkAbgBnAHcANgA0AC8AYgBpAG4ALwAiACwAIAAkAG0AaQBuAGcAdwA2ADQAUABhAHQAaAAgAHwAIABTAGUAdAAtAEMAbwBuAHQAZQBuAHQAIABNAGEAawBlAGYAaQBsAGUACgAoAEcAZQB0AC0AQwBvAG4AdABlAG4AdAAgAE0AYQBrAGUAZgBpAGwAZQApACAAfAAgAFcAaABlAHIAZQAtAE8AYgBqAGUAYwB0ACAAewAkAF8AIAAtAG4AbwB0AG0AYQB0AGMAaAAgACcAXgBcAHMAKgBzAHQAcgBpAHAAIAAtAHMAJwB9ACAAfAAgAFMAZQB0AC0AQwBvAG4AdABlAG4AdAAgAE0AYQBrAGUAZgBpAGwAZQA= && make sppc64.dll
        INSTALL_COMMAND ""
    )

    set(GENERATED_FILE ${CMAKE_BINARY_DIR}/sppc64.dll.c)

    add_custom_command(
        OUTPUT ${GENERATED_FILE}
        COMMAND python ${CMAKE_SOURCE_DIR}/../src/helpers/xxd.py 
                -i ${CMAKE_BINARY_DIR}/_deps/ohook/src/ohook/sppc64.dll 
                -o ${GENERATED_FILE}
        DEPENDS ohook
    )

    set(OHOOOK_BUILT TRUE)
else()
    set(GENERATED_FILE ${CMAKE_BINARY_DIR}/sppc64.dll.c)
    set(OHOOOK_BUILT FALSE)
endif()

if (MINGW OR WIN32)
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -static")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -static")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -static")

    add_executable(activator WIN32
        main.c
        ${CMAKE_BINARY_DIR}/index.modified.c
        common.c
        hwid.c
        ohook.c
        vs.c
        vmware.c
        ${GENERATED_FILE}
        resource.rc
    )

    if (OHOOOK_BUILT)
        add_dependencies(activator ohook)
    endif()

    target_link_libraries(activator PRIVATE webview::core_static urlmon)
else()
    add_executable(activator
        main.c
        ${CMAKE_BINARY_DIR}/index.modified.c
        common.c
    )
    target_link_libraries(activator PRIVATE webview::core_static)
endif()