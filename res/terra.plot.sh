#!/usr/bin/gnuplot --persist

set xdata time
set timefmt '%H:%M:%S'
set format x '%H:%M'
set datafile sep '\t'
set style data lines
set terminal dumb

plot "/tmp/hygro" using 1:3 title 'temp'

#, '' using 1:3 title 'temperature'
