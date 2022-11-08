#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

typedef struct little_
{
    unsigned long int no_eventos;
    double tempo_anterior;
    double soma_areas;
} little;

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

void inicia_little(little *l)
{
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}

int main(void)
{
    double tempo_decorrido = 0.0, tempo_simulacao;

    double intervalo_medio_chegada, tempo_medio_servico;

    double chegada, servico;

    double soma_tempo_chegada = 0.0, soma_tempo_servico = 0.0;

    unsigned long int fila = 0;

    unsigned long int maxFila = 0;

    // Little
    little e_n;
    little e_w_chegada;
    little e_w_saida;

    inicia_little(&e_n);
    inicia_little(&e_w_chegada);
    inicia_little(&e_w_saida);
    // Little - fim

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
    srand(sementeAleat); // inicia o rand

    // gerando o tempo de chegada da primeira requisicao
    chegada = (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());

    while (tempo_decorrido <= tempo_simulacao)
    {
        tempo_decorrido = !fila ? chegada : minimo(chegada, servico);

        if (tempo_decorrido == chegada)
        {
            //printf("Chegada em %lF.\n", tempo_decorrido);
            if (!fila)
            {
                servico = tempo_decorrido + (-1.0 / (1.0 / tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }
            fila++;
            maxFila = maximo(maxFila, fila);
            chegada = tempo_decorrido + (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());

            // Little
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos++;

            e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_chegada.no_eventos++;
            // Little - fim
        }
        else
        {
            //printf("Saída em %lF.\n", tempo_decorrido);
            fila--;
            if (fila)
            {
                servico = tempo_decorrido + (-1.0 / (1.0 / tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }
            // Little
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos--;

            e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;
            e_w_saida.tempo_anterior = tempo_decorrido;
            e_w_saida.no_eventos++;
            // Little - fim
        }
    }

    e_n.soma_areas += (tempo_simulacao - e_n.tempo_anterior) * e_n.no_eventos;
    e_w_chegada.soma_areas += (tempo_simulacao - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
    e_w_saida.soma_areas += (tempo_simulacao - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

    double e_n_final = e_n.soma_areas / tempo_simulacao;
    double e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;

    printf("\nN eventos saida = %lu\n", e_w_saida.no_eventos);
    printf("N eventos chegada = %lu\n", e_w_chegada.no_eventos);

    double lambda = e_w_chegada.no_eventos / tempo_decorrido;

    printf("E[N] = %lF\n", e_n_final);   
    printf("E[W] = %lF\n", e_w_final);   
    printf("lambda = %lF\n", lambda);   

    printf("Erro de little: %.20lF\n", e_n_final - e_w_final * lambda);

    // Qual a proporção do tempo em que o servidor ficou ocupado?
    printf("Ocupação: %lF\n", soma_tempo_servico / maximo(tempo_decorrido, servico));
    printf("Max fila: %ld\n", maxFila);

    return 0;
}