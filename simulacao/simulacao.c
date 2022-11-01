#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

double aleatorio()
{
    double u = rand() / ((double)RAND_MAX + 1);
    // limitando entre (0,1]
    u = 1.0 - u;

    return (u);
}

double minimo(double a, double b)
{
    return a > b ? b : a;
}

int main(void)
{
    double tempo_decorrido = 0.0, tempo_simulacao;

    double intervalo_medio_chegada, tempo_medio_servico;

    double chegada, servico;

    unsigned long int fila = 0;

    printf("Informe o tempo de simulacao (seg): ");
    scanf("%lF", &tempo_simulacao);

    printf("Informe o intervalo médio entre chegadas (seg): ");
    scanf("%lF", &intervalo_medio_chegada);

    printf("Informe o tempo médio de serviço (seg): ");
    scanf("%lF", &tempo_medio_servico);

    /***************************
	 *  Iniciando semente      *
	 ***************************/

	int sementeAleat;
	sementeAleat = time(NULL);
	srand(sementeAleat); //inicia o rand


    // gerando o tempo de chegada da primeira requisicao
    chegada = (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());

    while (tempo_decorrido <= tempo_simulacao)
    {
        tempo_decorrido = !fila ? chegada : minimo(chegada, servico);

        if (tempo_decorrido == chegada)
        {
            printf("Chegada em %lF.\n", tempo_decorrido);
            if (!fila)
            {
                servico = tempo_decorrido + (-1.0 / (1.0 / tempo_medio_servico)) * log(aleatorio());
            }
            fila++;
            chegada = tempo_decorrido + (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());
        }
        else
        {
            printf("Saída em %lF.\n", tempo_decorrido);
            fila--;
            if (fila)
            {
                servico = tempo_decorrido + (-1.0 / (1.0 / tempo_medio_servico)) * log(aleatorio());
            }
        }
    }

    return 0;
}