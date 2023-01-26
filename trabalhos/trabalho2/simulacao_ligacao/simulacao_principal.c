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

double minimo(double num1, double num2)
{
    if (num1 < num2)
    {
        return num1;
    }
    return num2;
}

double minimo_a(double *arr, int arr_size)
{
    int i;
    double min = arr[0];
    for (i = 1; i < 3; i++)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }
    return min;
}

void inicia_little(little *l)
{
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}

int gera_pacote_chamada(int num_chamadas, int intervalo_medio_chegada, int CBR)
{
    return CBR * intervalo_medio_chegada * num_chamadas;
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

int main()
{
    // Capacity of 10 elements
    MinHeap *heapEventos = init_minheap(200);
    double tempo_simulacao = 100.0;
    double tempo_decorrido = 0.0;

    double CBR = 64000;

    double intervalo_medio_chegada = 0.02;
    double intervalo_medio_chamada = 5;
    double duracao_media_chamada = 20;

    double largura_link;
    double porc_ocupacao;

    Event chegada;
    Event servico;
    Event coleta_dados;
    Event nova_chamada;
    Event fim_chamada;

    double soma_tempo_servico = 0.0;

    unsigned long int fila = 0;
    unsigned long int num_chamadas = 0;
    unsigned long int max_fila = 0;

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
    largura_link = (1 / intervalo_medio_chegada) * ((duracao_media_chamada / intervalo_medio_chamada) * intervalo_medio_chegada * CBR) / porc_ocupacao;
    printf("Largura do link: %lF", largura_link);
    printf("\n%.2lF%%,0", porc_ocupacao * 100);

    // gerando o tempo de chegada da primeira requisicao.

    coleta_dados = (Event){COLETA_DADOS, 100.00};
    insert_minheap(heapEventos, coleta_dados);

    nova_chamada = (Event){NOVA_CHAMADA, (-1.0 / (1.0 / intervalo_medio_chamada)) * log(aleatorio())};
    insert_minheap(heapEventos, nova_chamada);

    while (tempo_decorrido <= tempo_simulacao)
    {
        Event current_event = extract_minheap(heapEventos);
        tempo_decorrido = current_event.time;
        printf("Numero de chamadas no momento: %lu\n", num_chamadas);
        printf("Event: %c, Time: %lF\n", current_event.type, current_event.time);

        // coleta de dados
        if (current_event.type == COLETA_DADOS)
        {

            // Fazer coleta | inicio
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
            // Fazer coleta | fim

            // Gerar novo evento de coleta de dados | inicio
            coleta_dados = (Event){COLETA_DADOS, tempo_decorrido + 100.00};
            insert_minheap(heapEventos, coleta_dados);
            // Gerar novo evento de coleta de dados | fim
        }
        // chegada
        else if (current_event.type == CHEGADA)
        {
            // printf("Chegada em %lF.\n", tempo_decorrido);
            if (!fila)
            {
                // Gerar evento de servico | inicio
                servico = (Event){SERVICO, tempo_decorrido + gera_pacote_chamada(num_chamadas, intervalo_medio_chegada, CBR) / largura_link};
                insert_minheap(heapEventos, servico);
                // Gerar evento de servico | fim
                soma_tempo_servico += servico.time - tempo_decorrido;
            }

            if (num_chamadas)
            {
                fila++;
                max_fila = fila > max_fila ? fila : max_fila;
                // Gerar evento de chegada | inicio
                chegada = (Event){CHEGADA, tempo_decorrido + (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio())};
                insert_minheap(heapEventos, chegada);
                // Gerar evento de chegada | fim

                // little
                e_n.soma_areas +=
                    (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
                e_n.tempo_anterior = tempo_decorrido;
                e_n.no_eventos++;

                e_w_chegada.soma_areas +=
                    (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
                e_w_chegada.tempo_anterior = tempo_decorrido;
                e_w_chegada.no_eventos++;
            }
        }

        // saida
        else if (current_event.type == SERVICO)
        {
            fila--;
            // Se a fila nao estiver vazia, gerar evento de servico
            if (fila)
            {
                // Gerar evento de servico | inicio
                servico = (Event){SERVICO, tempo_decorrido + gera_pacote_chamada(num_chamadas, intervalo_medio_chegada, CBR) / largura_link};
                insert_minheap(heapEventos, servico);
                // Gerar evento de servico | fim
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
        }
        // nova chamada
        else if (current_event.type == NOVA_CHAMADA)
        {

            if (!num_chamadas)
            {
                // Gerar evento de fim de chamada | inicio
                fim_chamada = (Event){FIM_CHAMADA, tempo_decorrido + (-1.0 / (1.0 / duracao_media_chamada)) * log(aleatorio())};
                insert_minheap(heapEventos, servico);
                // Gerar evento de fim de chamada | fim
            }

            num_chamadas++;

            if (num_chamadas == 1)
            {
                // Gerar primeiro evento de chegada | inicio
                chegada = (Event){CHEGADA, tempo_decorrido + (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio())};
                insert_minheap(heapEventos, chegada);
                // Gerar evento de chegada | fim
            }

            // Gerar evento de nova chamada | inicio
            nova_chamada = (Event){NOVA_CHAMADA, tempo_decorrido + (-1.0 / (1.0 / intervalo_medio_chamada)) * log(aleatorio())};
            insert_minheap(heapEventos, nova_chamada);
            // Gerar evento de nova chamada | fim
        }
        else if (current_event.type == FIM_CHAMADA)
        {
            num_chamadas--;

            if (num_chamadas)
            {
                fim_chamada = (Event){FIM_CHAMADA, tempo_decorrido + (-1.0 / (1.0 / duracao_media_chamada)) * log(aleatorio())};
                insert_minheap(heapEventos, servico);
            }
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
