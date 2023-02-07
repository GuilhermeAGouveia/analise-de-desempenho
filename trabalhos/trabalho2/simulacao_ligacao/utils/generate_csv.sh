gcc simulacao_principal.c -o sim -lm

for i in 0.8 0.9 0.95 0.99; do echo $i | ./sim; done >> ./csv/$1_$i.dat