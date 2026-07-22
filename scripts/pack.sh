#!/usr/bin/env bash
set -euo pipefail

readonly NATIVE_BINARY="build/bin/activator"
readonly CROSS_BINARY="build/bin/Release/activator.exe"

remove_unused_library() {
    local library="build/bin/Release/libwebview.dll"
    [[ ! -f "$library" ]] || rm "$library"
}

next_output_name() {
    local directory="$1"
    local extension="$2"
    local commit
    local number=0

    # Exported source archives have no .git directory, so use a clear fallback.
    commit="$(git rev-parse --short HEAD 2>/dev/null || printf 'local')"

    while [[ -e "$directory/activator.$commit-$number$extension" ]]; do
        number=$((number + 1))
    done

    printf '%s/activator.%s-%s%s' "$directory" "$commit" "$number" "$extension"
}

pack_cross_binary() {
    upx --best "$CROSS_BINARY"
    mv "$CROSS_BINARY" "$(next_output_name "build/bin/Release" ".exe")"
}

pack_native_binary() {
    if [[ "$(uname)" == "Darwin" ]]; then
        echo "Skipping UPX for the macOS preview build."
        return
    fi

    # Keep the native preview at its usual path so `make run` still works.
    upx --best "$NATIVE_BINARY"
}

remove_unused_library

if ! command -v upx >/dev/null 2>&1; then
    echo "UPX is not installed; leaving the binary uncompressed."
elif [[ -f "$CROSS_BINARY" ]]; then
    pack_cross_binary
elif [[ -f "$NATIVE_BINARY" ]]; then
    pack_native_binary
else
    echo "No activator binary was found."
fi
