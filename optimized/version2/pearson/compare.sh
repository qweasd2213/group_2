#!/usr/bin/env bash
set -euo pipefail

START=${1:-1}
END=${2:-5}

for ((n = START + 1; n <= END; n++)); do
    prev=$((n - 1))
    echo "Comparing version${prev} -> version${n}"

    old_dir=$(find . -maxdepth 1 -type d \
        -regextype posix-extended \
        -regex "./v${prev}_.*" -print -quit)

    new_dir=$(find . -maxdepth 1 -type d \
        -regextype posix-extended \
        -regex "./v${n}_.*" -print -quit)

    if [[ -z "$old_dir" || -z "$new_dir" ]]; then
        echo "Could not find both version directories." >&2
        exit 1
    fi

    for f in analysis.cpp analysis.hpp dataset.cpp dataset.hpp \
        vector.cpp vector.hpp pearson.cpp Makefile verify.c; do
        old="$old_dir/$f"
        new="$new_dir/$f"

        if [[ -f "$old" && -f "$new" ]]; then
            diff -u "$old" "$new" || [[ $? -eq 1 ]]
        elif [[ -f "$old" ]]; then
            echo "Only in ${old_dir}: $f"
        elif [[ -f "$new" ]]; then
            echo "Only in ${new_dir}: $f"
        fi
    done
done
