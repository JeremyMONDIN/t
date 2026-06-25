#ifndef MJ
#define MJ

#include <SDL2/SDL.h>
#include "structure.h"



/**
 * @brief crée les variables globales monde_pnj et monde_decor 
 */
void init_monde(pnj_grid_t **monde_pnj,
                decor_grid_t **monde_decor,
                SDL_Rect taille_monde);

/**
 * @brief libere les variables globales monde_pnj et monde_decor et leur contenue
 */
void free_monde(pnj_grid_t *monde_pnj,
                decor_grid_t *monde_decor);


//**************Mj****************/

/**
 * @brief Crée une nouvelle espèce.
 *
 * Initialise une espèce avec son régime alimentaire,
 * son apparence graphique et ses caractéristiques de base.
 *
 * @param alim Type d'alimentation :
 *             0 = herbivore,
 *             1 = carnivore,
 *             2 = omnivore.
 * @param visu Texture SDL représentant l'espèce.
 * @param stat Caractéristiques initiales de l'espèce.
 *
 * @return Structure espèce initialisée.
 */
espece_t create_espece(char nom[50], int alim, SDL_Texture *visu, caract_t stat, float tab[7][14]);

/**
 * @brief Ajoute un nouvel individu dans le monde.
 *
 * Crée un PNJ appartenant à l'espèce indiquée et
 * l'insère dans la liste globale des agents.
 *
 * @param espece Espèce du nouvel individu.
 * @param pos Position initiale.
 */
void add_pnj(   espece_t *espece,
                SDL_Rect pos,
                pnj_grid_t *monde_pnj,
                pnj_list_t *pnj_l,
                SDL_Rect taille_monde);

/**
 * @brief Ajoute un élément de décor.
 *
 * Utilisé pour créer des ressources, cadavres ou
 * autres éléments interactifs de l'environnement.
 *
 * @param id Identifiant du type de décor.
 * @param visuel Texture SDL associée.
 * @param id_etat État initial du décor.
 * @param rang Priorité ou catégorie du décor.
 * @param pos Position du décor.
 */
void add_decor(int id,
               SDL_Texture *visuel,
               int id_etat,
               int rang,
               SDL_Rect pos,
               decor_grid_t *monde_decor,
               decor_list_t *decor_l,
                SDL_Rect taille_monde);


int check_food(int a, int b);

/**
 * @brief Génère les perceptions d'un agent.
 *
 * Recherche dans le voisinage de l'agent les ressources,
 * alliés, ennemis et proies visibles.
 *
 * @param monde_pnj Grille des agents.
 * @param monde_decor Grille des décors.
 * @param perso Agent observateur.
 *
 * @return Ensemble des perceptions de l'agent.
 */
perception_t donne_perception(
    pnj_grid_t* monde_pnj,
    decor_grid_t* monde_decor,
    pnj_t *perso
);

/**
 * @brief Détermine la prochaine action d'un agent.
 *
 * Utilise les perceptions et le modèle décisionnel
 * pour choisir un état comportemental.
 *
 * @param perso Agent concerné.
 *
 * @return Action sélectionnée.
 */
reponse_t get_action(pnj_t *perso,
                     pnj_grid_t *monde_pnj,
                     decor_grid_t *monde_decor);

/**
 * @brief Exécute le comportement courant d'un agent.
 *
 * Appelle la fonction correspondant à l'état actuel
 * de l'agent (exploration, fuite, chasse, etc.).
 *
 * @param perso Agent à mettre à jour.
 */
void agir(pnj_t *perso,
          pnj_grid_t *monde_pnj,
          pnj_list_t *pnj_l,
          decor_grid_t *monde_decor,
          decor_list_t *decor_l,
          SDL_Rect taille_monde,
          SDL_Texture *texture_death,
          int vitesse_base,
          int vision_base,
          int affichage_graphique);

/**
 * @brief Fait avancer la simulation d'un tour.
 *
 * Met à jour les perceptions, décisions et actions
 * de tous les agents présents dans le monde.
 */
void etape_suivante(pnj_list_t *pnj_l,
                    decor_list_t *decor_l,
                    int faim,
                    pnj_grid_t *monde_pnj,
                    decor_grid_t *monde_decor,
                    SDL_Rect taille_monde,
                    SDL_Texture *texture_death,
                    int affichage_graphique,
                    int print_texte,
                    FILE *log);

/**
 * @brief Affiche le monde.
 *
 * Dessine le décor et les agents visibles.
 *
 * @param window Fenêtre SDL.
 * @param renderer Renderer SDL.
 * @param background Texture de fond.
 * @param rect Zone observée.
 */
void affichage(SDL_Window *window,
               SDL_Renderer *renderer,
               SDL_Texture *background,
               SDL_Rect user,
               pnj_grid_t *monde_pnj,
               decor_grid_t *monde_decor,
               SDL_Rect taille_monde,
                int temp_on);

#endif