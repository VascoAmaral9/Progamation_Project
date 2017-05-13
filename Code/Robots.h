#ifndef ROBOTS_H_INCLUDED
#define ROBOTS_H_INCLUDED

//include libraries
#include "map.h"

typedef struct dados_robot {
    char name[nameDim_MAX]; //Robot name
    int x; //actual x
    int y; //actual y
    int final_x; //final x
    int final_y; //final y
    int percorridos; //número de quadrados percorridos
    int limpos; //número de quadrados limpos
    float bateria; //bateria atual
    int new_pos; //1=necessita de nova posiçao, 0=já tem nova posição
    MAP** mapa; //mapa de cada robot
    struct dados_robot *seguinte; //apontador para o elemento seguinte
    } ROBOT;

void read_n_robots(FILE *fp, int *robots); //Função que lê e guarda o número de robots
ROBOT* create_robot_list(ROBOT* head); //Função que aloca memória na lista para um robot
ROBOT* set_robot_list(ROBOT* headR, ROBOT* novo_robot); //Função que coloca os robots por ordem na lista dinâmica
ROBOT* read_robots(FILE *fp, int robots, MAP **mapa, ROBOT *headR, int largura, int altura); //Função que lê e coloca os robots no mapa
void MoveRobot(int *_xc, int *_yc, int xt, int yt); //Função para movimentar um robot quadrado a quadrado
ROBOT* move_robots(int robots, MAP **mapa, ROBOT *headR, int largura, int altura, char *bat); //Função para movimentar os robots
float generate_random(float BatMAX); //Função que gera um número aleatório entre 0.1 e BatMAX
ROBOT* free_robots_list(ROBOT *headR); //Função que liberta a memória alocada para a lista de robots
ROBOT* new_robot(int x, int y, int largura, int altura, ROBOT *headR, MAP **mapa); //Função que adiciona um robot
ROBOT* remove_robot(ROBOT* headR, MAP** mapa); //Função para remover um robot com a bateria inferior ou igual a 0
void create_file(ROBOT *headR); //Função que cria e escreve no ficheiro "estatísticas.txt"

#endif // ROBOTS_H_INCLUDED
