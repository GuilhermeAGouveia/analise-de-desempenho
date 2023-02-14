#Define os tamanhos das margens dos gráficos
set tmargin 4
set bmargin 4
set rmargin 7
set lmargin 10
set grid
show grid

#Define o tamanho do gráfico
set terminal png size 1280,720 enhanced font "Helvetica, 14"

#Define o caminho e o nome do arquivo de saída
set output '/home/2020.1.08.035/Documentos/analise-de-desempenho/trabalhos/trabalho2/simulacao_ligacao/png/e_w_tr/e_w_tr.png'

#Define o titulo e os nomes dos eixos no gráfico
set title "Valor esperado de W | E[W] para diferentes ocupações (Tempo Real)" font "Helvetica-Bold, 18"
set xlabel "Tempo (s)" font "Helvetica-Bold, 14"
set ylabel "E[W]" font "Helvetica-Bold, 14"

#Define a escala dos eixos
set xtics 0, 2000, 36000 font "Helvetica, 12"
set ytics 0, 0.02, 0.4 font "Helvetica, 12"
#Espelhar eixo Y
set y2tics 0, 0.02, 0.4 font "Helvetica, 12" 

#Define a posição e legenda do gráfico
set key at 35000, 0.38 font "Helvetica, 12"

#set axis x, y 
set xrange [0:36000]
set yrange [0:0.4]

#Necessário para o espelhamento do eixo Y
set y2range [0:0.4]

#Plota o gráfico pegando os dados de vários arquivos
plot "./csv/e_w_tr/e_w_tr0.99.dat" title 'Ocupação 99% ' \
                   with linespoints ls 4 lw 3 pt 0, \
     "./csv/e_w_tr/e_w_tr0.95.dat" title 'Ocupação 95% ' \
                   with linespoints ls 3 lw 3 pt 0, \
     "./csv/e_w_tr/e_w_tr0.8.dat" title 'Ocupação 80% ' \
                   with linespoints ls 2 lw 3 pt 0, \
     "./csv/e_w_tr/e_w_tr0.6.dat" title 'Ocupação 60% ' \
                   with linespoints ls 1 lw 3 pt 0
     
     