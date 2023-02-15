# Trabalho 2 (2022/2)

> <strong>O código fonte principal são `simulador_ligacao/simulacao_principal.c` e `simulador_web/simulacao_web.c` </strong>

## 💻 Objetivo

Gerar dados aleatórios que simulem o comportamento de um comutador de pacotes. Todos os dados gerados são captados a cada 100 segundos e armazenados em um arquivo de texto. O arquivo de texto é lido e os dados são processados para gerar gráficos que representam o comportamento do sistema de filas.

## 🚀 Como executar

Definir quais informações deseja exibir manipulando as macros que existem no inicio do código fonte principal. As macros são:

```c
#define OCUPACAO(x) 
#define E_N(x) 
#define E_W(x) 
#define ERRO_LITTLE(x) x
#define E_N_TR(x) 
#define E_W_TR(x) 
#define ERRO_LITTLE_TR(x) 
#define VALORES_FINAIS(x)
```
 No exemplo acima, a macro `ERROR_LITTLE` está definida, portanto, o programa irá gerar um arquivo de texto com os valores de erro de Little. Para gerar o arquivo csv, basta executar o programa com o comando `./generate_csv <filename>`. O arquivo csv `<filename>` será gerado na pasta `csv/`.
