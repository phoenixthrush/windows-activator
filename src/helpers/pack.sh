#!/bin/env bash

cross_build="build/bin/Release/activator.exe"
build="build/bin/activator"
build_number=0
commit_hash=$(git rev-parse --short HEAD)

if [ -f "build/bin/Release/libwebview.dll" ]; then
    rm "build/bin/Release/libwebview.dll"
fi

if [ -f "$cross_build" ] || [ -f "$build" ]; then
    echo "Activator found, compressing with UPX..."

    if which upx &>/dev/null; then
        if [ -f "$cross_build" ]; then
            upx --best "$cross_build"
            while [ -f "build/bin/Release/activator.$commit_hash-$build_number.exe" ]; do
                build_number=$((build_number + 1))
            done
            mv "$cross_build" "build/bin/Release/activator.$commit_hash-$build_number.exe"
        elif [ -f "$build" ]; then
            if [[ "$(uname)" == "Darwin" ]]; then
                echo "Skipping compression on macOS, only cross builds are compressed."
            else
                upx --best "$build"
                while [ -f "build/bin/activator.$commit_hash-$build_number" ]; do
                    build_number=$((build_number + 1))
                done
            fi
        fi
    else
        echo "UPX is not installed."
    fi
else
    echo "Activator not found."
fi
