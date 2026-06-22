#define NDEBUG //compile avec le flag -DNDEBUG=1 pour que les asserts coupent le prog

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <assert.h>

#include "structure.h"
#include "agent.h"
#include "action.h"
#include "grid.h"
#include "liste.h"
#include "moteur_rendu.h"
#include "Mj.h"

pnj_grid_t * monde_pnj;    
decor_grid_t * monde_decor;

SDL_Rect taille_monde={1000,1000,50,50};

int tests_listes_pnj(void){
    //Banque de tests pour les listes de pnjs
    pnj_list_t * liste_pnj = init_pnj_list();

    printf("Tests initialisation de liste");
    assert(liste_pnj != NULL);
    assert(liste_pnj->nb_pnj == 0);
    assert(liste_pnj->list != NULL);

    //Initialisation de l'espèce ajoutée dans la liste
    SDL_Rect pos = {100,100,0,0};
    caract_t caractere = {10,10,10,10};
    espece_t espece1 = create_espece(1, NULL, caractere);
    pnj_t * new = create_pnj(espece1, 100, espece1.caract, pos,100);

    add_pnj_to_list(liste_pnj, new);
    printf("Ajout d'un élément dans la liste");
    assert(liste_pnj->nb_pnj == 1);
    assert(liste_pnj->list[liste_pnj->nb_pnj] == new);

    //Supression de l'espèce ajoutée
    sup_pnj_from_list(liste_pnj, new);
    printf("Supression de l'élément ajouté");
    assert(liste_pnj->nb_pnj == 0);
    assert(liste_pnj->list[liste_pnj->nb_pnj] != new); //pas sur de celui la

    //Libération de l'espace mémoire
    free_list_pnj(liste_pnj);
    printf("Libération de l'espace mémoire de la liste");
    assert(liste_pnj == NULL);

    return EXIT_SUCCESS;
}

int tests_listes_decors(void){
    //Banque de tests pour les listes de decors
    decor_list_t * liste_decor = init_decor_list();

    printf("Tests initialisation de liste");
    assert(liste_decor != NULL);
    assert(liste_decor->nb_decor == 0);
    assert(liste_decor->list != NULL);

    //Initialisation du décor ajouté dans la liste
    SDL_Rect pos = {100,100,0,0};
    decor_t * new = create_decor(1, NULL, 1, 1, pos);

    add_decor_to_list(liste_decor, new);
    printf("Ajout d'un élément dans la liste");
    assert(liste_decor->nb_decor == 1);
    assert(liste_decor->list[liste_decor->nb_decor] == new);

    //Supression du décor ajouté
    sup_decor_from_list(liste_decor, new);
    printf("Supression de l'élément ajouté");
    assert(liste_decor->nb_decor == 0);
    assert(liste_decor->list[liste_decor->nb_decor] != new); //pas sur de celui la

    //Libération de l'espace mémoire
    free_list_decor(liste_decor);
    printf("Libération de l'espace mémoire de la liste");
    assert(liste_decor == NULL);

    return EXIT_SUCCESS;
}

int tests_grids_pnj(void){
    //Banque de tests sur les grilles pour les pnjs
    SDL_Rect pos = {100,100,0,0};
    caract_t caractere = {10,10,10,10};
    espece_t espece1 = create_espece(1, NULL, caractere);
    pnj_t * new = create_pnj(espece1, 100, espece1.caract, pos,100);

    pnj_grid_t * grille = creat_grid_pnj(10,10,1,1);
    printf("Test de la fonction de création");
    assert(grille != NULL);

    //Ajout d'un pnj dans la grille
    place_pnj_in_grid(grille,new);
    assert(grille->grid->nb_pnj == 1);
    assert(grille->grid->list[grill->grid->nb_pnj] == new); //pas super sur

    //Supression du pnj dans la grille
    sup_pnj_from_grid(grille,new);
    assert(grille->grid->nb_pnj == 0);
    assert(grille->grid->list[grill->grid->nb_pnj] != new); //idem

    //Libération de l'espace mémoire
    free_pnj_grid(grille);
    printf("Test de libération de l'espace mémoire");
    assert(grille == NULL);

    return EXIT_SUCCESS;

}

int tests_grids_decors(void){
    //Banque de tests sur les grilles pour les decors
    SDL_Rect pos = {100,100,0,0};
    decor_t * new = create_decor(1, NULL, 1, 1, pos);

    pnj_grid_t * grille = creat_grid_decor(10,10,1,1);
    printf("Test de la fonction de création");
    assert(grille != NULL);

    //Ajout d'un decor dans la grille
    place_decor_in_grid(grille,new);
    assert(grille->grid->nb_decor == 1);
    assert(grille->grid->list[grill->grid->nb_decor] == new); //pas super sur

    //Supression du decor dans la grille
    sup_decor_from_grid(grille,new);
    assert(grille->grid->nb_decor == 0);
    assert(grille->grid->list[grill->grid->nb_decor] != new); //idem
    
    //Libération de l'espace mémoire
    free_pnj_grid(grille);
    printf("Test de libération de l'espace mémoire");
    assert(grille == NULL);

    return EXIT_SUCCESS;
}

int tests_agents(void){
    int T={10,20,5,6};
    int T1[4]={1,0,0,0};
    int T2[4]={0,1,0,0};
    int T3[4]={0,0,1,0};
    int T4[4]={0,0,0,1};

    int *pos=malloc(2*sizeof(2));
    pos[0]=0;
    pos[1]=0;

    pnj_t * inter_indi=NULL;
    decor_t * inter_dec=NULL;

    change_proba(T);

    assert(action(T1, pos, inter_indi, inter_dec)->act == 0); // 0 (0,0)
    assert(action(T1, pos, inter_indi, inter_dec)->depla[0] == 0);
    assert(action(T1, pos, inter_indi, inter_dec)->depla[1] == 0);
    assert(action(T2, pos, inter_indi, inter_dec)->act == 1); // 1 NULL
    assert(action(T2, pos, inter_indi, inter_dec)->depla == NULL);
    assert(action(T3, pos, inter_indi, inter_dec)->act == 2); // 2 NULL
    assert(action(T3, pos, inter_indi, inter_dec)->depla == NULL);
    assert(action(T4, pos, inter_indi, inter_dec)->act == 3); // 3 NULL
    assert(action(T4, pos, inter_indi, inter_dec)->depla == NULL);

    
}