#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

// pre-processor definitions
#define nameDim_MAX 3
#define small_string 100
#define single 1
#define obstacle 2
#define clean 1
#define dirty 0
#define OCUP 1
#define NOCUP 0
#define new_posi 1
#define old_posi 0
#define BatMIN 0.1
#define MAXbatRand 100
#define MINbatRand 50
#define PAUSE 1
#define running 0
#define batery 5
#define no_batery 0
#define RESET 1
#define NRESET 0
#define not_new_rob 0
#define new_rob_place 1
#define new_rob_name 2
#define RUN 0
#define STOP 1
#define NONE 0

typedef struct dados_mapa {
    char name[nameDim_MAX]; //Robot name, if present
    int state; //0=Dirty, 1=Clean, 2=Obstacle
    int ocupied; //0=No robots, 1=Robot is there
} MAP;

void open_file(FILE **fp, char *map_name); //Função que abre e inicializa o ficheiro .map
void read_firstline(FILE *fp, int *largura, int *altura, int *obstaculos);//Função que lê e guarda os valores da primeira linha
MAP** create_map(int largura, int altura); //Função que cria uma matriz 2D para o mapa
MAP** read_obstacles(FILE *fp, MAP **mapa, int obstaculos); //Função que lê e coloca os obstaculos no mapa
MAP** free_map(int largura, MAP **mapa); //Função que liberta a memória alocada para o mapa
void check_end(MAP **mapa,int *stop, int largura, int altura); //Função que verifica se todas as posições do mapa já foram limpas

#endif // MAP_H_INCLUDED
