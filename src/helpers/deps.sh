#!/bin/env bash
check_dependencies() {
    echo "Verifying that all dependencies are installed"

    # todo add missing
    dependencies=(
        "make"
        "cmake"
        "yt-dlp"
    )

    for dep in "${dependencies[@]}"; do
        if ! which "$dep" &>/dev/null; then
            echo "Error: $dep is not installed."
            exit 1
        fi
    done

    echo "All needed dependencies found"
}

fetch_audio() {
    audioFilePath="site/assets/audio/keygen-Uh-p3TOIrOc.mp3"

    if [ ! -f "$audioFilePath" ]; then
        echo "Audio file not found. Downloading..."
        yt-dlp -f bestaudio --extract-audio --audio-quality 64K --audio-format mp3 -o "$audioFilePath" "https://www.youtube.com/watch?v=tPY-I3RX10c"
    else
        echo "Audio file already exists."
    fi
}

fetch_sppc64() {
    curl -Lo build/ohook.zip https://github.com/asdcorp/ohook/releases/download/0.5/ohook_0.5.zip
    unzip -o build/ohook.zip -d build/_deps/ohook/src/ohook/
    python src/helpers/xxd.py -i build/_deps/ohook/src/ohook/sppc64.dll -o build/sppc64.dll.c
}

check_dependencies
fetch_audio
fetch_sppc64
