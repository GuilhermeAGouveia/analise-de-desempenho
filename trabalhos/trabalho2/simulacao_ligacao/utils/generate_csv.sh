gcc -o sim simulacao_principal.c binaryheap.c -lm

for i in 0.6 0.8 0.95 0.99; do 
    a=$1
    mkdir ./csv/$1
    a=$a$i
    echo $i | ./sim >> ./csv/$1/$a.dat
done