#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "binaryheap.h"

// Escolha apenas uma das macros abaixo para definir o tipo de medida que será exibida
#define OCUPACAO(x)
#define E_N(x)
#define E_W(x)
#define ERRO_LITTLE(x)
#define VALORES_FINAIS(x) x

static int no_pacotes_web = 0;
static int no_pacotes_ligacao = 0;

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

double maximo(double num1, double num2)
{
    if (num1 > num2)
    {
        return num1;
    }
    return num2;
}

void inicia_little(little *l)
{
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}

int gera_pacote_ligacao()
{
    return 1280;
}

int gera_pacote_web()
{
    int tamPacotes[3] = {1500, 40, 550};
    int i = rand() % 10;
    if (i < 1)
    {
        i = 0;
    }
    else if (i < 5)
    {
        i = 1;
    }
    else
    {
        i = 2;
    }
    return tamPacotes[i];
}

int gera_pacote(double intervalo_medio_chegada_web, double intervalo_medio_chegada_ligacao, long int no_chamadas)
{
    double no_pacotes_web_por_segundo = 1.0 / intervalo_medio_chegada_web;
    double no_pacotes_ligacao_por_segundo = no_chamadas / intervalo_medio_chegada_ligacao;
    double no_pacotes_por_segundo = no_pacotes_web_por_segundo + no_pacotes_ligacao_por_segundo;
    int chance_web = (no_pacotes_web_por_segundo / no_pacotes_por_segundo * 100);
    int chance = rand() % 100;

    if (chance < chance_web)
    {
        no_pacotes_web++;
        return gera_pacote_web();
    }
    else
    {
        no_pacotes_ligacao++;
        return gera_pacote_ligacao();
    }
}

Event create_event(MinHeap *heapEventos, EventType type, double time)
{
    Event event;
    event.type = type;
    event.time = time;
    insert_minheap(heapEventos, event);
    return event;
}

void printArray(double *arr, int arr_size)
{
    int i;
    for (i = 0; i < arr_size; i++)
    {
        printf("%lf ", arr[i]);
    }
    printf("\n");
}

double exponential(double lambda)
{
    return -1.0 / (1.0 / lambda) * log(aleatorio());
}

int main()
{
    // Capacity of 10 elements
    MinHeap *heapEventos = init_minheap(2000);
    double tempo_simulacao = 36000;
    double tempo_decorrido = 0.0;

    double intervalo_medio_chamada = 10;
    double duracao_chamada = 100;

    double intervalo_medio_chegada_ligacao = 0.02;
    double intervalo_medio_chegada_web = 0.01;
    double largura_link;
    double porc_ocupacao;

    Event chegada;
    Event servico;
    Event last_servico;
    Event coleta_dados;
    Event chamada;
    Event fim_chamada;

    double soma_tempo_servico = 0.0;

    unsigned long int fila = 0;
    unsigned long int max_fila = 0;
    unsigned long int no_chamadas = 0;

    double e_n_final = 0.0;
    double e_w_final = 0.0;
    double lambda;

    /**
    Little
    */
    little e_n;
    little e_w_chegada;
    little e_w_saida;

    inicia_little(&e_n);
    inicia_little(&e_w_chegada);
    inicia_little(&e_w_saida);
    /**
    Little -- fim
    */

    srand(1);
    // calculando chances que cada pacote tem de ser gerado
    double no_pacotes_web_por_segundo = 1.0 / intervalo_medio_chegada_web;
    double no_pacotes_ligacao_por_segundo = (duracao_chamada / intervalo_medio_chamada) / intervalo_medio_chegada_ligacao;
    double no_pacotes_por_segundo = no_pacotes_web_por_segundo + no_pacotes_ligacao_por_segundo;
    double chance_web = no_pacotes_web_por_segundo / no_pacotes_por_segundo;

    scanf("%lF", &porc_ocupacao);
    printf("\n%.2lF%%,0", porc_ocupacao * 100);
    double intervalo_medio_chegada = 1 / (1 / intervalo_medio_chegada_web + (duracao_chamada / intervalo_medio_chamada) / intervalo_medio_chegada_ligacao);
    largura_link = (1.00 / intervalo_medio_chegada) * ((0.1 * 1500.00 + 0.4 * 40.00 + 0.5 * 550.00) * chance_web + 1280.00 * (1 - chance_web)) / porc_ocupacao;
    printf("Largura do link: %lF\n", largura_link);

    chamada = create_event(heapEventos, NOVA_CHAMADA, exponential(intervalo_medio_chamada));
    create_event(heapEventos, FIM_CHAMADA, chamada.time + exponential(duracao_chamada));
    create_event(heapEventos, CHEGADA, exponential(intervalo_medio_chegada));
    create_event(heapEventos, COLETA_DADOS, 100.0);
    while (tempo_decorrido <= tempo_simulacao)
    {
        intervalo_medio_chegada = 1 / (1 / intervalo_medio_chegada_web + no_chamadas / intervalo_medio_chegada_ligacao);
        Event current_event = extract_minheap(heapEventos);
        tempo_decorrido = current_event.time;

        // printf("Evento: %c, Tempo: %lF, Fila de Pacotes: %lu, Numero de chamadas em simul.: %lu\n", current_event.type, current_event.time, fila, no_chamadas);
        switch (current_event.type)
        {
        case COLETA_DADOS:

            // printf("%lF,", coleta_dados * 100);
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

            e_w_saida.tempo_anterior = tempo_decorrido;
            e_n.tempo_anterior = tempo_decorrido;
            e_w_chegada.tempo_anterior = tempo_decorrido;

            e_n_final = e_n.soma_areas / tempo_decorrido;
            e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / (double)e_w_chegada.no_eventos;
            lambda = e_w_chegada.no_eventos / tempo_decorrido;

            E_N(printf(",%lF", e_n_final););
            E_W(printf(",%lF", e_w_final););
            ERRO_LITTLE(printf(",%.20lF", fabs(e_n_final - lambda * e_w_final)););
            OCUPACAO(printf(",%lF", soma_tempo_servico / maximo(coleta_dados, servico)););
            create_event(heapEventos, COLETA_DADOS, tempo_decorrido + 100.00);
            break;

        case CHEGADA:

            if (!fila)
            {
                servico = create_event(heapEventos, SERVICO, tempo_decorrido + gera_pacote(intervalo_medio_chegada_web, intervalo_medio_chegada_ligacao, no_chamadas) / largura_link);
                soma_tempo_servico += servico.time - tempo_decorrido;
            }

            fila++;
            max_fila = fila > max_fila ? fila : max_fila;

            create_event(heapEventos, CHEGADA, tempo_decorrido + exponential(intervalo_medio_chegada));

            // little
            e_n.soma_areas +=
                (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos++;

            e_w_chegada.soma_areas +=
                (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_chegada.no_eventos++;
            break;

        case SERVICO:
            fila--;
            if (fila)
            {
                servico = create_event(heapEventos, SERVICO, tempo_decorrido + gera_pacote(intervalo_medio_chegada_web, intervalo_medio_chegada_ligacao, no_chamadas) / largura_link);
                soma_tempo_servico += servico.time - tempo_decorrido;
            }

            // little
            e_n.soma_areas +=
                (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos--;

            e_w_saida.soma_areas +=
                (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;
            e_w_saida.tempo_anterior = tempo_decorrido;
            e_w_saida.no_eventos++;
            break;

        case NOVA_CHAMADA:

            no_chamadas++;
            chamada = create_event(heapEventos, NOVA_CHAMADA, tempo_decorrido + exponential(intervalo_medio_chamada));
            create_event(heapEventos, FIM_CHAMADA, chamada.time + exponential(duracao_chamada));
            break;

        case FIM_CHAMADA:
            no_chamadas--;
            break;
        }
    }
    e_w_chegada.soma_areas +=
        (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;

    e_w_saida.soma_areas +=
        (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

    e_n_final = e_n.soma_areas / tempo_decorrido;
    e_w_final =
        (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
    lambda = e_w_chegada.no_eventos / tempo_decorrido;
    VALORES_FINAIS(
        puts("\nResultado final:");

        printf("E[N]: %lF\n", e_n_final);
        printf("E[W]: %lF\n", e_w_final);
        printf("lambda: %lF\n\n", lambda);
        printf("Erro de Little: %.20lF\n\n", fabs(e_n_final - lambda * e_w_final));
        printf("Ocupacao: %lF.\n", soma_tempo_servico / tempo_decorrido);
        printf("Max fila: %ld.\n", max_fila);
        printf("Numero de pacotes: web: %ld, ligacao: %ld.\n", no_pacotes_web, no_pacotes_ligacao);)
    free_minheap(heapEventos);

    return 0;
}
