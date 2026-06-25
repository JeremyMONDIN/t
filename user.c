#include <SDL2/SDL.h>
#include <time.h>
#include "structure.h"
#include "user.h"
#include "moteur_rendu.h"
#include "grid.h"
    
int mouse=1,
    duree=200,
    defile = 1;

clock_t timer=0;
    
    
SDL_Rect get_demande(SDL_Event event){

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


void user_modif(    SDL_Rect rect,
                    SDL_Rect *user,
                    int *prog_on,
                    int *temp_on,
                    pnj_t ** select,
                    pnj_grid_t *monde_pnj,
                    SDL_Rect taille_monde){
    *temp_on=defile;
    switch (rect.h)
            {
            case 1:            //deplace
                //printf("deplace\n");
                user->x -= rect.x;
                user->y -= rect.y;
                break;
            case 2:            //selection entity
                //printf("click\n");
                *select = get_pnj(monde_pnj,user->x + rect.x,user->y + rect.y,30,taille_monde);
                break;
            case 3:            //zoom
                //printf("zoom\n");
                user->w = max(100,user->w + rect.w);
                user->h = max(100,user->h + rect.w);
                break;
            case 4:            //quitter
                //printf("quite\n");
                *prog_on = 0;
                break;
            case 5:            
                *temp_on = 1;
                break;
            case 6:            //pause
                //printf("pause\n");
                defile = !defile;
                break;
            
            default:
                break;
            }

            //printf("user %d %d %d %d\n",user.x,user.y,user.w,user.h);
}