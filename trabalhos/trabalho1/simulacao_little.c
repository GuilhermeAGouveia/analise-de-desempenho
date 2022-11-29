#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Escolha apenas uma das macros abaixo para definir o tipo de medida que será exibida
#define OCUPACAO(x)
#define E_N(x)
#define E_W(x)
#define ERRO_LITTLE(x) x
#define VALORES_FINAIS(x) 

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

double minimo(double num1, double num2)
{
    if (num1 < num2)
    {
        return num1;
    }
    return num2;
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

int main()
{
    double tempo_simulacao = 36000;
    double tempo_decorrido = 0.0;

    double intervalo_medio_chegada = 0.2;
    double tempo_medio_servico;
    double porc_ocupacao;

    double chegada;
    double servico;

    double soma_tempo_servico = 0.0;

    unsigned long int fila = 0;
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

    // printf("Informe o tempo medio de servico (segundos): ");
    scanf("%lF", &porc_ocupacao);
    tempo_medio_servico = intervalo_medio_chegada * porc_ocupacao;
    printf("\n%.2lF%%,0", porc_ocupacao * 100);

    // gerando o tempo de chegada da primeira requisicao.
    chegada = (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());
    double timeControl = 100.00;
    while (tempo_decorrido <= tempo_simulacao)
    {
        tempo_decorrido = !fila ? chegada : minimo(chegada, servico);

        if (tempo_decorrido >= timeControl)
        {
            // printf("%lF,", timeControl * 100);
            // double soma_areas_chegada = e_w_chegada.soma_areas + (timeControl - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            // double soma_areas_saida = e_w_saida.soma_areas + (timeControl - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

            // e_n_final = e_n.soma_areas / (timeControl);
            // e_w_final = (soma_areas_chegada - soma_areas_saida) / (double)e_w_chegada.no_eventos;
            // lambda = e_w_chegada.no_eventos / (timeControl);

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
            OCUPACAO(printf(",%lF", soma_tempo_servico / maximo(timeControl, servico)););
            timeControl += 100.00;
        }

        // chegada
        if (tempo_decorrido == chegada)
        {
            // printf("Chegada em %lF.\n", tempo_decorrido);
            if (!fila)
            {
                servico = tempo_decorrido + (-1.0 / (1.0 / tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }
            fila++;
            max_fila = fila > max_fila ? fila : max_fila;

            chegada = tempo_decorrido + (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());

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
        else
        { // saida
            // printf("Saida em %lF.\n", tempo_decorrido);
            fila--;

            if (fila)
            {
                servico = tempo_decorrido + (-1.0 / (1.0 / tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
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
        printf("Ocupacao: %lF.\n", soma_tempo_servico / maximo(tempo_decorrido, servico));
        printf("Max fila: %d.\n", max_fila);)

    return 0;
}

// testando o live sh
// e ai mermão kkkkk
// tem chat pô
// eu estou pelo celular kkkk

//é daora pra caramba mesmo kkk//é daora pra caramba nesmo kkkkk// oloco kk
// mas tem um chatzinho embutido tbm kk
// muito bom