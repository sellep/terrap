#!/usr/bin/gnuplot --persist

set timefmt '%H:%M:%S'
set xdata time
set datafile sep '\t'
set style data lines
set terminal dumb

plot "/tmp/hygro" using 1:2 title 'humidity', '' using 1:3 title 'temperature'
