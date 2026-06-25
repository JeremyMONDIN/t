#ifndef LISTE
#define LISTE

#include "structure.h"



traj_t init_traj();

/**
 * @brief initialise une liste de pnj
 * @return renvoie un pnj_list* vide

 */
pnj_list_t * init_pnj_list();

/**
 * @brief ajoute un pnj à une liste de pnj (s'occupe de la mémoire,gere NULL)
 * @param list une liste de pnj
 * @param pnj le pnj à ajouter dans la liste
 */
void add_pnj_to_list(pnj_list_t *list, pnj_t *pnj);

/**
 * @brief supprime un pnj d'une liste
 * @param list la liste des pnj
 * @param pnj le pnj à supprimer de la liste
 */
int sup_pnj_from_list(pnj_list_t *list, pnj_t *pnj);


/**
 * @brief initialise une liste de decor
 * @return renvoie une decor_list_t* vide
 */
decor_list_t * init_decor_list();

/**
 * @brief ajoute un decor à une liste de décor (s'occupe de la mémoire,gere NULL)
 * @param list une liste de pnj
 * @param pnj le pnj à ajouter dans la liste
 */
void add_decor_to_list(decor_list_t *list, decor_t *decor);

/**
 * @brief supprime un decor d'une liste
 * @param list la liste des décors
 * @param decor le décor à supprimer de la liste
 */
void sup_decor_from_list(decor_list_t *list, decor_t *decor);

void free_list_pnj(pnj_list_t * l);
void free_list_decor(decor_list_t * l);

#endif