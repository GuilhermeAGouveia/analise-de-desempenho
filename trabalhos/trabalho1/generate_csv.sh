gcc generate_seq.c -o generate_seq 
gcc simulacao_little_event_based.c -o sim -lm
./generate_seq 360 > $1.csv

for i in 0.8 0.9 0.95 0.99; do echo $i | ./sim; done >> $1.csv