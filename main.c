/*
    Projecto: iClean
    Autores: Miguel Santos - 81347 / Vasco Amaral - 81532
    Objectivo: Simulação de limpeza de uma divisão com obstáculos e uma frota de robots pre-definida,
    tudo dado por um ficheiro .map. O utilizador é ainda capaz de gerir a frota através de diversas funcionalidades.
    Data de Realização: 14 de Maio de 1015 a 22 de Maio de 2015
*/

//include libraries
#include "Graphics.h"
#include "map.h"
#include "Robots.h"

int main( int argc, char *argv[])
{
    SDL_Event event;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	srand(time(NULL)); //Declaração da função srand
	int delay = initial_delay; //Define o delay inicial;

    int altura=0, largura=0, obstaculos=0; //Declaração das variáveis globais altura, largura e obstaculos
    int robots=0; //Declaração da variável global robots, que conta o número de robots
    int pause=running, reset=NRESET, new_rob=not_new_rob; //Declaração das variáveis que permitem pausar, reinicializar o programa e adicionar robots
    int new_robx=0, new_roby=0; //Declaração das variáveis que permitem definir a posição dos novos robots no mapa
    int stop = RUN; //Declaração da variável que define quando o programa encerra
    FILE *fp;   //Declaração da variável fp do tipo FILE* para abrir o ficheiro que contém o mapa
    MAP **mapa=NULL; //Declaração da variável principal mapa, do tipo MAP** para representar uma matriz 2D
    ROBOT *headR=NULL; //Declaração da variável principal headR, do tipo ROBOT* para aceder à lista de robots

    if(argc != 3)
    {
        fprintf(stdout, "Erro na definição dos parâmetros, parâmetros insuficientes.\nInsira o nome do mapa e a 'MAX battery dropout' ao correr o programa\n");
        exit(EXIT_FAILURE);
    }

    while(stop == RUN)
    {
        open_file(&fp, argv[1]); //Abre o ficheiro do mapa no modo leitura
        read_firstline(fp, &largura, &altura, &obstaculos); //Lê e guarda os valores da primeira linha do ficheiro
        mapa = create_map(largura , altura); //Cria o mapa principal na variável "mapa"
        mapa = read_obstacles(fp, mapa, obstaculos); //Lê e coloca os obstáculos no mapa principal
        read_n_robots(fp, &robots); //Lê e guarda o número de robots
        headR = read_robots(fp, robots, mapa, headR, largura, altura); //Lê e coloca no mapa os robots
        fclose(fp); //Fecha o ficheiro

        if(reset!=RESET) //Apenas inicializa os gráficos se não estiver a fazer um reset
        {
            // initialize graphics
            if ( !InitEverything(largura, altura, &window, &renderer) )
                return -1;
        }
        else //Reinicializa a variável reset a 0
            reset=NRESET;

        //Render initial map
        RenderMap(largura, altura, mapa, headR, renderer);
        // add a delay
        SDL_Delay( delay );

        while(stop == RUN)
        {
            // while there's events to handle
            while( SDL_PollEvent( &event ) )
            {
                if( event.type == SDL_QUIT )
                {
                    // quit the program and free memory
                    fprintf(stdout,"Programa encerrado com sucesso\n");
                    mapa=free_map(largura, mapa);
                    headR=free_robots_list(headR);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    exit(EXIT_SUCCESS);
                }
                else if ( event.type == SDL_KEYDOWN )
                {
                    switch ( event.key.keysym.sym )
                    {
                        case SDLK_DOWN:
                            // Dimuinição da velocidade.
                            delay = delay+delay_change;
                            break;
                        case SDLK_UP:
                            // Aumento da velocidade, inserido num "if" para o delay nunca ser menor ou igual a zero.
                            if(delay>delay_change)
                                delay = delay-delay_change;
                            break;
                        case SDLK_q:
                            // quit the program and free memory
                            fprintf(stdout,"Programa encerrado com sucesso\n");
                            mapa=free_map(largura, mapa);
                            headR=free_robots_list(headR);
                            SDL_DestroyRenderer(renderer);
                            SDL_DestroyWindow(window);
                            SDL_Quit();
                            exit(EXIT_SUCCESS);
                        case SDLK_p:
                            // pause the program
                            pause=!pause; //pause flips between 1 and 0
                            break;
                        case SDLK_e:
                            // creates file
                            create_file(headR);
                            break;
                        case SDLK_i:
                            // reset the program
                            reset=RESET;
                            break;
                        case SDLK_a:
                            // add robot
                            if(pause==PAUSE) //Only if in pause
                            {
                                new_rob=new_rob_place;
                                fprintf(stdout,"Clique onde pretende adicionar o robot\n");
                            }
                            break;
                    }
                }
                else if ( event.type == SDL_MOUSEBUTTONDOWN )
                {
                    if(pause==PAUSE && new_rob==new_rob_place)
                    {
                        SDL_GetMouseState(&new_robx, &new_roby); //Obtém a posição do clique do rato
                        new_robx=new_robx/SQUARE_SIZE;
                        new_roby=new_roby/SQUARE_SIZE;
                        new_rob=new_rob_name;

                        if(mapa[new_robx][new_roby].ocupied==OCUP||mapa[new_robx][new_roby].state==obstacle)
                        { //Testa se a posição é inválida, no caso de ser, new_rob=new_rob_place para selecionar novamente a posição
                            fprintf(stdout,"Posição inválida, selecione uma posição desocupada\n");
                            new_rob=new_rob_place;
                        }
                    }
                }
            }

            if(pause==PAUSE && new_rob==new_rob_name) //Coloca um novo robot na posição já definida
            {
                headR = new_robot(new_robx, new_roby, largura, altura, headR, mapa); //Devolve headR para o caso deste ser alterado
                new_rob=not_new_rob; //reinicializa a variável new_rob a 0
            }

            if(reset==RESET) //Reinica o programa
            {
                mapa=free_map(largura, mapa); //Dealoca a memória do mapa
                headR=free_robots_list(headR); //Dealoca a memória da lista dinâmica
                break; //Faz break do ciclo da movimentação e reinicia todo o programa
            }

            if(pause==running) //Apenas movimenta os robots se a variável pause não estiver ativa
            {
                headR = move_robots(robots, mapa, headR, largura, altura, argv[2]); //Função principal de movimentação dos robots
                RenderMap(largura, altura, mapa, headR, renderer); //Atualiza o mapa a cada nova posição de cada robot
                // add a delay
                SDL_Delay( delay );
                check_end(mapa, &stop, largura, altura); //Verifica se já todos os quadrados estão limpos
            }
        }
    }
    // quit the program and free memory
    fprintf(stdout,"Programa encerrado com sucesso\n");
    mapa=free_map(largura, mapa);
    headR=free_robots_list(headR);
 	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

    return 1;
}
