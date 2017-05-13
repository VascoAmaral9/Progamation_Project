#include "Graphics.h"
#include "map.h"
#include "Robots.h"

// Nomes e números dos alunos
const char myName1[] = "Miguel Santos";
const char myName2[] = "Vasco Amaral";
const char myNumber1[] = "IST81347";
const char myNumber2[] = "IST81532";

/**
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Renderer* _renderer)
{
	SDL_Texture *text_IST;
	SDL_Surface *img_IST;
	SDL_Rect boardPos;

	// renders IST Logo
	img_IST = SDL_LoadBMP("ist_logo.bmp");
	if (img_IST == NULL)
	{
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		exit(-5);
	}
	// square where the Logo is placed
	boardPos.x = x;
	boardPos.y = y;
	boardPos.w = img_IST->w;
	boardPos.h = img_IST->h;

	// creates a texture and renders it in the screen
	text_IST = SDL_CreateTextureFromSurface(_renderer, img_IST);
	SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

	// destroy texture and surface
	SDL_DestroyTexture(text_IST);
	SDL_FreeSurface(img_IST);
	return img_IST->h;
}

/**
 * RenderText function: Renders the IST Logo on the window screen
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *font, SDL_Renderer* _renderer)
{
	SDL_Color color = { 0, 0, 0 };
	SDL_Surface *text_surface;
	SDL_Texture *text_texture;
	SDL_Rect solidRect;

	solidRect.x = x;
	solidRect.y = y;

	// creates a surface with some text
	text_surface = TTF_RenderText_Blended(font,text,color);
	if(!text_surface)
	{
	    printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
	    exit(-5);
	}

	// creates a texture from the surface and renders it !
	text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
	SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
	SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

	// destroy texture and surface
	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
	return solidRect.h;
}

/**
 * RenderMap function: Renders the map on the window screen according to their size
 * \param largura number of squares to render (width)
 * \param altura number of squares to render (height)
 * \param pos_robot position of the robot (ID of the square)
 * \param robot_name name of the robot (max. 2 letters)
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderMap(int largura, int altura, MAP **mapa, ROBOT *headR, SDL_Renderer* _renderer)
{
	TTF_Font *sans;
	TTF_Font *serif;
	SDL_Rect gridPos;
	int auxX=0, auxY=0, height=0, id = 0;
	ROBOT *aux=headR;
	char nome[nameDim_MAX];
	char bateria[batery];
	float bat=0;

	// set color of renderer to some color
	SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

	// clear the window
	SDL_RenderClear( _renderer );

	// opens a font style and sets a size
	sans = TTF_OpenFont("FreeSans.ttf", 18);
	serif = TTF_OpenFont("FreeSerif.ttf", 14);
	if(!sans || !serif)
	{
	    printf("TTF_OpenFont: %s\n", TTF_GetError());
	    exit(-5);
	}

	// render the IST Logo
	height = RenderLogo(largura*SQUARE_SIZE+1*MARGIN, 0, _renderer);
	// render the student name 1

	height += RenderText(largura*SQUARE_SIZE+9*MARGIN, height, myName1, serif, _renderer);

	// render the student number 1
	height += RenderText(largura*SQUARE_SIZE+13*MARGIN, height, myNumber1, serif, _renderer);

	// render the student name 2
    height += RenderText(largura*SQUARE_SIZE+9*MARGIN, height, myName2, serif, _renderer);

	// render the student number 2
	height += RenderText(largura*SQUARE_SIZE+13*MARGIN, height, myNumber2, serif, _renderer);

    //render the robot list
    while(aux!=NULL)
    {
        bat = aux->bateria;
        strcpy(nome, aux->name);
        snprintf(bateria, batery, "%f", bat);
        RenderText(largura*SQUARE_SIZE+3*MARGIN, height, nome, serif, _renderer);
        height += RenderText(largura*SQUARE_SIZE+13*MARGIN, height, bateria, serif, _renderer);
        aux=aux->seguinte;
    }

	// grid position
	gridPos.w = SQUARE_SIZE;
	gridPos.h = SQUARE_SIZE;
	gridPos.y = 0;

	// iterate over all squares
	for (auxY = 0; auxY < altura; auxY++)
	{
		gridPos.x = 0;
		for (auxX = 0; auxX < largura; auxX++)
		{
		    // Writes dirty square when mapa.state=dirty
		    if (mapa[auxX][auxY].state == dirty)
            {
			SDL_SetRenderDrawColor( _renderer, 221, 161, 135, 255 );
			SDL_RenderFillRect( _renderer, &gridPos );
			SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
			SDL_RenderDrawRect( _renderer, &gridPos );
            }
            // Writes clean square when mapa.state=clean
            else if (mapa[auxX][auxY].state == clean)
            {
            SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
			SDL_RenderFillRect( _renderer, &gridPos );
			SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
			SDL_RenderDrawRect( _renderer, &gridPos );
            }
            // Writes obstacle when mapa.state=obstacle
            else
            {
            SDL_SetRenderDrawColor( _renderer, 255, 140, 0, 255 );
			SDL_RenderFillRect( _renderer, &gridPos );
			SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
			SDL_RenderDrawRect( _renderer, &gridPos );
            }
            // Writes the destiny square for each robot
            aux=headR; //Ponteiro auxiliar que toma o valor do ponteiro inicial da lista de robots
            while(aux!=NULL) //Corre enquanto a lista não chega ao fim, ie, aux é diferente de NULL
            {
                if (aux->final_x==auxX && aux->final_y==auxY) //Escreve uma posição de destino para cada robot
                {
                    SDL_SetRenderDrawColor( _renderer, 0, 255, 0, 255 );
                    SDL_RenderFillRect( _renderer, &gridPos );
                    SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 255 );
                    SDL_RenderDrawRect( _renderer, &gridPos );
                }
                aux=aux->seguinte; //Muda o apontador auxiliar para apontar para a posição seguinte
            }
            //Puts a robot in a square if mapa.ocupied=OCUP
			if (mapa[auxX][auxY].ocupied == OCUP)
			{
				RenderText(gridPos.x+MARGIN, gridPos.y, mapa[auxX][auxY].name, sans, _renderer);
			}
			gridPos.x += SQUARE_SIZE;
			id++;
		}
		gridPos.y += SQUARE_SIZE;
	}

	set_path(_renderer, headR); //Função que desenha uma linha unindo as posiçoes atuais e de destino dos robots

	// render the changes above
	SDL_RenderPresent( _renderer);

	// destroy everything
	TTF_CloseFont(sans);
	TTF_CloseFont(serif);
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
int InitEverything(int largura, int altura, SDL_Window** _window, SDL_Renderer** _renderer)
{
    int width=0, height=0;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	// calculate the window size
	width = SQUARE_SIZE*largura;
	height = SQUARE_SIZE*altura;

	if ( !InitSDL() )
		return 0;

	if ( !InitFont() )
		return 0;

	window = CreateWindow(width, height);
	if ( window == NULL )
		return 0;

	renderer = CreateRenderer(width, height, window);
	if ( renderer == NULL )
		return 0;

	*_window = window;
	*_renderer = renderer;

	return 1;
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
int InitSDL()
{
	// init SDL library
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		printf(" Failed to initialize SDL : %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
int InitFont()
{
	// init font library
	if(TTF_Init()==-1)
	{
	    printf("TTF_Init: %s\n", TTF_GetError());
	    return 0;
	}

	return 1;
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
	SDL_Window* window = NULL;
	// init window
	window = SDL_CreateWindow( "iClean", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );

	if ( window == NULL )
	{
		printf("Failed to create window : %s\n", SDL_GetError());
		return NULL;
	}

	return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window* _window)
{
	SDL_Renderer* renderer;
	// init renderer
	renderer = SDL_CreateRenderer( _window, -1, 0 );

	if ( renderer == NULL )
	{
		printf("Failed to create renderer : %s", SDL_GetError());
		return NULL;
	}
	// set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

	return renderer;
}

void set_path(SDL_Renderer* _renderer, ROBOT *headR) //Desenha o caminho que o robot deve percorrer
{
    ROBOT *auxR=headR; //Declara um ponteiro auxiliar que toma o valor do ponteiro principal headR

    while(auxR!=NULL) //Corre o algoritmo enquanto o ponteiro auxiliar não for nulo, ie, até acabarem as posições de memória
    { //Função SDL que desenha uma linha entre 2 pontos
        SDL_RenderDrawLine(_renderer, ((auxR->x)*SQUARE_SIZE+(SQUARE_SIZE/2)), ((auxR->y)*SQUARE_SIZE+(SQUARE_SIZE/2)), ((auxR->final_x)*SQUARE_SIZE+(SQUARE_SIZE/2)), ((auxR->final_y)*SQUARE_SIZE+(SQUARE_SIZE/2)));
        auxR=auxR->seguinte;
    }
}
