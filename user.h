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
void user_modif(    SDL_Rect rect,
                    SDL_Rect *user,
                    int *prog_on,
                    int *temp_on,
                    pnj_t ** select,
                    pnj_grid_t *monde_pnj,
                    SDL_Rect taille_monde);


#endif