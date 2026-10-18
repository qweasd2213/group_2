#!/usr/bin/env bash
set -euo pipefail

for n in {0..6}; do

    dir=$(find . -maxdepth 1 -type d \
        -regextype posix-extended \
        -regex "./v${n}_.*" -print -quit)
    if [[ ! -d "$dir" ]]; then
        echo "Skipping $dir: directory not found"
        continue
    fi

    echo "Building $dir..."

    if ! make -C "$dir"; then
        echo "Build failed in $dir"
        exit 1
    fi
done

echo "All builds completed successfully."
