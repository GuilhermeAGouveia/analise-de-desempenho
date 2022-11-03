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

double maximo(double a, double b)
{
    return a < b ? b : a;
}

int main(void)
{
    double tempo_decorrido = 0.0, tempo_simulacao;

    double intervalo_medio_chegada, tempo_medio_servico;

    double chegada, servico;

    double soma_tempo_chegada = 0.0, soma_tempo_servico = 0.0;

    unsigned long int fila = 0;

    unsigned long int maxFila = 0;

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
                soma_tempo_servico += servico - tempo_decorrido;
            }
            fila++;
            maxFila = maximo(maxFila, fila);
            chegada = tempo_decorrido + (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());
        }
        else
        {
            printf("Saída em %lF.\n", tempo_decorrido);
            fila--;
            if (fila)
            {
                servico = tempo_decorrido + (-1.0 / (1.0 / tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;

            }
        }
    }

    //Qual a proporção do tempo em que o servidor ficou ocupado?
    printf("Ocupação: %lF\n", soma_tempo_servico / maximo(tempo_decorrido, servico));
    printf("Max fila: %ld\n", maxFila);
    
    return 0;
}