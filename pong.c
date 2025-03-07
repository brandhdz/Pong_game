#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

typedef struct {
  int x0;
  int y0;
} velocity_ball;

velocity_ball init() {

  velocity_ball v_b;

  v_b.x0 = rand() % 7;
  v_b.y0 = rand() % 7;

  return v_b; 
}

int init_sign(float r_or_l) {

  int sign = 1;
  
  if (r_or_l > 0.5) {
      sign *= 1;
    } else {
      sign *= -1;
    }
  
  return sign;
}

int main() {

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Rect p1 = {50, 50, 20, 80};
  SDL_Rect p2 = {570, 350, 20, 80};
  SDL_Rect ball = {320, 240, 10, 10};

  srand(time(NULL));

  int velocity_player = 30;
  int velocity_ia = 4;
  
  velocity_ball v_b = init();
  float r_or_l = (float) rand() / RAND_MAX;
  int sign = init_sign(r_or_l);
  v_b.x0 *= sign;
  
  SDL_Event event;
  int running = 1;

  while (running) {
    
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
	running = 0;
      } else if (event.type == SDL_KEYDOWN) {

	if (p1.y < 30) {

	  switch (event.key.keysym.sym) {

	  case SDLK_UP:
	    p1.y -= 0;
	    break;

	  case SDLK_DOWN:
	    p1.y += velocity_player;
	    break;
	  }
	  
	} else if (p1.y > 370) {

	  switch (event.key.keysym.sym) {

	  case SDLK_UP:
	    p1.y -= velocity_player;
	    break;

	  case SDLK_DOWN:
	    p1.y += 0;
	    break;
	  }

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
    
    p2.y -= velocity_ia;
    
    if (p2.y < 15) {
      velocity_ia *= -1;
    } else if (p2.y > 370) {
      velocity_ia *= -1;
    }

    ball.x += v_b.x0;
    ball.y += v_b.y0;
    
    if (ball.y < 15) {
      v_b.y0 *= -1;
    } else if (ball.y > 465) {
      v_b.y0 *= -1;
    }

    if (ball.x < p1.x + p1.w && ball.x > p1.x && ball.y > p1.y && ball.y < p1.y + p1.h) {
      v_b.x0 *= -1;
    } else if (ball.x + ball.w > p2.x && ball.x + ball.w < p2.x + p2.w && ball.y > p2.y && ball.y < p2.y + p2.h) {
      v_b.x0 *= -1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &p1);
    SDL_RenderFillRect(renderer, &p2);
    SDL_RenderFillRect(renderer, &ball);
    SDL_RenderDrawLine(renderer, 320, 40, 320, 440);
    
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
    
  }
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
  
}
