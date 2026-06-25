#ifndef AGENT
#define AGENT

#include <SDL2/SDL.h>
#include "structure.h"
#include "liste.h"




float dist(SDL_Rect a, SDL_Rect b);

/**
* @brief creer la reponse correspondant a l'action souhaité
* @param s l'identification de l'action
* @param pos la position si il faut faire un déplacement
* @param nouriture nourriture la plus proche si on doit manger
* @param allie allie le plus proche pour la reproduction
* @param ennemi ennemi le plus proche a fuir ou attaquer
* @param proie proie la plus proche pour la chasse
* @return L'action que l'agent souhaite réaliser
*/
reponse_t action(int s, int x, int y, decor_t* nourriture, pnj_t* allie, pnj_t* ennemi, pnj_t* proie);

float dist(SDL_Rect a, SDL_Rect b);

float somme_tab(
    float tab[7][14],
    int k,
    int vie,
    int energie,
    int faim,
    int nb_nourriture,
    int nb_allie,
    int nb_ennemi,
    int nb_proie,
    float d_nourriture,
    float d_allie,
    float d_ennemi,
    float d_proie,
    float rapport_force,
    float rapport_vitesse);

/**
 * @brief calcule l'action que l'indivue souhaite faire
 * @param per la perception de l'individu
 * @param tab tableau des coeficient action/perception
 * @return  L'action que l'agent souhaite réaliser
 */
reponse_t reaction_entite(perception_t per,float tab[7][14]);

/**
 * @brief quelle fonction lancer selon l'alimentation de l'individus
 * @param p la perception de l'individus
 * @return L'action que l'agent souhaite réaliser
 */
reponse_t reaction(perception_t p);


#endif