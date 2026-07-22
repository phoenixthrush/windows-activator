cmake_minimum_required(VERSION 3.16)
project(activator LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include(FetchContent)
include(ExternalProject)

# A fixed release keeps builds reproducible. The project uses the static C API.
FetchContent_Declare(
  webview
  GIT_REPOSITORY https://github.com/webview/webview
  GIT_TAG 0.12.0
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(webview)

set(SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../scripts")
set(OHOOK_DIR "${CMAKE_BINARY_DIR}/_deps/ohook")
set(OHOOK_SOURCE_DIR "${OHOOK_DIR}/src/ohook")
set(SPPC_SOURCE "${CMAKE_BINARY_DIR}/sppc64.dll.c")
set(OHOOK_BUILT_LOCALLY FALSE)

# Native Windows builds compile ohook before converting its DLL to a C array.
# Cross builds receive the same generated C file from scripts/deps.sh.
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  ExternalProject_Add(
    ohook
    GIT_REPOSITORY https://github.com/asdcorp/ohook
    GIT_TAG principalis
    PREFIX "${OHOOK_DIR}"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND
      COMMAND
        ${CMAKE_COMMAND} -E chdir "${OHOOK_SOURCE_DIR}"
        powershell -ExecutionPolicy Bypass -File "${SCRIPTS_DIR}/patch_ohook.ps1"
      COMMAND ${CMAKE_COMMAND} -E chdir "${OHOOK_SOURCE_DIR}" make sppc64.dll
    INSTALL_COMMAND "")

  add_custom_command(
    OUTPUT "${SPPC_SOURCE}"
    COMMAND
      python "${SCRIPTS_DIR}/binary_to_c.py"
      --input "${OHOOK_SOURCE_DIR}/sppc64.dll"
      --output "${SPPC_SOURCE}"
    DEPENDS ohook
    VERBATIM)

  set(OHOOK_BUILT_LOCALLY TRUE)
endif()

set(APP_SOURCES
  main.c
  common.c
  "${CMAKE_BINARY_DIR}/index.modified.c")

if(WIN32)
  list(APPEND APP_SOURCES
    hwid.c
    ohook.c
    vs.c
    resource.rc
    "${SPPC_SOURCE}")

  add_executable(activator WIN32 ${APP_SOURCES})
  target_link_libraries(activator PRIVATE webview::core_static urlmon wininet)

  if(MINGW)
    target_link_options(activator PRIVATE -static)
  endif()

  if(OHOOK_BUILT_LOCALLY)
    add_dependencies(activator ohook)
  endif()
else()
  # A non-Windows build is useful for previewing the WebView shell only.
  add_executable(activator ${APP_SOURCES})
  target_link_libraries(activator PRIVATE webview::core_static)
endif()
