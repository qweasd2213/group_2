#!/bin/bash
set -euo pipefail

cd "$(dirname "$(realpath "$0")")"

RADIUS=15
CSV="blur_metrics.csv"
TMP=$(mktemp)
trap 'rm -f "$TMP"' EXIT

if [[ ! -x ./blur ]]; then
    echo "Error: ./blur not found or not executable" >&2
    exit 1
fi

command -v gnuplot >/dev/null || {
    echo "Error: Install gnuplot with sudo pacman -S gnuplot" >&2
    exit 1
}

echo "Image,Execution_time_s,CPU_percent,Max_RSS_kB" > "$CSV"

for image in im1 im2 im3 im4; do
    input="data/${image}.ppm"

    [[ -f "$input" ]] || {
        echo "Error: $input not found" >&2
        exit 1
    }

    echo "Benchmarking $input..."

    LC_ALL=C /usr/bin/time -f '%e %P %M' -o "$TMP" \
        ./blur "$RADIUS" "$input" out.ppm

    read -r elapsed cpu rss < "$TMP"
    cpu="${cpu%\%}"

    echo "${image},${elapsed},${cpu},${rss}" >> "$CSV"
    echo "Time: ${elapsed}s | CPU: ${cpu}% | Max RSS: ${rss} KiB"
done

gnuplot <<'GNUPLOT'
set terminal pngcairo size 1800,500 enhanced font "sans,14"
set output "blur_benchmark.png"

set datafile separator ","
set grid ytics
set key off
set xtics rotate by -35
set lmargin 9
set rmargin 4

set multiplot layout 1,3 title "Blur Performance Benchmark" font ",18"

set title "Execution Time" font ",16"
set ylabel "Time (s)"
set xlabel "Input image"
plot "blur_metrics.csv" every ::1 using 2:xtic(1) with linespoints pointtype 7 pointsize 1.2 linewidth 2

set title "Maximum RSS" font ",16"
set ylabel "Memory (KiB)"
set xlabel "Input image"
set yrange [0:*]
plot "blur_metrics.csv" every ::1 using 4:xtic(1) with linespoints pointtype 7 pointsize 1.2 linewidth 2

set title "CPU Utilization" font ",16"
set ylabel "CPU (%)"
set xlabel "Input image"
set yrange [0:*]
plot "blur_metrics.csv" every ::1 using 3:xtic(1) with linespoints pointtype 7 pointsize 1.2 linewidth 2

unset multiplot
GNUPLOT

echo "Done. Results: $CSV"
echo "Generated: execution_time.png, cpu_percent.png, max_rss.png"
