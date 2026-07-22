#!/usr/bin/env bash
set -euo pipefail

readonly AUDIO_PATH="site/assets/audio/keygen-Uh-p3TOIrOc.mp3"
readonly OHOOK_DIRECTORY="build/_deps/ohook/src/ohook"
readonly SPPC_SOURCE="build/sppc64.dll.c"

python_command() {
    # Most Unix systems call Python 3 "python3"; a few still use "python".
    if command -v python3 >/dev/null 2>&1; then
        printf 'python3'
    else
        printf 'python'
    fi
}

check_dependencies() {
    echo "Checking build dependencies..."

    local dependencies=(
        make ffmpeg cmake ninja gcc yt-dlp curl unzip
    )
    local missing=0

    for dependency in "${dependencies[@]}"; do
        if ! command -v "$dependency" >/dev/null 2>&1; then
            echo "Missing dependency: $dependency"
            missing=1
        fi
    done

    if ! command -v "$(python_command)" >/dev/null 2>&1; then
        echo "Missing dependency: Python 3"
        missing=1
    fi

    if ((missing)); then
        exit 1
    fi
}

fetch_audio() {
    if [[ -f "$AUDIO_PATH" ]]; then
        echo "Audio asset already exists."
        return
    fi

    echo "Downloading the fallback audio asset..."
    mkdir -p "$(dirname "$AUDIO_PATH")"
    yt-dlp \
        --format bestaudio \
        --extract-audio \
        --audio-format mp3 \
        --output "$AUDIO_PATH" \
        "https://www.youtube.com/watch?v=tPY-I3RX10c"
}

fetch_sppc_source() {
    if [[ -f "$SPPC_SOURCE" ]]; then
        echo "Generated sppc source already exists."
        return
    fi

    echo "Downloading the ohook build asset..."
    mkdir -p "$OHOOK_DIRECTORY"
    curl --fail --location \
        --output build/ohook.zip \
        "https://github.com/asdcorp/ohook/releases/download/0.5/ohook_0.5.zip"
    unzip -o build/ohook.zip -d "$OHOOK_DIRECTORY"
    "$(python_command)" scripts/binary_to_c.py \
        --input "$OHOOK_DIRECTORY/sppc64.dll" \
        --output "$SPPC_SOURCE"
}

check_dependencies
fetch_audio
fetch_sppc_source
