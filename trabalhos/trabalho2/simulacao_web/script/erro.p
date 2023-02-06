set tmargin 4
set bmargin 4
set rmargin 7
set lmargin 15

set terminal png size 1280,720 enhanced font "Helvetica, 14"
set output '/home/2020.1.08.035/Documentos/analise-de-desempenho/trabalhos/trabalho2/simulacao_web/png/erro/erro.png'
set title "Erro de Little" font "Helvetica-Bold, 18"
set xlabel "Tempo" font "Helvetica-Bold, 14"
set ylabel "Erro" font "Helvetica-Bold, 14"

set xtics 0, 2000, 36000 font "Helvetica, 12"
set ytics -0.00000014, 0.00000002, 0.00000014 font "Helvetica, 12"

set key at 35000, 1.9 font "Helvetica, 12"

#set axis x, y 
set xrange [0:36000]
set yrange [-0.00000014:0.00000014]
#Necessário para o espelhamento do eixo Y

plot "./csv/erro/erro0.99.dat" title 'Ocupação 99% ' \
                   with linespoints ls 4 lw 3 pt 0, \
     "./csv/erro/erro0.95.dat" title 'Ocupação 95% ' \
                   with linespoints ls 3 lw 3 pt 0, \
     "./csv/erro/erro0.8.dat" title 'Ocupação 80% ' \
                   with linespoints ls 2 lw 3 pt 0, \
     "./csv/erro/erro0.6.dat" title 'Ocupação 60% ' \
                   with linespoints ls 1 lw 3 pt 0
     
     