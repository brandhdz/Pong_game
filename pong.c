#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Definimos esta estructura como una especie de vector velocidad inicial para la pelota
typedef struct {
  int x0;
  int y0;
} velocity_ball;

// Inicializamos las componentes del vector de manera aleatoria, incluyendo el signo para vx para que no siempre empiece del mismo lado
velocity_ball init() {
  
  velocity_ball v_b;
  float r_or_l = (float) rand() / RAND_MAX;
  int sign = 1;

  srand(time(NULL));
  v_b.x0 = rand() % 5 + 1; // Quitamos al 0 del rango para que avance no se quede en el centro
  srand(time(NULL));
  v_b.y0 = rand() % 5;
  
  if (r_or_l > 0.5) {
      sign *= 1;
    } else {
      sign *= -1;
    }
  
  v_b.x0 *= sign;
  
  return v_b; 
}

// Definimos una función para colocar los marcadores en la pantalla
void ScoreText(SDL_Renderer *renderer, TTF_Font *ttf_font, const char *text, int x, int y) {
  SDL_Color color = {255, 255, 255}; // Color blanco
  SDL_Surface* surface = TTF_RenderText_Solid(ttf_font, text, color); // Creamos la imagen
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); // Convertimos en textura
  
  SDL_Rect txt_pos = {x, y, surface->w, surface->h}; // Escogemos la posición
  SDL_RenderCopy(renderer, texture, NULL, &txt_pos); // La renderizamos en pantalla

  // Liberamos la memoria
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

int main() {

  // Inicializamos el video y la fuente
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  // Creamos la pantalla
  SDL_Window* window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN); 

  // Usamos un rederizador para las animaciones
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Creamos al jugador 1, 2 y la pelota
  SDL_Rect p1 = {50, 50, 20, 80};
  SDL_Rect p2 = {570, 350, 20, 80};
  SDL_Rect ball = {320, 240, 10, 10};

  int velocity_player = 30; // Definimos la velocidad del jugador
  int velocity_ia = 4; // Definimos la velocidad del jugador 2 (CPU)

  velocity_ball v_b = init(); // Inicializamos la velocidad de la pelota

  // Inicializamos los marcadores
  int score_p1 = 0;
  int score_p2 = 0;

  // Cargamos la fuente
  TTF_Font *font = TTF_OpenFont("./Fuente/04b_19/04B_19__.ttf", 100);
  if (!font) {
    printf("Error al cargar la fuente: %s\n", TTF_GetError());
    return 1;
  }

  // Definimos la variable de eventos para registrar las teclas precionadas y una variable true para determinar si el juego esta en marcha
  SDL_Event event;
  int running = 1;

  while (running) {
    
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
	running = 0;  // Salimos del programa si lo cerramos
      } else if (event.type == SDL_KEYDOWN) { // Registramos las teclas

	// Si el jugador llega al borde superior se para
	if (p1.y < 30) {

	  switch (event.key.keysym.sym) {

	  case SDLK_UP:
	    p1.y -= 0;
	    break;

	  case SDLK_DOWN:
	    p1.y += velocity_player;
	    break;
	  }

	  // Si el jugador llega al borde inferior se para
	} else if (p1.y > 370) {

	  switch (event.key.keysym.sym) {

	  case SDLK_UP:
	    p1.y -= velocity_player;
	    break;

	  case SDLK_DOWN:
	    p1.y += 0;
	    break;
	  }

	  // Movimiento natural del jugador
	} else {
	  
	  switch (event.key.keysym.sym) {

	  case SDLK_UP:
	    p1.y -= velocity_player;
	    break;

	  case SDLK_DOWN:
	    p1.y += velocity_player;
	    break;
	  }
	}
      }
    }

    // Movimiento basico para el jugador 2 (CPU), solo se mueve y rebota en los bordes
    p2.y -= velocity_ia;
    
    if (p2.y < 15) {
      velocity_ia *= -1;
    } else if (p2.y > 370) {
      velocity_ia *= -1;
    }

    // Fijamos la velocidad inicial de la pelota 
    ball.x += v_b.x0;
    ball.y += v_b.y0;

    // Definimos la colisión de la pelota con los bordes superior e inferior
    if (ball.y < 15) {
      v_b.y0 *= -1;
    } else if (ball.y > 465) {
      v_b.y0 *= -1;
    }

    // Definimos la colisión de la pelota con los jugadores
    if (ball.x < p1.x + p1.w && ball.x > p1.x && ball.y > p1.y && ball.y < p1.y + p1.h) {
      v_b.x0 *= -1;
    } else if (ball.x + ball.w > p2.x && ball.x + ball.w < p2.x + p2.w && ball.y > p2.y && ball.y < p2.y + p2.h) {
      v_b.x0 *= -1;
    }

    // Definimos el sistema de puntos, si la pelota pasa a los jugadores se marca un punto y la posición de la pelota se reinicia en el centro con una nueva velocidad inicial
    if (ball.x < 0) {
      score_p2++;
      // printf("Player 2: %i\n", score_p2);

      ball.x = 320;
      srand(time(NULL));
      ball.y = rand() % 480;

      velocity_ball v_b = init();
      
      ball.x += v_b.x0;
      ball.y += v_b.y0;
    }

    if (ball.x > 640) {
      score_p1++;
      // printf("Player 1: %i\n", score_p1);
      
      ball.x = 320;
      srand(time(NULL));
      ball.y = rand() % 480;

      velocity_ball v_b = init();
      
      ball.x += v_b.x0;
      ball.y += v_b.y0;
    }

    // Renderizamos el fondo
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Color negro
    SDL_RenderClear(renderer); // Limpiamos la pantalla

    // Renderizamos los objetos en pantalla
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Color blanco
    SDL_RenderFillRect(renderer, &p1); // Jugador 1
    SDL_RenderFillRect(renderer, &p2); // Juagador 2
    SDL_RenderFillRect(renderer, &ball); // Pelota
    SDL_RenderDrawLine(renderer, 320, 40, 320, 440); // La línea central de division

    // Mostramos los marcadores
    char text_1[5];
    sprintf(text_1, "%d", score_p1);
    ScoreText(renderer, font, text_1, 150, 50);

    char text_2[5];
    sprintf(text_2, "%d", score_p2);
    ScoreText(renderer, font, text_2, 450, 50);

    // Actualizamos la pantalla con los cambios y con un tiempo de espera de 10mls
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
    
  }

  // Liberamos los recursos utilizados
  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
  
  return 0;
  
}
