set tmargin 4
set bmargin 4
set rmargin 4
set lmargin 10

set terminal png size 1280,720 enhanced font "Helvetica, 14"
set output '/home/2020.1.08.035/Documentos/analise-de-desempenho/trabalhos/trabalho2/simulacao_web/csv/e_n/e[n].png'
set title "Valor esperado de N | E[N]" font "Helvetica-Bold, 18"
set xlabel "Tempo" font "Helvetica-Bold, 14"
set ylabel "E[N]" font "Helvetica-Bold, 14"

set xtics 0, 2000, 36000 font "Helvetica, 12"
set ytics -50, 1, 50 font "Helvetica, 12"

set key at 35000, 22.5

#set axis x, y 
#set xrange [0:36000]
set yrange [0:24]

plot "./e_n/e_n0.95.dat" title 'Ocupação 95% ' \
                   with linespoints ls 3 lw 3 pt 0, \
     "./e_n/e_n0.8.dat" title 'Ocupação 80% ' \
                   with linespoints ls 2 lw 3 pt 0, \
     "./e_n/e_n0.6.dat" title 'Ocupação 60% ' \
                   with linespoints ls 1 lw 3 pt 0
     