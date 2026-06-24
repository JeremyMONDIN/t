#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "moteur_rendu.h"
#include "structure.h"


//int print_var = 0;
// permet de dessiner l'arrière plan de l'image
void dessiner_fenetre(SDL_Window *window, SDL_Renderer *renderer,
                      SDL_Texture *texture)
{
    if (print_var)
        printf("dessiner fenetre\n");

    SDL_Rect source = {0},
             window_dimensions = {0},
             destination = {0};

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h);
    SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);

    destination = window_dimensions;

    SDL_RenderCopy(renderer, texture, &source, &destination);
}

// affiche un PNJ
void afficher_PNJ(SDL_Window *window, SDL_Renderer *renderer,
                  pnj_t *entite)
{
    if (print_var)
        printf("afficher_pnj\n");
    SDL_Rect source = {0, 0, 0, 0},
             position = entite->pos;

    SDL_QueryTexture(entite->espece->visuel, NULL, NULL, &source.w, &source.h);

    SDL_RenderCopy(renderer, entite->espece->visuel, &source, &position);
}

// affiche un decor
void afficher_decor(SDL_Window *window, SDL_Renderer *renderer,
                    decor_t *decor)
{
    if (print_var)
        printf("afficher_decor\n");
    SDL_Rect source = {0, 0, 0, 0},
             position = decor->pos;

    SDL_QueryTexture(decor->visu, NULL, NULL, &source.w, &source.h);

    SDL_RenderCopy(renderer, decor->visu, &source, &position);
    return;
}

// à partir de là c'est des fonctions de test
void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer)
{
    if (print_var)
        printf("end_sdl\n");
    char msg_formated[255];
    int l;

    if (!ok)
    {
        strncpy(msg_formated, msg, 250);
        l = strlen(msg_formated);
        strcpy(msg_formated + l, " : %s\n");

        SDL_Log(msg_formated, SDL_GetError());
    }

    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();

    if (!ok)
    {
        exit(EXIT_FAILURE);
    }
}

SDL_Texture *load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_Surface *my_image = NULL;   // Variable de passage
    SDL_Texture *my_texture = NULL; // La texture

    my_image = IMG_Load(file_image_name); // Chargement de l'image dans la surface
                                          // image=SDL_LoadBMP(file_image_name); fonction standard de la SDL,
                                          // uniquement possible si l'image est au format bmp */
    if (my_image == NULL)
        end_sdl(0, "Chargement de l'image impossible", window, renderer);

    my_texture = SDL_CreateTextureFromSurface(renderer, my_image); // Chargement de l'image de la surface vers la texture
    SDL_FreeSurface(my_image);                                     // la SDL_Surface ne sert que comme élément transitoire
    if (my_texture == NULL)
        end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);

    return my_texture;
}

void affiche_fond2(SDL_Window *window, SDL_Renderer *renderer,
                  SDL_Texture *texture, SDL_Rect rect)
{
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Rect win;

    SDL_GetWindowSize(window, &win.w, &win.h);
    SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);


    int offset_x = (rect.x % rect.w + rect.w) % rect.w;
    int offset_y = (rect.y % rect.h + rect.h) % rect.h;

    for (int x = -offset_x; x < win.w; x += rect.w)
    {
        for (int y = -offset_y; y < win.h; y += rect.h)
        {
            dst.x = x;
            dst.y = y;
            dst.w = rect.w;
            dst.h = rect.h;

            src.x = 0;
            src.y = 0;

            SDL_RenderCopy(renderer, texture, &src, &dst);
        }
    }
}












/*int mouse=1,
    duree=200;

clock_t timer=0;
    
    
SDL_Rect get_demande(SDL_Event event){
    if (print_var) printf("user_ask\n");
    SDL_Rect rect={0};
    switch(event.type){
        case SDL_QUIT :
            rect.h=4;                 // Il est temps d'arrêter le programme
            break;

        case SDL_KEYDOWN:     

            switch (event.key.keysym.sym) {
                case SDLK_KP_PLUS:
                    rect.w+=10;
                    rect.h=3;
                    break;
                case SDLK_KP_MINUS:
                    rect.w+=-10;
                    rect.h=3;
                    break;
                case SDLK_SPACE: 
                    //printf("on quitte\n");
                    rect.h=4;                 // Il est temps d'arrêter le programme
                    break;
                case SDLK_KP_ENTER:
                    rect.h=5;
                    break;
                case SDLK_KP_0:
                    rect.h=6;
                    break;
            }
            break;
        
        case SDL_MOUSEWHEEL:
            if (event.wheel.y>0){
                rect.w=5;
            }
            else {
                rect.w=-5;
            }
            rect.h=3;
            break;

                    
        case SDL_MOUSEBUTTONDOWN:
            //printf("mouse_down %d\n",mouse);
            //printf("click\n");
            mouse=0;
            timer=clock();
            break;
        case SDL_MOUSEBUTTONUP:
            //printf("mouse_up %d\n",mouse);
            mouse=1;
            if ((clock() - timer)* 1000 / CLOCKS_PER_SEC<duree){
                //printf("change\n");
                rect.x=event.button.x;
                rect.y=event.button.y;
                rect.h=2;
            }
            break;
        case SDL_MOUSEMOTION:
            //printf("mouse_motion %d\n",mouse);
            if (!mouse){
                //printf("move\n");
                rect.x=event.motion.xrel;
                rect.y=event.motion.yrel;
                rect.h=1;
            }
            break;
    }
    return rect;
}


void user_modif(SDL_Rect rect,SDL_Rect *user,int *prog_on,int *temp_on){
    int c_x,c_y;
    switch (rect.h)
            {
            case 1:            //deplace
            printf("deplace\n");
                user->x -= rect.x;
                user->y -= rect.y;
                break;
            case 2:            //selection entity
                printf("click\n");
                
                break;
            case 3:            //zoom
                printf("zoom\n");
                user->w = max(100,user->w + rect.w);
                user->h = max(100,user->h + rect.w);
                break;
            case 4:            //quitter
            printf("quite\n");
                *prog_on = 0;
                break;
            case 5:            
                
                break;
            case 6:            //pause
            printf("pause\n");
                *temp_on=!*temp_on;
                break;
            
            default:
                break;
            }
            //printf("user %d %d %d %d\n",user.x,user.y,user.w,user.h);
}





































int main(){
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
    my_image = IMG_Load("assets/backgrounds/Designer.png");
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
    int temp = 1;

    SDL_Rect user = {15,20,200,200};
    SDL_Rect rect = {0};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    while (running) {
        while (SDL_PollEvent(&event)) {
            rect = get_demande(event);
            user_modif(rect,&user,&running,&temp);
            rect.h=0;
        }

        SDL_RenderClear(renderer);
        affiche_fond2(window, renderer, background, user);
        //afficher_decor(window, renderer, decor, decoration);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }

    SDL_DestroyTexture(decoration);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    if (background)     SDL_DestroyTexture(background);
    free(decor);
    SDL_Quit();
    return 0;

}*/