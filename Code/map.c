#include "Graphics.h"
#include "map.h"
#include "Robots.h"

void open_file(FILE **fp, char *map_name) //Abre e inicializa o ficheiro .map
{
    (*fp) = fopen(map_name , "r"); //Abre o ficheiro correspondente ao mapa, dado por argv[1] em modo de leitura
    if((*fp) == NULL) //Apresenta uma mensagem de erro e encerra o programa caso o ficheiro não exista
    {
        fprintf(stdout, "Erro na abertura do mapa, mapa '%s' não existente\n", map_name);
        exit(EXIT_FAILURE);
    }
}

void read_firstline(FILE *fp, int *largura, int *altura, int *obstaculos) //Lê e guarda os valores da primeira linha
{
    char line[small_string]; //Variável do tipo string auxiliar para guardar o valor que recebe de fgets

    fgets(line, small_string, fp); //Retira os valores do ficheiro e guarda-os nas variáveis globais por referência
    if(sscanf(line, "%d %d %d", &(*largura), &(*altura), &(*obstaculos)) != 3) //Confirma também todos os parametros de sscanf
    {   //Apresenta uma mensagem de erro e encerra o programa caso o ficheiro não esteja de acordo com o predefinido
        fprintf(stdout, "Erro no ficheiro .map, não corresponde à formatação predifinida\n");
        exit(EXIT_FAILURE);
    }
}

MAP** create_map(int largura, int altura) //Cria uma matriz 2D para o mapa
{
    int i=0; //Variável local auxiliar para percorrer a matriz
    MAP **M; //Variável do tipo MAP** para criar a matriz 2D

    M = (MAP**)calloc(largura,sizeof(MAP*)); //Aloca e inicializa a 0 (índice dos quadrados sujos) o número de posições da largura

	for(i = 0; i < largura; i++) //Para cada posição de x aloca e inicializa a 0 uma coluna com o tamanho da altura.
	{
		M[i] = (MAP*)calloc(altura,sizeof(MAP));
	}
    return M; //Retorna a matriz
}

MAP** read_obstacles(FILE *fp, MAP **mapa, int obstaculos) //Lê e coloca os obstaculos no mapa
{
    int i=1;    //Variável local auxiliar para percorrer o número de obstáculos
    int auxX=0, auxY=0; //Variáveis auxiliares x e y
    char line[small_string]; //Variável do tipo string auxiliar para guardar o valor que recebe de fgets

    for(i=1;i<=obstaculos;i++) //Percorre o número de obstáculos
    {
        fgets(line, small_string, fp); //Retira os valores do ficheiro e guarda-os nas variáveis locais
        if(sscanf(line, "[%d, %d]", &auxX, &auxY) == 2) //Confirma também todos os parametros de sscanf
            mapa[auxX][auxY].state = obstacle; //Coloca o obstáculo que leu na matriz
        else //Apresenta uma mensagem de erro e encerra o programa caso o ficheiro não esteja de acordo com o predefinido
        {
            fprintf(stdout, "Erro no ficheiro .map, não corresponde à formatação predifinida\n");
            exit(EXIT_FAILURE);
        }
    }
    return mapa; //retorna a matriz principal
}

MAP** free_map(int largura, MAP **mapa) //Liberta a memória alocada para o mapa
{
    int i=0; //Variável local auxiliar para percorrer o número de obstáculos

    for(i=0; i<largura; i++) //Percorre todas as colunas da matriz
    {
        free(mapa[i]); //Liberta cada coluna da matriz
    }
    free(mapa); //Liberta a primeira linha da matriz
    mapa=NULL; //Atribui o endereço NULL ao mapa

    return mapa;
}

void check_end(MAP **mapa,int *stop, int largura, int altura) //Verifica se todas as posições do mapa já foram limpas
{
    int x=0, y=0; //Declaração de duas variáveis auxiliares para correr todas as posições do mapa principal
    int i=0; //Declaração de variável auxiliar para contabilizar o número de posições que não estão sujas

    for(x=0; x<largura; x++) //Junção de 2 for para passar por todas as posições da matriz "mapa"
    {
        for(y=0; y<altura; y++)
        {
            if(mapa[x][y].state != dirty) //Se a posição específica não estiver suja, ie, ser um obstáculo ou estar limpa,
                i++;                      //a variável i é incrementada
        }
    }

    if(i==(largura*altura)) //Se a variável i tomar o valor da área do mapa, ie, todos os quadrados os estão limpos
        *stop=STOP;         //ou são obstáculos, a variável stop toma o valor STOP que é passado por referência
}
