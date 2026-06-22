#ifndef MJ
#define MJ

#include <SDL2/SDL.h>
#include "structure.h"

extern pnj_grid_t *monde_pnj;
extern decor_grid_t *monde_decor;

extern pnj_list_t * pnj_l;
extern decor_list_t * decor_l;

extern SDL_Rect taille_monde ;

extern int duree_action ;
extern float coef_ennemi_proie ;

extern int force_base ;
extern int perte_faim ;
extern int degat_faim ;

extern SDL_Texture* texture_death;

/**
 * @brief crée les variables globales monde_pnj et monde_decor 
 */
void init_monde();

/**
 * @brief libere les variables globales monde_pnj et monde_decor et leur contenue
 */
void free_monde();


//**************Mj****************/

/**
 * @brief Crée une nouvelle espèce.
 *
 * Initialise une structure espèce avec son régime alimentaire,
 * sa texture d'affichage et ses caractéristiques de base.
 *
 * @param alim Type d'alimentation de l'espèce
 *             (herbivore, carnivore ou omnivore).
 * @param visu Texture utilisée pour représenter les individus
 *             de cette espèce.
 * @param stat Caractéristiques initiales de l'espèce.
 *
 * @return Structure espèce initialisée.
 */
espece_t create_espece(char nom[50], int alim, SDL_Texture *visu, caract_t stat);

/**
 * @brief Ajoute un individu dans le monde.
 *
 * Crée un nouveau PNJ appartenant à l'espèce indiquée et le place
 * à la position spécifiée.
 *
 * @param espece Espèce du nouvel individu.
 * @param pos Position et dimensions initiales du PNJ.
 */
void add_pnj(espece_t* espece, SDL_Rect pos);

/**
 * @brief Ajoute un élément de décor dans le monde.
 *
 * Crée un nouvel élément de décor (nourriture, cadavre, obstacle, etc.)
 * et l'insère dans la grille de décor.
 *
 * @param id Identifiant du type de décor.
 * @param visuel Texture utilisée pour l'affichage.
 * @param id_etat État associé au décor.
 * @param rang Priorité ou couche d'affichage du décor.
 * @param pos Position et dimensions du décor.
 */
void add_decor(int id, SDL_Texture * visuel, int id_etat, int rang, SDL_Rect pos);

/**
 * @brief Génère la perception d'un individu.
 *
 * Analyse l'environnement proche du PNJ afin de construire
 * l'ensemble des informations nécessaires à la prise de décision.
 *
 * Les informations peuvent inclure :
 * - nourriture visible ;
 * - alliés visibles ;
 * - ennemis visibles ;
 * - proies visibles ;
 * - distances associées ;
 * - rapports de force et de vitesse.
 *
 * @param monde_pnj Grille contenant les PNJ.
 * @param monde_decor Grille contenant les éléments de décor.
 * @param perso Individu dont la perception doit être calculée.
 *
 * @return Structure contenant les informations perçues.
 */
perception_t donne_perception(
    pnj_grid_t* monde_pnj,
    decor_grid_t* monde_decor,
    pnj_t * perso
);

/**
 * @brief Détermine l'action à effectuer.
 *
 * Utilise la perception de l'individu ainsi que son tableau
 * d'intérêt pour sélectionner une action.
 *
 * Le choix est effectué de manière probabiliste à partir
 * des intérêts calculés pour chaque état comportemental.
 *
 * @param perso Individu devant prendre une décision.
 *
 * @return Réponse contenant l'action choisie et ses paramètres.
 */
reponse_t get_action(pnj_t * perso);

/**
 * @brief Exécute l'action courante d'un individu.
 *
 * Met à jour l'état du PNJ en fonction de l'action sélectionnée :
 * déplacement, alimentation, attaque, reproduction, fuite,
 * exploration ou repos.
 *
 * Cette fonction applique également les coûts énergétiques,
 * les dégâts éventuels et la progression de l'action en cours.
 *
 * @param perso Individu à mettre à jour.
 */
void agir(pnj_t * perso);

/**
 * @brief Fait avancer la simulation d'un tick.
 *
 * Parcourt l'ensemble des individus présents dans le monde,
 * met à jour leurs perceptions, sélectionne leurs actions
 * lorsque nécessaire et exécute leur comportement.
 *
 * Cette fonction constitue la boucle principale du moteur
 * de simulation.
 */
void etape_suivante(pnj_list_t* pnj_l, decor_list_t* decor_l);

/**
 * @brief Affiche l'état courant du monde.
 *
 * Dessine le fond, les éléments de décor et les PNJ
 * présents dans la simulation puis met à jour la fenêtre.
 *
 * @param window Fenêtre SDL utilisée pour l'affichage.
 * @param renderer Renderer SDL utilisé pour le rendu.
 * @param background Texture de fond.
 * @param rect Zone d'affichage du fond.
 */
void affichage(
    SDL_Window *window,
    SDL_Renderer *renderer,
    SDL_Texture *background,
    SDL_Rect rect
);

#endif