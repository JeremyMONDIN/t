#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "moteur_rendu.h"
#include "structure.h"

//permet de dessiner l'arrière plan de l'image
void dessiner_fenetre(SDL_Window * window, SDL_Renderer * renderer, 
                      SDL_Texture * texture){
    if (print_var) printf("dessiner fenetre\n");

    SDL_Rect source = {0}, 
             window_dimensions = {0}, 
             destination = {0};

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h);
    SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);

    destination = window_dimensions;

    SDL_RenderCopy(renderer, texture, &source, &destination);
}

//affiche un PNJ
void afficher_PNJ(SDL_Window * window, SDL_Renderer * renderer,
                  pnj_t * entite){
    //if (print_var) printf("afficher_pnj\n");
    SDL_Rect source = {0, 0, 0, 0},
             position = entite->pos;
    
    SDL_QueryTexture(entite->espece->visuel, NULL, NULL, &source.w, &source.h);
    
    SDL_RenderCopy(renderer, entite->espece->visuel, &source, &position);
}

//affiche un decor
void afficher_decor(SDL_Window * window, SDL_Renderer * renderer,
                  decor_t * decor){
    //if (print_var) printf("afficher_decor\n");
    SDL_Rect source = {0, 0, 0, 0},
             position = decor->pos;
    
    SDL_QueryTexture(decor->visu, NULL, NULL, &source.w, &source.h);
    
    SDL_RenderCopy(renderer, decor->visu, &source, &position);
    return;
}

//à partir de là c'est des fonctions de test
void end_sdl(char ok, char const* msg, SDL_Window * window, SDL_Renderer * renderer){
    if (print_var) printf("end_sdl\n");
    char msg_formated[255];
    int l;

    if(!ok){
        strncpy(msg_formated,msg,250);
        l = strlen(msg_formated);
        strcpy(msg_formated + l, " : %s\n");

        SDL_Log(msg_formated, SDL_GetError());
    }

    if (renderer != NULL){
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    if (window != NULL){
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();

    if(!ok){
        exit(EXIT_FAILURE);
    }
}

SDL_Texture* load_texture_from_image(char  *  file_image_name, SDL_Window *window, SDL_Renderer *renderer ){
    SDL_Surface *my_image = NULL;           // Variable de passage
    SDL_Texture* my_texture = NULL;         // La texture

    my_image = IMG_Load(file_image_name);   // Chargement de l'image dans la surface
                                            // image=SDL_LoadBMP(file_image_name); fonction standard de la SDL, 
                                            // uniquement possible si l'image est au format bmp */
    if (my_image == NULL) end_sdl(0, "Chargement de l'image impossible", window, renderer);
   
    my_texture = SDL_CreateTextureFromSurface(renderer, my_image); // Chargement de l'image de la surface vers la texture
    SDL_FreeSurface(my_image);                                     // la SDL_Surface ne sert que comme élément transitoire 
    if (my_texture == NULL) end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);

    return my_texture;
  }


/*int main(){
    SDL_Window   * window   = NULL;
    SDL_Renderer * renderer = NULL; 
    SDL_Texture * background = NULL;
    SDL_Texture * decoration = NULL;
    SDL_Surface * my_image = NULL;

    decor_t * decor = malloc(sizeof(decor_t));
    decor->pos.x = 10;
    decor->pos.y = 100;
    decor->pos.w = 64;
    decor->pos.h = 64;

    if (SDL_Init(SDL_INIT_VIDEO) == -1)  // SDL_INIT_EVERYTHING 
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        return EXIT_FAILURE;   
    } 

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Erreur IMG_Init : %s\n", IMG_GetError());
    }
    window = SDL_CreateWindow("Exemple SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 
                                    //SDL_WINDOW_FULLSCREEN ); 
                                     SDL_WINDOW_SHOWN |  SDL_WINDOW_RESIZABLE);

    if (window == 0) {
        fprintf(stderr, "Erreur a la creation de la fenetre : %s\n", SDL_GetError());
        SDL_Delay(5000); 
    } else {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED ); // | SDL_RENDERER_PRESENTVSYNC 
        if (renderer == 0) {
            fprintf(stderr, "Erreur de creation d'un renderer : %s\n", SDL_GetError());
        }
    }
    
    //Background
    my_image = IMG_Load("assets/backgrounds/backgroundColorFall.png");
    if(!my_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
    }

    background = SDL_CreateTextureFromSurface(renderer, my_image); // arrière plan de l'image
    SDL_FreeSurface(my_image);

    if(!background) {
        printf("SDL_CreateTextureFromSurface: %s\n", IMG_GetError());
    }

    //decor
    my_image = IMG_Load("assets/decors/Carotte.png");
    if(!my_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
    }

    decoration = SDL_CreateTextureFromSurface(renderer, my_image);
    SDL_FreeSurface(my_image);


    //boucle d'évènement
    SDL_Event event;
    int running = 1;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        SDL_RenderClear(renderer);
        dessiner_fenetre(window, renderer, background);
        afficher_decor(window, renderer, decor, decoration);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(decoration);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    if (background)     SDL_DestroyTexture(background);
    free(decor);
    SDL_Quit();
    return 0;

}*/