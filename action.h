#ifndef ACTION
#define ACTION

#include <SDL2/SDL.h>
#include "structure.h"
#include "liste.h"
#include "Mj.h"

extern pnj_list_t * pnj_l;
extern decor_list_t * decor_l;

extern pnj_grid_t* monde_pnj;
extern decor_grid_t* monde_decor;
extern SDL_Texture* texture_death;

//mort
extern float coef_nouriture_par_corp ;

//deplacement
extern int deplacemant_par_tic ,
    cout_deplacement ;

//manger
extern int gain_faim_plante , //doublé pour la viande
    gain_energie ;

//attaque
extern int degats_attaque ,
    coef_bonus_force ,
    cout_attaque ;

//reproduction
extern int cout_reproduction ;

//repos
extern int regen_energie ,
    regen_vie;


//stat de base
extern int force_base ,
    vitesse_base ,
    vision_base ,
    distance_interaction ;



//**************ETATS******************************

/**
 * @brief tue la cible fait apparaitre un corp a son emplacement
 * @param cible a tuer 
 */
void mourir(pnj_t *perso);

/**
 * @brief fais ce déplacer l'unité vers la position
 * @param perso pnj qui se déplace
 * @param pos tableau de taille 2 contenant la position cible x,y
 */
void explorer(pnj_t *perso, int cor_x, int cor_y);

/**
 * @brief fais ce déplacer un pnj vers une source de nouriture qu'il mangera une fois atteinte
 * @param perso pnj qui effectue l'action
 * @param cible source de nourriture cible
 */
void manger(pnj_t *perso, decor_t *cible);

/**
 * @brief fais ce déplacer un pnj vers un ennemi qu'il attaquera une fois atteinte
 * @param perso pnj qui effectue l'action
 * @param cible pnj qui sera suivi et attaqué
 */
void defendre(pnj_t *perso, pnj_t *cible);

/**
 * @brief fais ce déplacer un pnj vers une proie qu'il attaquera une fois atteinte
 * @param perso pnj qui effectue l'action
 * @param cible pnj qui sera suivi et attaqué
 */
void chasser(pnj_t *perso, pnj_t *cible);

/**
 * @brief fais ce déplacer un pnj vers un allié avec qui il se reproduira une fois atteinte
 * @param perso pnj qui effectue l'action
 * @param cible pnj qui sera suivi et avec qui il se reproduira
 */
void reproduction(pnj_t *perso, pnj_t *cible);

/**
 * @brief fais ce déplacer un pnj a l'opposé un ennemi 
 * @param perso pnj qui effectue l'action
 * @param cible pnj qui sera fuit
 */
void fuir(pnj_t *perso, pnj_t *cible);

/**
 * @brief fais ce regenerer a un pnj sa vie et son energie en restant sur place
 * @param perso pnj qui effectue l'action
 */
void repos(pnj_t *perso);

#endif