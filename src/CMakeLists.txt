cmake_minimum_required(VERSION 3.16)
project(activator LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include(FetchContent)

FetchContent_Declare(
    webview
    GIT_REPOSITORY https://github.com/webview/webview
    GIT_TAG master)
FetchContent_MakeAvailable(webview)

if (MINGW OR WIN32)
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -static")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -static")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -static")
    add_executable(activator WIN32)
    target_sources(activator PRIVATE main.c ${CMAKE_BINARY_DIR}/index.modified.c common.c hwid.c ohook.c resource.rc)
else()
    add_executable(activator)
    target_sources(activator PRIVATE main.c ${CMAKE_BINARY_DIR}/index.modified.c)
endif()

target_link_libraries(activator PRIVATE webview::core_static)
