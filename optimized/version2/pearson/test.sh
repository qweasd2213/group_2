#!/usr/bin/env bash
set -euo pipefail

PERF_PROFILE="/home/gabzz/local/courses/dv1674/assignment_1/perf_profiler/scripts/bench.sh"

START=${1:-0}
END=${1:-6}

mkdir -p ./tmp/metric

for ((n = START; n <= END; n++)); do
    version="version${n}"

    dir=$(find . -maxdepth 1 -type d \
        -regextype posix-extended \
        -regex "./v${n}_.*" -print -quit)

    if [[ ! -d "$dir" ]]; then
        continue
    fi

    output="$dir/results/metric"
    rm -rf "$output"
    echo "Profiling ${version}..."

    mkdir -p "$output"

    bash "$PERF_PROFILE" metric \
        --command "${dir}/./pearson_opt {file} {output}" \
        --output "$output" \
        --max-threads 1 \
        --runs 3 \
        "${dir}/data/"
done

echo "Plotting metrics..."
bash "$PERF_PROFILE" plot ./tmp/metric

echo "Done!"
