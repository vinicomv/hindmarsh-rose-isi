set terminal postscript enhanced color eps
set output "phase_diagram_min.eps"
set xlabel "b" font ",30" offset 0,1.3,0
set ylabel "I_{e}" font ",30" offset 1,0,0
set xrange [1.7:1.95]
set yrange [5.22:5.255]
set pointsize 0.2
unset key
set cbrange[0:120]
set cblabel 'ISI_{min}' font ",20" offset "1,0,0"

set palette defined (0 "blue", 10 "dark-blue", 10 "white", 20 "black", 20 "dark-violet", 120 "yellow" )
#------------------------------------------------------------------------------------------------------
# Here are some command options to add a colored vertical bar as the one in Figure 4A. They are adapted to that resolution. Uncomment and change the coordinates in order to add them as you fit.
#------------------------------------------------------------------------------------------------------
#CS
#set arrow 1 front from 1.35,5.22000 to 1.35,5.22815 nohead dashtype 1 lc 'dark-green' lw 4.5

#EAD
#set arrow 2 front from 1.35,5.22815 to 1.35,5.23780 nohead dashtype 1 lc 'red' lw 4.5

#Big U (inside periodic EAD)
#set arrow 3 front from 1.35,5.23780 to 1.35,5.24560 nohead dashtype 1 lc 'dark-red' lw 4.5

#EAD
#set arrow 4 front from 1.35,5.24560 to 1.35,5.26385 nohead dashtype 1 lc 'red' lw 4.5

#Chaotic LAO
#set arrow 5 front from 1.35,5.26385 to 1.35,5.28510 nohead dashtype 1 lc 'orange' lw 4.5

#Period-2n LAO
#set arrow 6 front from 1.35,5.28510 to 1.35,5.29730 nohead dashtype 1 lc 'cyan' lw 4.5

#Period-1 LAO
#set arrow 7 front from 1.35,5.29730 to 1.35,5.31000 nohead dashtype 1 lc 'green' lw 4.5

plot "prl_min.dat" with points pointtype 7 palette

set output "phase_diagram_max.eps"
set title "Maximum ISI phase diagram" font ",24" offset 0,-1.2,0
set cblabel 'ISI_{max}' font ",20" offset 1,0,0

plot "plt_max.dat" with points pt 7 palette

