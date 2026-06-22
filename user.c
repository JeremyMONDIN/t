#include <SDL2/SDL.h>
#include <time.h>
#include "structure.h"
#include "user.h"
    
int mouse=1,
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


void user_modif(SDL_Rect *rect,SDL_Rect *user,SDL_Event event,int *prog_on,int *temp_on){
    int c_x,c_y;
    switch (user->h)
            {
            case 1:            //deplace
                rect->x+=-user->x;
                rect->y+=-user->y;
                break;
            case 2:            //selection entity
                
                break;
            case 3:            //zoom
                if (10>(rect->w+user->w)) 
                    rect->w  = 10;
                else 
                    rect->w = rect->w+user->w;
                break;
            case 4:            //quitter
                *prog_on = 0;
                break;
            case 5:            
                
                break;
            case 6:            //pause
                *temp_on=!*temp_on;
                break;
            
            default:
                break;
            }
            //printf("user %d %d %d %d\n",user.x,user.y,user.w,user.h);
            user->h=0;
}