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

int gera_pacote_ligacao()
{
    return 1280;
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

    double intervalo_medio_chamada = 5;
    double duracao_chamada = 20;

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

    // srand(time(NULL));
    srand(10000);

    printf("Informe o percentual de ocupação desejado (entre 0 e 1): ");
    scanf("%lF", &porc_ocupacao);
    // largura_link = (1 / intervalo_medio_chegada) * 1280 / porc_ocupacao;
    printf("\n%.2lF%%,0", porc_ocupacao * 100);

    largura_link = (1.00 / intervalo_medio_chegada_web + 2.00 / intervalo_medio_chegada_ligacao) * ((0.1 * 1500.00 + 0.4 * 40.00 + 0.5 * 550.00) * 1.00 / 3.00 + 1280.00 * 2.00/3.00) / porc_ocupacao;
    // largura_link = 1 / intervalo_medio_chegada_web * ((0.1 * 1500 + 0.4 * 40 + 0.5 * 550)) / porc_ocupacao + 2 / intervalo_medio_chegada_ligacao * 1280 / porc_ocupacao;
    printf("Largura do link: %lF\n", largura_link);

    chamada = (Event){NOVA_CHAMADA, exponential(intervalo_medio_chamada)};
    insert_minheap(heapEventos, chamada);


    chamada = (Event){NOVA_CHAMADA, exponential(intervalo_medio_chamada)};
    insert_minheap(heapEventos, chamada);

    // fim_chamada = (Event){FIM_CHAMADA, chamada.time + exponential(duracao_chamada)};
    // insert_minheap(heapEventos, fim_chamada);

    chegada = (Event){CHEGADA_WEB, exponential(intervalo_medio_chegada_web)};
    insert_minheap(heapEventos, chegada);

    coleta_dados = (Event){COLETA_DADOS, 100.00};
    insert_minheap(heapEventos, coleta_dados);

    int count = 0;
    int sum = 0;
    while (tempo_decorrido <= tempo_simulacao)
    {
        // largura_link = (1 / (intervalo_medio_chegada / (no_chamadas))) * 1280 / porc_ocupacao;
        // printf("Largura do link: %lF\n", largura_link);
        // print_heap(heapEventos);
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
            coleta_dados = (Event){COLETA_DADOS, tempo_decorrido + 100.00};
            insert_minheap(heapEventos, coleta_dados);
            break;

        case CHEGADA_WEB:

            if (!fila)
            {
                last_servico.time = maximo(servico.time, tempo_decorrido);
                servico = (Event){SERVICO_WEB, last_servico.time + gera_pacote_web() / largura_link};
                insert_minheap(heapEventos, servico);
                soma_tempo_servico += servico.time - last_servico.time;
            }
            fila++;
            max_fila = fila > max_fila ? fila : max_fila;

            chegada = (Event){CHEGADA_WEB, tempo_decorrido + exponential(intervalo_medio_chegada_web)};
            insert_minheap(heapEventos, chegada);
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
                last_servico.time = maximo(servico.time, tempo_decorrido);
                servico = (Event){SERVICO_WEB, last_servico.time + gera_pacote_web() / largura_link};
                insert_minheap(heapEventos, servico);
                soma_tempo_servico += servico.time - last_servico.time;
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

            if (!fila)
            {
                last_servico.time = maximo(servico.time, tempo_decorrido);
                servico = (Event){SERVICO_LIGACAO, last_servico.time + gera_pacote_ligacao() / largura_link};
                insert_minheap(heapEventos, servico);
                soma_tempo_servico += servico.time - last_servico.time;
            }
            fila++;
            max_fila = fila > max_fila ? fila : max_fila;

            chegada = (Event){CHEGADA_LIGACAO, tempo_decorrido + exponential(intervalo_medio_chegada_ligacao / no_chamadas)};
            insert_minheap(heapEventos, chegada);
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

        case SERVICO_LIGACAO:
            fila--;

            if (fila)
            {
                last_servico.time = maximo(servico.time, tempo_decorrido);
                servico = (Event){SERVICO_LIGACAO, last_servico.time + gera_pacote_ligacao() / largura_link};
                insert_minheap(heapEventos, servico);
                soma_tempo_servico += servico.time - last_servico.time;
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

            if (!no_chamadas)
            {
                chegada = (Event){CHEGADA_LIGACAO, tempo_decorrido + exponential(intervalo_medio_chegada_ligacao)};
                insert_minheap(heapEventos, chegada);
            }

            no_chamadas++;
            sum += no_chamadas;
            count++;
            // chamada = (Event){NOVA_CHAMADA, tempo_decorrido + exponential(intervalo_medio_chamada)};
            // insert_minheap(heapEventos, chamada);

            // fim_chamada = (Event){FIM_CHAMADA, chamada.time + exponential(duracao_chamada)};
            // insert_minheap(heapEventos, fim_chamada);
            break;

        case FIM_CHAMADA:
            no_chamadas--;
            // if (no_chamadas > 1)
            // {
            //     fim_chamada = (Event){FIM_CHAMADA, tempo_decorrido + (-1.0 / (1.0 / duracao_chamada)) * log(aleatorio())};
            //     insert_minheap(heapEventos, fim_chamada);
            // }

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
        printf("Ocupacao: %lF.\n", soma_tempo_servico / maximo(tempo_decorrido, servico.time));
        printf("Max fila: %ld.\n", max_fila);)
    printf("Media de chamadas: %lF.\n", (double)sum / count);
    return 0;
}

// int main() {
//     // Capacity of 10 elements
//     MinHeap* heap = init_minheap(10);

//     insert_minheap(heap, (Event) {1, 3.12321});
//     insert_minheap(heap, (Event) {1, 10.00});
//     insert_minheap(heap, (Event) {1, 4.0});
//     insert_minheap(heap, (Event) {1, 3.12319});
//     insert_minheap(heap, (Event) {1, 20.0});

//     print_heap(heap);

//     // Delete the heap->arr[1] (50)
//     delete_element(heap, 1);

//     print_heap(heap);

//     // Delete the minimum element
//     delete_minimum(heap);
//     print_heap(heap);
//     free_minheap(heap);
//     return 0;
// }
