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

add_executable(activator WIN32)
target_sources(activator PRIVATE main.c)

if(WIN32)
    target_sources(activator PRIVATE activate.c)
    target_link_libraries(activator PRIVATE urlmon)
endif()

target_link_libraries(activator PRIVATE webview::core_static)
