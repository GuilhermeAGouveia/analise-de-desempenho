set tmargin 4
set bmargin 4
set rmargin 7
set lmargin 10

set terminal png size 1280,720 enhanced font "Helvetica, 14"
set output '/home/2020.1.08.035/Documentos/analise-de-desempenho/trabalhos/trabalho2/simulacao_web/png/e_n/e[n]99.png'
set title "Valor esperado de N | E[N]" font "Helvetica-Bold, 18"
set xlabel "Tempo" font "Helvetica-Bold, 14"
set ylabel "E[N]" font "Helvetica-Bold, 14"

set xtics 0, 2000, 36000 font "Helvetica, 12"
set ytics 0, 7, 140 font "Helvetica, 12"
#Espelhar eixo Y
set y2tics 0, 7, 140 font "Helvetica, 12" 

set key at 35000, 132

#set axis x, y 
set xrange [0:36000]
set yrange [0:140]
#Necessário para o espelhamento do eixo Y
set y2range [0:140]

plot "./csv/e_n/e_n0.99.dat" title 'Ocupação 99% ' \
                   with linespoints ls 4 lw 3 pt 0
     