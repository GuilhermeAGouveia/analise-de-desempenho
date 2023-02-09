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
#define E_N_TR(x) 
#define E_W_TR(x) 
#define ERRO_LITTLE_TR(x) 
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

double minimo(double num1, double num2)
{
    if (num1 < num2)
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
    no_pacotes_ligacao++;
    return 160;
}

int gera_pacote_web()
{
    int tamPacotes[3] = {1500, 40, 550};
    int i = rand() % 100;
    if (i < 10)
    {
        i = 0;
    }
    else if (i < 50)
    {
        i = 1;
    }
    else
    {
        i = 2;
    }
    no_pacotes_web++;
    return tamPacotes[i];
}

Event create_event(MinHeap *heapEventos, EventType type, double time)
{
    Event event;
    event.type = type;
    event.time = time;
    insert_minheap(heapEventos, event);
    return event;
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
    double tempo_servico;

    Event chegada;
    Event servico_ligacao;
    Event servico_web;
    Event last_servico;
    Event coleta_dados;
    Event chamada;
    Event fim_chamada;

    double soma_tempo_servico = 0.0;

    unsigned long int fila = 0;
    unsigned long int fila_l = 0;
    unsigned long int no_chamadas = 0;

    double e_n_final = 0.0;
    double e_w_final = 0.0;
    double lambda;

    double e_n_final_tr = 0.0;
    double e_w_final_tr = 0.0;
    double lambda_tr;

    /**
    Little
    */
    little e_n;
    little e_w_chegada;
    little e_w_saida;

    inicia_little(&e_n);
    inicia_little(&e_w_chegada);
    inicia_little(&e_w_saida);

    /*
    Little tempo real
    */
    little e_n_tr;
    little e_w_chegada_tr;
    little e_w_saida_tr;

    inicia_little(&e_n_tr);
    inicia_little(&e_w_chegada_tr);
    inicia_little(&e_w_saida_tr);
    /**
    Little -- fim
    */

    srand(100);
    // calculando chances que cada pacote tem de ser gerado
    double no_pacotes_web_por_segundo = 1.0 / intervalo_medio_chegada_web;
    double no_pacotes_ligacao_por_segundo = (duracao_chamada / intervalo_medio_chamada) / intervalo_medio_chegada_ligacao;
    double no_pacotes_por_segundo = no_pacotes_web_por_segundo + no_pacotes_ligacao_por_segundo;
    double chance_web = no_pacotes_web_por_segundo / no_pacotes_por_segundo;

    scanf("%lF", &porc_ocupacao);
    double intervalo_medio_chegada = 1 / (1 / intervalo_medio_chegada_web + (duracao_chamada / intervalo_medio_chamada) / intervalo_medio_chegada_ligacao);
    largura_link = (1.00 / intervalo_medio_chegada) * ((0.1 * 1500.00 + 0.4 * 40.00 + 0.5 * 550.00) * chance_web + 160.00 * (1 - chance_web)) / porc_ocupacao;
    VALORES_FINAIS(printf("Largura do link para %.2lF%% de ocupação: %lF\n", porc_ocupacao * 100, largura_link););

    chamada = create_event(heapEventos, NOVA_CHAMADA, exponential(intervalo_medio_chamada));
    create_event(heapEventos, FIM_CHAMADA, chamada.time + exponential(duracao_chamada));
    create_event(heapEventos, CHEGADA_WEB, exponential(intervalo_medio_chegada_web));
    create_event(heapEventos, CHEGADA_LIGACAO, chamada.time);
    create_event(heapEventos, COLETA_DADOS, 100.0);
    
    while (tempo_decorrido <= tempo_simulacao)
    {
        Event current_event = extract_minheap(heapEventos);
        tempo_decorrido = current_event.time;

        switch (current_event.type)
        {
        case COLETA_DADOS:

            // Geral
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

            e_w_saida.tempo_anterior = tempo_decorrido;
            e_n.tempo_anterior = tempo_decorrido;
            e_w_chegada.tempo_anterior = tempo_decorrido;

            e_n_final = e_n.soma_areas / tempo_decorrido;
            e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / (double)e_w_chegada.no_eventos;
            lambda = e_w_chegada.no_eventos / tempo_decorrido;

            // Tempo real
            e_n_tr.soma_areas += (tempo_decorrido - e_n_tr.tempo_anterior) * e_n_tr.no_eventos;
            e_w_chegada_tr.soma_areas += (tempo_decorrido - e_w_chegada_tr.tempo_anterior) * e_w_chegada_tr.no_eventos;
            e_w_saida_tr.soma_areas += (tempo_decorrido - e_w_saida_tr.tempo_anterior) * e_w_saida_tr.no_eventos;

            e_w_saida_tr.tempo_anterior = tempo_decorrido;
            e_n_tr.tempo_anterior = tempo_decorrido;
            e_w_chegada_tr.tempo_anterior = tempo_decorrido;

            e_n_final_tr = e_n_tr.soma_areas / tempo_decorrido;
            e_w_final_tr = (e_w_chegada_tr.soma_areas - e_w_saida_tr.soma_areas) / (double)e_w_chegada_tr.no_eventos;
            lambda_tr = e_w_chegada_tr.no_eventos / tempo_decorrido;

            E_N(printf("%.0lF %lF\n", tempo_decorrido, e_n_final););
            E_W(printf("%.0lF %lF\n", tempo_decorrido, e_w_final););
            ERRO_LITTLE(printf("%.0lF %.20lF\n", tempo_decorrido, (e_n_final - lambda * e_w_final)););
            OCUPACAO(printf("%.0lF %lF\n", tempo_decorrido, soma_tempo_servico / maximo(coleta_dados.time, maximo(servico_ligacao.time, servico_web.time))););
            E_N_TR(printf("%.0lF %lF\n", tempo_decorrido, e_n_final_tr););
            E_W_TR(printf("%.0lF %lF\n", tempo_decorrido, e_w_final_tr););
            ERRO_LITTLE_TR(printf("%.0lF %.20lF\n", tempo_decorrido, (e_n_final_tr - lambda_tr * e_w_final_tr)););
            create_event(heapEventos, COLETA_DADOS, tempo_decorrido + 100.00);
            break;

        case CHEGADA_WEB:

            if (!fila)
            {
                tempo_servico = gera_pacote_web() / largura_link;
                servico_web = create_event(heapEventos, SERVICO_WEB, maximo(servico_ligacao.time, tempo_decorrido) + tempo_servico);
                soma_tempo_servico += tempo_servico;
            }

            fila++;

            create_event(heapEventos, CHEGADA_WEB, tempo_decorrido + exponential(intervalo_medio_chegada_web));
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

        case SERVICO_WEB:
            fila--;
            if (fila)
            {
                tempo_servico = gera_pacote_web() / largura_link;
                servico_web = create_event(heapEventos, SERVICO_WEB, maximo(servico_ligacao.time, tempo_decorrido) + tempo_servico);
                soma_tempo_servico += tempo_servico;
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

        case CHEGADA_LIGACAO:
            if (!fila_l)
            {
                tempo_servico = gera_pacote_ligacao() / largura_link;
                servico_ligacao = create_event(heapEventos, SERVICO_LIGACAO, maximo(servico_web.time, tempo_decorrido) + tempo_servico);
                soma_tempo_servico += tempo_servico;
            }

            fila_l++;

            create_event(heapEventos, CHEGADA_LIGACAO, tempo_decorrido + exponential(intervalo_medio_chegada_ligacao / (duracao_chamada / intervalo_medio_chamada)));
            // little
            e_n.soma_areas +=
                (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos++;

            e_w_chegada.soma_areas +=
                (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_chegada.no_eventos++;

            e_n_tr.soma_areas +=
                (tempo_decorrido - e_n_tr.tempo_anterior) * e_n_tr.no_eventos;
            e_n_tr.tempo_anterior = tempo_decorrido;
            e_n_tr.no_eventos++;

            e_w_chegada_tr.soma_areas +=
                (tempo_decorrido - e_w_chegada_tr.tempo_anterior) * e_w_chegada_tr.no_eventos;
            e_w_chegada_tr.tempo_anterior = tempo_decorrido;
            e_w_chegada_tr.no_eventos++;
            break;

        case SERVICO_LIGACAO:
            fila_l--;
            if (fila_l)
            {
                tempo_servico = gera_pacote_ligacao() / largura_link;
                servico_ligacao = create_event(heapEventos, SERVICO_LIGACAO, maximo(servico_web.time, tempo_decorrido) + tempo_servico);
                soma_tempo_servico += tempo_servico;
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

            // little
            e_n_tr.soma_areas +=
                (tempo_decorrido - e_n_tr.tempo_anterior) * e_n_tr.no_eventos;
            e_n_tr.tempo_anterior = tempo_decorrido;
            e_n_tr.no_eventos--;

            e_w_saida_tr.soma_areas +=
                (tempo_decorrido - e_w_saida_tr.tempo_anterior) * e_w_saida_tr.no_eventos;
            e_w_saida_tr.tempo_anterior = tempo_decorrido;
            e_w_saida_tr.no_eventos++;
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
    e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;

    e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

    e_n_final = e_n.soma_areas / tempo_decorrido;
    e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
    lambda = e_w_chegada.no_eventos / tempo_decorrido;

    // Tempo real
    e_w_chegada_tr.soma_areas += (tempo_decorrido - e_w_chegada_tr.tempo_anterior) * e_w_chegada_tr.no_eventos;

    e_w_saida_tr.soma_areas += (tempo_decorrido - e_w_saida_tr.tempo_anterior) * e_w_saida_tr.no_eventos;

    e_n_final_tr = e_n_tr.soma_areas / tempo_decorrido;
    e_w_final_tr = (e_w_chegada_tr.soma_areas - e_w_saida_tr.soma_areas) / e_w_chegada_tr.no_eventos;
    lambda_tr = e_w_chegada_tr.no_eventos / tempo_decorrido;
    VALORES_FINAIS(
        puts("\nResultado final:");
        puts("Geral:");
        printf("E[N]: %lF\n", e_n_final);
        printf("E[W]: %lF\n", e_w_final);
        printf("lambda: %lF\n", lambda);
        printf("Erro de Little: %.20lF\n", fabs(e_n_final - lambda * e_w_final));
        printf("Ocupacao: %lF.\n", soma_tempo_servico / tempo_decorrido);
        printf("Numero de pacotes: web: %d, ligacao: %d.\n", no_pacotes_web, no_pacotes_ligacao);

        puts("\nTempo real:");
        printf("E[N]: %lF\n", e_n_final_tr);
        printf("E[W]: %lF\n", e_w_final_tr);
        printf("lambda: %lF\n", lambda_tr);
        printf("Erro de Little: %.20lF\n\n", fabs(e_n_final_tr - lambda_tr * e_w_final_tr));)
    free_minheap(heapEventos);

    return 0;
}
