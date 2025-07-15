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

set(OHOOK_BUILT FALSE)
set(SPPC64_DLL_C "${CMAKE_BINARY_DIR}/sppc64.dll.c")
set(OHOOK_DIR "${CMAKE_BINARY_DIR}/_deps/ohook")
set(OHOOK_SRC_DIR "${OHOOK_DIR}/src/ohook")
set(HELPERS_DIR "${CMAKE_SOURCE_DIR}/../src/helpers")

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  ExternalProject_Add(
    ohook
    GIT_REPOSITORY https://github.com/asdcorp/ohook
    GIT_TAG principalis
    PREFIX "${OHOOK_DIR}"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND
      COMMAND ${CMAKE_COMMAND} -E chdir "${OHOOK_SRC_DIR}" powershell -ExecutionPolicy Bypass -File "${HELPERS_DIR}/patch.ps1"
      COMMAND ${CMAKE_COMMAND} -E chdir "${OHOOK_SRC_DIR}" make sppc64.dll
    INSTALL_COMMAND "")
  add_custom_command(
    OUTPUT "${SPPC64_DLL_C}"
    COMMAND python "${HELPERS_DIR}/xxd.py" -i "${OHOOK_SRC_DIR}/sppc64.dll" -o "${SPPC64_DLL_C}"
    DEPENDS ohook)
  set(OHOOK_BUILT TRUE)
endif()

if(MINGW OR WIN32)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static")
  add_executable(activator WIN32
    main.c
    common.c
    hwid.c
    ohook.c
    vs.c
    vmware.c
    resource.rc
    "${CMAKE_BINARY_DIR}/index.modified.c"
    "${SPPC64_DLL_C}")
  if(OHOOK_BUILT)
    add_dependencies(activator ohook)
  endif()
  target_link_libraries(activator PRIVATE webview::core_static urlmon wininet)
else()
  add_executable(activator
    main.c
    common.c
    "${CMAKE_BINARY_DIR}/index.modified.c"
    )
  target_link_libraries(activator PRIVATE webview::core_static)
endif()
