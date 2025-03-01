#!/bin/env bash

cross_build="build/bin/Release/activator.exe"
build="build/bin/activator"

if [ -f "$cross_build" ] || [ -f "$build" ]; then
    echo "Activator found, compressing with UPX..."

    if which upx &>/dev/null; then
        if [ -f "$cross_build" ]; then
            upx --best "$cross_build"
        elif [ -f "$build" ]; then
            if [[ "$(uname)" == "Darwin" ]]; then
                echo "Skipping compression on macOS, only cross builds are compressed."
            else
                upx --best "$build"
            fi
        fi
    else
        echo "UPX is not installed."
    fi
else
    echo "Activator not found."
fi
