//include libraries
#include "Graphics.h"
#include "map.h"
#include "Robots.h"

void read_n_robots(FILE *fp, int *robots) //Lê e guarda o número de robots
{
    char line[small_string]; //Variável do tipo string auxiliar para guardar o valor que recebe de fgets

    fgets(line, small_string, fp); //Retira os valores do ficheiro e guarda-os nas variáveis globais por referência
    if( sscanf(line, "%d", &(*robots)) != 1) //Confirma também todos os parametros de sscanf
    { //Apresenta uma mensagem de erro e encerra o programa caso o ficheiro não esteja de acordo com o predefinido
        fprintf(stdout, "Erro no ficheiro .map, não corresponde à formatação predifinida\n");
        exit(EXIT_FAILURE);
    }
}

ROBOT* create_robot_list(ROBOT* head) //Aloca memória na lista para um robot
{
    head = (ROBOT*) malloc(sizeof (ROBOT)); //Aloca memória e devolve o endereço desta
    if(head==NULL) //Apresenta uma mensagem de erro e encerra o programa caso a memória seja mal alocada
    {
        fprintf(stdout, "Erro a alocar memória relativa à lista de robots\n");
        exit(EXIT_FAILURE);
    }

    head->seguinte=NULL; //Coloca o ponteiro para a estrutura seguinte a NULL
    return head;
}

ROBOT* set_robot_list(ROBOT* headR, ROBOT* novo_robot) //Coloca os robots por ordem na lista dinâmica
{
    ROBOT *aux=headR; //Declara um ponteiro auxiliar que toma o valor do ponteiro principal headR

    if(aux==NULL || strcmp(aux->name, novo_robot->name)>0)
    {   //Se o ponteiro auxiliar tiver o endereço NULL, ou o caracter do nome novo for menor que o seguinte,
        novo_robot->seguinte = headR; //aloca o novo robot no início da lista
        return novo_robot;
    }

    while(aux->seguinte!=NULL && strcmp(aux->seguinte->name, novo_robot->name)<0)
    {   //Avança na lista até encontrar um caracter seguinte maior que o novo
        aux=aux->seguinte;
    }

    novo_robot->seguinte = aux->seguinte; //Coloca a estrutura do novo robot no sítio certo
    aux->seguinte=novo_robot;
    return headR; //Retorna o ponteiro inicial
}

ROBOT* read_robots(FILE *fp, int robots, MAP **mapa, ROBOT *headR, int largura, int altura) //Lê e coloca os robots no mapa
{
    int auxX=0, auxY=0, bateria=0, i=1; //Variáveis inteiras auxiliares para armazenarem os valores vindos do ficheiro
    char robot_name[nameDim_MAX], line[small_string]; //Variável do tipo string auxiliar para guardar o valor que recebe de fgets
    ROBOT *novo_robot=NULL; //Ponteiro do tipo ROBOT para armazenar um robot de cada vez

    for(i=1;i<=robots;i++) //Percorre o número de robots
    {
        fgets(line, small_string, fp); //Retira os valores do ficheiro e guarda-os na variáveis locais para serem posteriormente guardados na memória
        if(sscanf(line, "%s [%d, %d] %d", robot_name, &auxX, &auxY, &bateria) == 4) //Confirma também todos os parametros de sscanf
        {
            novo_robot = create_robot_list(novo_robot); //Aloca a memória para o novo robot
            mapa[auxX][auxY].ocupied = OCUP; //Marca o mapa principal como ocupado e clean
            mapa[auxX][auxY].state = clean;
            strcpy(mapa[auxX][auxY].name, robot_name); //Copia o nome do robot para o mapa principal e para o ponteiro novo_robot
            strcpy(novo_robot->name, robot_name);
            novo_robot->bateria = bateria; //Define a bateria e a posição lida do ficheiro
            novo_robot->x = auxX;
            novo_robot->y = auxY;
            novo_robot->new_pos =new_posi; //Define a variável new_pos com new_posi, o que significa que não tem posição final
            novo_robot->mapa = create_map(largura , altura); //Cria uma matriz 2D para o novo_robot, ie, um mapa próprio
            novo_robot->mapa[auxX][auxY].state = clean; //Marca o mapa do robot como limpo
            novo_robot->limpos = NONE; //Inicializa a variável limpos e percorridos a 0
            novo_robot->percorridos = NONE;
            headR = set_robot_list(headR, novo_robot); //Coloca o novo robot na lista dinâmica ordenadamente
        }
        else //Apresenta uma mensagem de erro e encerra o programa caso o ficheiro não esteja de acordo com o predefinido
        {
            fprintf(stdout, "Erro no ficheiro .map, não corresponde à formatação predifinida\n");
            exit(EXIT_FAILURE);
        }
    }
    return headR; //Devolve o apontador principal
}

void MoveRobot(int *_xc, int *_yc, int xt, int yt) //Movimento de um robot quadrado a quadrado
{
	int xc, yc;
	double angle;

	// just copy the values
	xc = *_xc;
	yc = *_yc;

	// calculate the angle
	angle = atan2((double)(yc-yt), (double)(xt-xc));

	// calculate the new position
	*_xc = floor(xc + cos(angle)+0.5);
	*_yc = floor(yc - sin(angle)+0.5);
}

ROBOT* move_robots(int robots, MAP **mapa, ROBOT *headR, int largura, int altura, char *bat) //Atribuição de novas posições a todos os robots
{
    int antX=NONE, antY=NONE; //Variáveis que representam o x e y anteriores
    ROBOT* auxAct=NULL; //Variável auxiliar para o endereço atual da lista de robots inicializada com NULL
    ROBOT* auxAnt=NULL; //Variável auxiliar para o endereço anterior da lista de robots inicializada com NULL
    float BatMAX=NONE; //Variável que representa a bateria máxima e um número aleatório inicializadas a 0
    float random_bat=NONE;

    if(sscanf(bat, "%f", &BatMAX)!=1 || BatMAX <= BatMIN) //Confirma todos os parametros de sscanf e se o segundo argumento da função, MAX é válido
    {   //Apresenta uma mensagem de erro e encerra o programa caso a bateria não seja um caracter válido
        fprintf(stdout, "Erro no argumento, MAX battery dropout, caracter não corresponde a um número real maior que 0.1\n");
        exit(EXIT_FAILURE);
    }

    auxAct=headR; //Ponteiro auxiliar toma o valor do ponteiro principal headR
    while(auxAct!=NULL) //Corre enquanto a lista não chega ao fim, ie, auxAct é diferente de NULL
    {
        antX=auxAct->x; //Grava a posição atual numa variável auxiliar
        antY=auxAct->y;
        do
        {
            if(auxAct->new_pos==new_posi||(auxAct->x==auxAct->final_x && auxAct->y==auxAct->final_y))
            {   //Gera novas posições finais enquanto estas já tiverem sido limpas pelo próprio robot ou quando chega ao seu destino
                do
                {
                    auxAct->final_x=rand()%largura; //Gera x
                    auxAct->final_y=rand()%altura; //Gera y
                }while(auxAct->mapa[auxAct->final_x][auxAct->final_y].state==clean);
                auxAct->new_pos=old_posi;
            }

            if(auxAct->new_pos!=new_posi) //Gera posição seguinte se já tiver uma posição final válida
            {
                MoveRobot(&(auxAct->x), &(auxAct->y), auxAct->final_x, auxAct->final_y);
                if(mapa[auxAct->x][auxAct->y].ocupied==OCUP||mapa[auxAct->x][auxAct->y].state==obstacle) //Testa se a posição seguinte é válida
                {
                    auxAct->new_pos=new_posi; //Necessita de nova posição final caso a seguinte não seja válida
                    auxAct->x=antX;
                    auxAct->y=antY;
                }
            }
        }while(auxAct->new_pos==new_posi);

        auxAnt=auxAct; //O ponteiro anterior toma o valor do ponteiro atual
        mapa[antX][antY].ocupied=NOCUP; //Marca a posição anterior como desocupada
        mapa[auxAct->x][auxAct->y].ocupied=OCUP; //Marca o mapa principal como ocupado
        strcpy(mapa[auxAct->x][auxAct->y].name, auxAct->name); //Obtém o nome do robot para escrever no mapa
        if(mapa[auxAct->x][auxAct->y].state!=clean) //Incrementa a variável limpos se a nova posição ainda não tinha sido limpa
            auxAct->limpos ++;
        mapa[auxAct->x][auxAct->y].state=clean; //Marca o mapa principal como clean
        random_bat = generate_random(BatMAX); //Gera um valor aleatório entre 0.1 e BatMAX para decrementar a bateria
        auxAct->bateria=auxAct->bateria - random_bat; //Decrementa a bateria
        auxAct->percorridos ++; //Incrementa a variável percorridos
        auxAct->mapa[auxAct->x][auxAct->y].state=clean; //Marca o mapa do robot como limpo
        auxAct=auxAct->seguinte; //O ponteiro passa a apontar para a estrutura seguinte
        if(auxAnt->bateria<=no_batery) //Se já não tiver bateria o robot é removido
            headR=remove_robot(headR, mapa);
    }
    return headR; //Retorna o ponteiro principal
}

float generate_random(float BatMAX) //Gera um número aleatório entre 0.1 e BatMAX
{
    float range = BatMAX - BatMIN; //Calcula o range entre o valor máximo e mínimo
    float random = ((float)rand()) / (float)RAND_MAX; //Obtém um float entre 0 e 1
    float a = random * range; //Multiplica o float entre 0 e 1 pelo range
    return (BatMIN + a); //Retorna o resultado mais o valor mínimo
}

ROBOT* free_robots_list(ROBOT *headR) //Liberta a memória alocada para a lista de robots
{
    ROBOT *auxR=NULL; //Variável auxiliar para o endereço atual da lista de robots inicializada com NULL

    while(headR != NULL) //Corre enquanto o endereço do ponteiro principal headR não for nulo
    {
        auxR = headR; //auxR toma o valor de headR
        headR = headR->seguinte; //headR passa a apontar para a variável seguinte
        free(auxR); //Liberta uma posição de memória
    }

    headR=NULL; //Dá o valor nulo ao ponteiro headR
    return headR; //Retorna o valor do ponteiro principal
}

ROBOT* new_robot(int x, int y, int largura, int altura, ROBOT *headR, MAP **mapa) //Adiciona um robot
{
    char robot_name=NONE; //Variável que representa o caracter
    ROBOT *novo_robot=NULL; //Ponteiro para o novo robot inicializado a NULL
    ROBOT *auxR=headR;  //Declara um ponteiro auxiliar que toma o valor do ponteiro principal headR
    SDL_Event event; //Declara uma variável do tipo evento SDL

    novo_robot = create_robot_list(novo_robot); //Aloca memória para o novo robot
    fprintf(stdout,"Introduza um caracter para o nome do robot\n"); //Pede ao utilizador que introduza um caracter
    while( robot_name==NONE ) //Espera por um evento do teclado enquanto o caracter para o nome do robot não for alterado
    {
        while( SDL_PollEvent( &event ) )
        {
            if ( event.type == SDL_KEYDOWN )
                robot_name=(char)event.key.keysym.sym; //Guarda a tecla pressionada na variável char robot_name
        }
        sprintf(novo_robot->name, "R%c", robot_name); //Junta à variável robot_name o caracter 'R' e grava no nome do novo robot
    }

    while(auxR!=NULL) //Corre enquanto a lista não chega ao fim, ie, auxR é diferente de NULL
    {
        while(strcmp(auxR->name, novo_robot->name)==NONE||robot_name>'z'||robot_name<'a')
        {   //Testa o caracter introduzido pelo utilizador e pede um novo caso não esteja entre a e z ou já exista
            fprintf(stdout,"Introduza um caracter de 'a' a 'z' que ainda não existe para o nome do robot\n");
            robot_name=NONE;
            auxR=headR;
            while(robot_name == NONE)
            {
                while( SDL_PollEvent( &event ) )
                {
                    if ( event.type == SDL_KEYDOWN )
                        robot_name=(char)event.key.keysym.sym;
                }
            }
            sprintf(novo_robot->name, "R%c", robot_name);
        }
        auxR=auxR->seguinte; //Incrementa o endereço do ponteiro auxiliar para testar se o nome não é igual a nenhum dos já exostentes
    }
    //Informa o utilizador que o robot foi adicionado com sucesso
    fprintf(stdout,"Robot %s adicionado com sucesso, pode correr a simulação\n", novo_robot->name);
    mapa[x][y].ocupied = OCUP; //Marca o mapa principal como ocupado e clean
    mapa[x][y].state = clean;
    strcpy(mapa[x][y].name, novo_robot->name); //Copia o nome do robot para o mapa principal
    novo_robot->bateria = (rand()%(MAXbatRand - MINbatRand))+MINbatRand; //Atribui uma bateria aleatória entre 50 e 100 ao robot
    novo_robot->x = x; //Define a posição do robot conforme a escolhida através do rato pelo utilizador
    novo_robot->y = y;
    novo_robot->new_pos = new_posi; //Define a variável new_pos com new_posi, o que significa que não tem posição final
    novo_robot->mapa = create_map(largura , altura); //Cria uma matriz 2D para o novo_robot, ie, um mapa próprio
    novo_robot->mapa[x][y].state = clean; //Marca o mapa do robot como limpo
    novo_robot->limpos = NONE; //Inicializa a variável limpos e percorridos a 0
    novo_robot->percorridos = NONE;
    headR = set_robot_list(headR, novo_robot); //Coloca o novo robot na lista dinâmica ordenadamente

    return headR; //Devolve o ponteiro principal da lista dinâmica
}

ROBOT* remove_robot(ROBOT* headR, MAP** mapa) //Remove um robot com a bateria inferior ou igual a 0
{
    ROBOT *auxAct=headR; //Declara um ponteiro auxiliar actual que toma o valor do ponteiro principal headR
    ROBOT *auxAnt=NULL; //Declara um ponteiro auxiliar anterior que toma o valor do ponteiro principal headR

    while(auxAct!=NULL) //Corre enquanto a lista não chega ao fim, ie, auxR é diferente de NULL
    {
        if(auxAct->bateria<=NONE && auxAnt==NULL)
        {   //Se a bateria do robot selecionado for menor que 0 e este for o primeiro, ie, auxAnt==NULL
            mapa[auxAct->x][auxAct->y].ocupied=NOCUP; //O mapa passa a estar desocupado
            headR = auxAct->seguinte; //O apontador principal passa a apontar para o seguinte
            free(auxAct); //Liberta uma posição de memória
            return headR; //Devolve o ponteiro principal da lista dinâmica
        }
        else if(auxAct->bateria<=NONE && auxAnt!=NULL)
        {   //Se a bateria do robot selecionado for menor que 0 e este não for o primeiro, ie, auxAnt!=NULL
            mapa[auxAct->x][auxAct->y].ocupied=NOCUP; //O mapa passa a estar desocupado
            auxAnt->seguinte=auxAct->seguinte; //O ponteiro do auxiliar anterior que aponta para o seguinte passa a apontar para
            free(auxAct); //o ponteiro do auxiliar atual que também aponta para o seguinte, é libertada a posição de memória do auxAct
            return headR; //Devolve o ponteiro principal da lista dinâmica
        }
        else
        {   //No caso de ainda não estarmos perante o robot que acabou a bateria
            auxAnt=auxAct; //O auxiliar anterior passa a ter o endereço do atual
            auxAct=auxAct->seguinte; //O auxiliar atual passa a ter o endereço do seguinte ao atual
        }
    }
    return NULL; //Nunca acontece pois a função só é chamada se algum robot tiver a bateria igual ou menor que 0
}                //Logo o caso de fazer return NULL é impossível;

void create_file(ROBOT *headR) //Cria e escreve no ficheiro "estatísticas.txt"
{
    ROBOT *aux=headR; //Declara um ponteiro auxiliar que toma o valor do ponteiro principal headR
    FILE *fp; //Declaração da variável fp do tipo FILE* para abrir e escrever no ficheiro

    fp = fopen ("estatísticas.txt" , "w" ); //Criação e abertura, para escrita, do ficheiro estatísticas.txt.
    if (fp == NULL) //Mensagem de erro caso o ficheiro não exista ou não seja possível criar.
    {
        fprintf (stdout , "ERRO, ficheiro inexistente.\n");
        exit(EXIT_FAILURE);
    }

    while(aux!=NULL) //Corre enquanto a lista não chega ao fim, ie, aux é diferente de NULL
    {
        fprintf( fp, "\nNome do Robot: %s \n" , aux->name); //Imprime no ficheiro o nome do Robot
        fprintf( fp, "Número de quadrados limpos: %d \n" , aux->limpos); //Imprime no ficheiro o número de quadrados limpos
        fprintf( fp, "Número de quadrados percorridos: %d \n" , aux->percorridos); //Imprime no ficheiro o número de quadrados percorridos

        aux=aux->seguinte; //Muda o apontador auxiliar para apontar para a posição seguinte e repetir as impressões
    }

    fclose(fp); //Esta função fecha o ficheiro.
    if (fp != NULL) //Mensagem a informar que foi criado um ficheiro.
        fprintf (stdout , "Criado ficheiro estatísticas.txt com informaçẽs relevantes sobre a simulação.\n");
}
