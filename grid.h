#ifndef GRID
#define GRID

#include "structure.h"
#include "Mj.h"



/**
 * 
 * @brief Crée une grille PNJ vide
 * @param monde_x le nombre de cases sur x
 * @param monde_y le nombre de cases sur y
 * @param taille_w la largeur d'une case
 * @param taille_h la hauteur d'une case
 * @return une grille initialisée vide de PNJs
 */

pnj_grid_t * creat_grid_pnj(int nb_case_x, int nb_case_y, int taille_w, int taille_h);

/**
 * @brief Rend la memoire de la grille y compris celle des entitées stockées
 * @param grid une grille de PNJs
 */
void free_pnj_grid(pnj_grid_t *grid);

/**
 * @brief Créer une grille de décors vide
 * @param monde_x le nombre de cases sur x
 * @param monde_y le nombre de cases sur y
 * @param taille_w la largeur d'une case
 * @param taille_h la hauteur d'une case
 * @return une grille initialisée vide de décors
 */
decor_grid_t * creat_grid_decor(int nb_case_x, int nb_case_y, int taille_w, int taille_h);

/**
 * @brief rend la memoire de la grille y compris celle des entitées stockées
 * @param grid une grille de décors
 */
void free_decor_grid(decor_grid_t *grid);

/**
 * @brief Crée un PNJ
 * @param espece une espèce choisie
 * @param energie une énergie donnée
 * @param stat les caractéristiques de l'espèce choisie
 * @param pos la position de base du PNJ
 * @param vie les points de vie du PNJ
 * @param faim la faim du PNJ
 * @return un PNJ ainsi que ses caractéristiques principales

 */
pnj_t* create_pnj( espece_t* espece, int energie,caract_t stat, SDL_Rect pos, int vie, int faim);

/**
 * @brief LIbère et supprime un PNJ du monde
 * @param monde_pnj le monde dans lequel se trouve le PNJ
 * @param perso le PNJ à supprimer
 */
void kill_pnj(pnj_grid_t* monde_pnj, pnj_t * perso);

/**
 * @brief Place un PNJ à sa place dans la grille
 * @param grid La grille dans laquelle on place le PNJ
 * @param pnj le pnj à poser dans la grille
 */
void place_pnj_in_grid(pnj_grid_t* grid, pnj_t *pnj);

/**
 * @brief Supprime un PNJ de la grille (mais il ne free pas !)
 * @param grid La grille dans laquelle se trouve le PNJ à supprimer
 * @param pnj le pnj à supprimer dans la grille
 */
int sup_pnj_from_grid(pnj_grid_t* grid, pnj_t *pnj);

/**
 * @brief Donne la liste des PNJ dans une zone donnée
 * @param grid La grille contenant les PNJ
 * @param area La zone dans la laquelle on recherche 
 * @return Une liste des pnj présents dans la zone
 */
pnj_list_t * get_pnj_list_in(pnj_grid_t* grid, SDL_Rect area);

/**
 * @brief Donne la liste des PNJ dans un rayon donné
 * @param grid la grille contenant les PNJ
 * @param x la coordonnée horizontale du centre de recherche
 * @param y la coordonnée verticale du centre de recherche
 * @param radius le rayon de recherche
 * @return Une liste des pnjs présents dans le rayon donné
 */
pnj_list_t * get_pnj_list_in_radius(pnj_grid_t* grid, int x, int y, int radius);


/**
 * @brief Crée un décor
 * @param id Le type de décor utilisé
 * @param visuel La texture du décor
 * @param id_etat L'état du décor donné 
 * @param rang L'étape de l'état (?)
 * @param pos La position du décor sur la grille
 * @return Un décor, ainsi que son état et son rang
 */
decor_t* create_decor(int id, SDL_Texture * visuel, int id_etat,int rang, SDL_Rect pos);
/**
 * @brief Libère et supprime du monde un décor
 * @param monde_decor Le monde contenant les décors
 * @param cible Le decor que l'on supprime
 */
void kill_decor(decor_grid_t* monde_decor, decor_t ** cible);

/**
 * @brief Place un décor dans grid
 * @param grid la grille dans laquelle on place le décor
 * @param decor le décor que l'on souhaite poser
 */
void place_decor_in_grid(decor_grid_t* grid, decor_t *decor);

/**
 * @brief Supprime un décor de grid (mais ne libère pas !)
 * @param grid La grille dans laquelle se trouve le decor
 * @param decor le décor à retirer
 */
void sup_decor_from_grid(decor_grid_t* grid, decor_t *decor);

/**
 * @brief Donne la liste des décors dans la zone
 * @param grid la grille dans laquelle on recherche
 * @param area la zone dans laquelle on recherche les decors
 * @return la liste des decors dans la zone recherchée
 */
decor_list_t * get_decor_list_in(decor_grid_t* grid, SDL_Rect area);
/**
 * @brief Donne la liste des décors dans un rayon donné
 * @param grid la grille dans laquelle on recherche
 * @param x la position horizontale du centre du cercle
 * @param y la position verticale du centre du cercle
 * @param radius le rayon dans lequel on recherche
 * @return une liste des décors dans le rayon recherché
 */
decor_list_t * get_decor_list_in_radius(decor_grid_t* grid, int x, int y, int radius);

#endif 