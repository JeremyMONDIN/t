#ifndef USER
#define USER

#include <SDL2/SDL.h>
#include "structure.h"
#include "liste.h"

/**
 * renvoie les demandes utilisateur
 */
SDL_Rect get_demande(SDL_Event event);

/**
 * 
 */
void user_modif(SDL_Rect *rect,SDL_Rect *user,SDL_Event event,int *prog_on,int *temp_on);


#endif