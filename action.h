#ifndef ACTION
#define ACTION

#include <SDL2/SDL.h>
#include "structure.h"
#include "liste.h"
#include "Mj.h"


int deplacement_elmentaire(  pnj_t *perso, 
                                    int x, int y, 
                                    int vitesse, int vitesse_base,
                                    SDL_Rect taille_monde);

void attaquer(pnj_t *attaquant, pnj_t *cible, int affichage_graphique);

pnj_t *breed(pnj_t *mere, pnj_t *pere, int affichage_graphique);

//**************ETATS******************************

/**
 * @brief tue la cible fait apparaitre un corp a son emplacement
 * @param cible a tuer 
 */
void mourir(pnj_t *perso, SDL_Texture *texture_death, decor_grid_t *monde_decor, decor_list_t *decor_l,SDL_Rect taille_monde, pnj_grid_t * monde_pnj);

/**
 * @brief fait se déplacer l'unité vers la position
 * @param perso pnj qui se déplace
 * @param pos tableau de taille 2 contenant la position cible x,y
 */
void explorer(  pnj_t *perso, 
                int cor_x, int cor_y,
                int vitesse_base,
                SDL_Rect taille_monde);

/**
 * @brief fait se déplacer un pnj vers une source de nourriture qu'il mangera une fois atteinte
 * @param perso pnj qui effectue l'action
 * @param cible source de nourriture cible
 */
void manger(pnj_t *perso, 
            decor_t *cible,
            int vitesse_base,
            SDL_Rect taille_monde);

/**
 * @brief fait se déplacer un pnj vers un ennemi qu'il attaquera une fois atteinte
 * @param perso pnj qui effectue l'action
 * @param cible pnj qui sera suivi et attaqué
 */
void defendre(  pnj_t *perso, 
                pnj_t *cible,
                int vitesse_base,
                SDL_Rect taille_monde,
                int affichage_graphique);

/**
 * @brief fait se déplacer un pnj vers une proie qu'il attaquera une fois atteinte
 * @param perso pnj qui effectue l'action
 * @param cible pnj qui sera suivi et attaqué
 */
void chasser(   pnj_t *perso, 
                pnj_t *cible,
                int vitesse_base,
                SDL_Rect taille_monde,
                int affichage_graphique);

/**
 * @brief fait se déplacer un pnj vers un allié avec qui il se reproduira une fois atteinte
 * @param perso pnj qui effectue l'action
 * @param cible pnj qui sera suivi et avec qui il se reproduira
 */
void reproduction(  pnj_t *perso, 
                    pnj_t *cible, 
                    pnj_grid_t *monde_pnj, 
                    pnj_list_t *pnj_l,
                    int vitesse_base,
                    SDL_Rect taille_monde,
                    int affichage_graphique);

/**
 * @brief fait se déplacer un pnj a l'opposé un ennemi 
 * @param perso pnj qui effectue l'action
 * @param cible pnj qui sera fuit
 */
void fuir(  pnj_t *perso, 
            pnj_t *cible,
            int vitesse_base,
            SDL_Rect taille_monde);

/**
 * @brief fait se regenerer a un pnj sa vie et son energie en restant sur place
 * @param perso pnj qui effectue l'action
 */
void repos(pnj_t *perso, int vision_base, int affichage_graphique);

#endif