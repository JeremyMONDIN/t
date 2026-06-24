#ifndef GRAPH
#define GRAPH

#include <SDL2/SDL.h>
#include "Mj.h"
#include "structure.h"

#define MAX(a,b) (a>=b?a:b)
#define MIN(a,b) (a<=b?a:b)

/**
* @brief Permet l'affichage de la fenêtre, et fait apparaître un arrière-plan
* @param window le fenêtre dans laquelle on va dessiner
* @param renderer le rendu de la fenêtre sur laquelle on dessine
* @param texture l'arrière-plan à mettre sur la fenêtre 
*/
void dessiner_fenetre(SDL_Window * window, SDL_Renderer * renderer, 
                      SDL_Texture * texture);

/** 
* @brief Affiche un PNJ en fonction de sa position actuelle
* @param window la fenêtre dans laquelle on dessine
* @param renderer le rendu de la fenêtre sur laquelle on dessine
* @param entite le PNJ à montrer sur la fenêtre 
*/
void afficher_PNJ(SDL_Window * window, SDL_Renderer * renderer,
                  pnj_t * entite);

/** 
* @brief Affiche un décor en fonction de sa position actuelle
* @param window la fenêtre sur laquelle on dessine 
* @param renderer le rendu de la fenêtre sur laquelle on dessine
* @param decor le décor à montrer sur la fenêtre
*/
void afficher_decor(SDL_Window * window, SDL_Renderer * renderer,
                  decor_t * decor);

void affiche_fond2(SDL_Window *window, SDL_Renderer *renderer,
                  SDL_Texture *texture, SDL_Rect rect);

#endif