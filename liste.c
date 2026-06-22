#include "structure.h"
#include "liste.h"

int taille_liste_base = 100;

pnj_list_t * init_pnj_list(){
    pnj_list_t* l=malloc(sizeof(pnj_list_t));
    l->list = (pnj_t**)malloc(taille_liste_base * sizeof(pnj_t*));
    l->nb_pnj = 0;
    return l;
}

void add_pnj_to_list(pnj_list_t *l, pnj_t *pnj) {
    if (print_var) printf("add_pnj_to_list\n");
    if (l == NULL) {
        printf("warning add_pnj_to_list NULL");
        l=init_pnj_list();
    } else if (l->nb_pnj % taille_liste_base == 0) {
        l->list = (pnj_t**)realloc(l->list, (l->nb_pnj + taille_liste_base) * sizeof(pnj_t*));
    }
    l->list[l->nb_pnj] = pnj;
    l->nb_pnj++;
}

int sup_pnj_from_list(pnj_list_t *list, pnj_t *pnj) {
    if (print_var) printf("sup_pnj_from_list\n");
    int i = 0;
    int n = list->nb_pnj;
    while (i < n) {
        if (list->list[i] == pnj) {
            list->nb_pnj--;
            list->list[i] = list->list[n - 1]; 
            i = n; 
        }
        i++;
    }
    return i-n;
}

void free_list_pnj(pnj_list_t * l){
    if (print_var) printf("free_list_pnj\n");
    if (l!=NULL){
        free(l->list);
        free(l);
    }
}

//******************* Decor *************************************


decor_list_t * init_decor_list(){
    decor_list_t* l=malloc(sizeof(decor_list_t));
    l->list = (decor_t**)malloc(taille_liste_base * sizeof(decor_t*));
    l->nb_decor = 0;
    return l;
}

void add_decor_to_list(decor_list_t *l, decor_t *decor) {
    if (print_var) printf("add_decor_to_list\n");
    if (l == NULL) {
        printf("warning add_decor_to_list NULL");
        l=init_decor_list();
    } else if (l->nb_decor % taille_liste_base == 0) {
        l->list = (decor_t**)realloc(l->list, (l->nb_decor + taille_liste_base) * sizeof(decor_t*));
    }
    l->list[l->nb_decor] = decor;
    l->nb_decor++;

}

void sup_decor_from_list(decor_list_t *list, decor_t *decor) {
    if (print_var) printf("sup_decor_from_list\n");
    int i = 0;
    int n = list->nb_decor;
    while (i < n) {
        if (list->list[i] == decor) {
            list->nb_decor--;
            list->list[i] = list->list[n - 1];
            i = n;
        }
        i++;
    }
}



void free_list_decor(decor_list_t * l){
    if (print_var) printf("free_list_decor\n");
    if (l!=NULL){
        free(l->list);
        free(l);
    }
}