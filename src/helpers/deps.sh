#!/bin/env bash
check_dependencies() {
    echo "Verifying that all dependencies are installed"

    dependencies=(
        "make"
        "cmake"
    )

    for dep in "${dependencies[@]}"; do
        if ! command -v "$dep" &>/dev/null; then
            echo "Error: $dep is not installed."
            exit 1
        fi
    done

    echo "All needed dependencies found"
}

check_dependencies
