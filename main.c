#include <SDL2/SDL.h>
#include <stdio.h>
#include "SDL2/SDL_ttf.h"
#define HEIGHT 1080
#define WIDTH 1920
#define WIDTH_MAT WIDTH/20
#define HEIGHT_MAT HEIGHT/20
#define SQUARE 20

void draw_cells(SDL_Surface *surface){
    for(int i = 0; i<surface->w; i+=20){
        for(int j = 0; j < surface->h;j+=20){
            SDL_Rect rect = {.x = i+1, .y=j+1, .h=19, .w = 19};
            SDL_FillRect(surface,&rect,0xffffff);
        }
    }
}

void Draw_grid(SDL_Surface *surface){
    for(int i = 0; i < surface->w; i+=20){
        SDL_Rect rect = {.x = i, .y=0, .h=HEIGHT, .w = 1};
        SDL_FillRect(surface,&rect,0x0000);
    }
    for(int i = 0; i < surface->h; i+=20){
        SDL_Rect rect2 = {.x = 0, .y=i, .h=1, .w = WIDTH};
        SDL_FillRect(surface,&rect2,0x0000);
    }
}

void update_click_matrix_sdl(int tab[HEIGHT_MAT][WIDTH_MAT], SDL_Surface *surface, int mouse_x, int mouse_y){
    int color;
    int matrix_x = mouse_x/20;
    int matrix_y = mouse_y/20;
    if (matrix_x < 0 || matrix_x >= WIDTH_MAT || matrix_y < 0 || matrix_y >= HEIGHT_MAT) {
        return;
    }
    if (tab[matrix_y][matrix_x] == 1) {
        tab[matrix_y][matrix_x] = 0;
        color = 0xFFFFFF;
    } else {
        tab[matrix_y][matrix_x] = 1;
        color = 0x000000;
    }
    SDL_Rect rect = {.x=matrix_x*20, .y=matrix_y*20, .h=SQUARE, .w=SQUARE};
    SDL_FillRect(surface,&rect,color);
}


int refresh_board(int tab[HEIGHT_MAT][WIDTH_MAT], SDL_Surface *surface, int population) {
    int new_tab[HEIGHT_MAT][WIDTH_MAT];
    
    for(int i = 0; i < HEIGHT_MAT; i++) {
        for(int j = 0; j < WIDTH_MAT; j++) {
            new_tab[i][j] = tab[i][j];
        }
    }
    
    for(int i = 0; i < HEIGHT_MAT; i++) {
        for(int j = 0; j < WIDTH_MAT; j++) {
            int live_cells = 0;
            
            for(int k = i-1; k <= i+1; k++) {
                for(int l = j-1; l <= j+1; l++) {
                    if(k == i && l == j) continue;
                    if (k >= 0 && k < HEIGHT_MAT && l >= 0 && l < WIDTH_MAT) {
                        if(tab[k][l] == 1) {
                            live_cells++;
                        }
                    }
                }
            }
            

            if (tab[i][j] == 1 && (live_cells < 2 || live_cells > 3)) {
                new_tab[i][j] = 0;  // Mort
            }
            else if (tab[i][j] == 0 && live_cells == 3) {
                new_tab[i][j] = 1;  // Naissance
            }
        }
    }
    population = 0;
    for(int i = 0; i < HEIGHT_MAT; i++) {
        for(int j = 0; j < WIDTH_MAT; j++) {
            tab[i][j] = new_tab[i][j];
            int color = 0xFFFFFF;
            if (tab[i][j] == 1){
                color = 0x000000;
                population++;
            }
            
            SDL_Rect rect = {
                .x = j * 20,
                .y = i * 20,
                .h = SQUARE,
                .w = SQUARE
            };
            SDL_FillRect(surface, &rect, color);
        }
    }
    return population;
}

void display_text(SDL_Surface* surface, TTF_Font* font, char* text, int x, int y) {
    SDL_Color noir = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, noir);
    if (textSurface) {
        SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};
        SDL_BlitSurface(textSurface, NULL, surface, &destRect);
        SDL_FreeSurface(textSurface);
    }
}


int main(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Game of life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH ,HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    Draw_grid(surface);
    draw_cells(surface);
    int tab[HEIGHT_MAT][WIDTH_MAT];
    //Init le tableau 0 = blanc (0xffffff)
    for(int i = 0; i < HEIGHT_MAT; i++) {      // i pour les lignes y
        for(int j = 0; j < WIDTH_MAT; j++) {   // j pour les colonnes x
            tab[i][j] = 0;
        }
    }

    if(TTF_Init()==-1) 
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
    if (!font) {
        fprintf(stderr, "TTF_OpenFont failed: %s\n", TTF_GetError());
        return 1;
    }
    
    SDL_UpdateWindowSurface(window);
    int running = 1;
    int mouse_x = 0;
    int mouse_y = 0;
    int generation = 0; 
    int population = 0;
    int simulation_running = 0;
    while(running){
        SDL_Event ev;
        while(SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
            {
                SDL_DestroyWindow(window);
                TTF_CloseFont(font);
                TTF_Quit();
                SDL_Quit();
                return 2;
            }
            
            switch (ev.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    mouse_x = ev.button.x;
                    mouse_y = ev.button.y;
                    update_click_matrix_sdl(tab,surface,mouse_x,mouse_y);
                    Draw_grid(surface);
                    char generation_text[50];
                    char population_text[50];
                    sprintf(generation_text, "Generation: %d", generation);
                    sprintf(population_text, "Population: %d", population);
                    display_text(surface, font, generation_text, 10, 10);
                    display_text(surface, font, population_text, 10, 30);
                    SDL_UpdateWindowSurface(window);
                    break;

                case SDL_KEYDOWN:
                    if (ev.key.keysym.sym == SDLK_SPACE) {
                        simulation_running = !simulation_running;
                    }
                    break;
                
                default:
                    break;
            }
        }
        if (simulation_running) {
            population = refresh_board(tab, surface, population);
            Draw_grid(surface);
            char generation_text[50];
            char population_text[50];
            sprintf(generation_text, "Generation: %d", generation);
            sprintf(population_text, "Population: %d", population);
            display_text(surface, font, generation_text, 10, 10);
            display_text(surface, font, population_text, 10, 30);
            SDL_UpdateWindowSurface(window);
            SDL_Delay(1);
            generation++;
        }
    }
}