#include <stdlib.h>
#include <SDL2/SDL.h>
#include "structure.h"
#include "liste.h"
#include "Mj.h"
#include "grid.h"


pnj_t* create_pnj(espece_t* espece, int energie,caract_t stat, SDL_Rect pos, int vie, int faim) {
    if (print_var) printf("create_pnj\n");
    pnj_t* new_pnj = malloc(sizeof(pnj_t));
    etat_t base={7,-1,-1,0,NULL};
    new_pnj->espece = espece;
    new_pnj->energie = energie;
    new_pnj->caract=stat;
    new_pnj->pos = pos;
    new_pnj->vie = vie;
    new_pnj->faim = faim;
    new_pnj->etat=base;
    new_pnj->lock_repro=10;
    return new_pnj;
}



void place_pnj_in_grid(pnj_grid_t* grid, pnj_t *pnj) {
    if (print_var) printf("place_pnj_in_grid\n");
    int case_x = pnj->pos.x / grid->taille_w;
    int case_y = pnj->pos.y / grid->taille_h;
    int n=0;
    if(grid->grid[case_x][case_y]->list == NULL) {
        n=0;
    }
    else {
        n=grid->grid[case_x][case_y]->nb_pnj;
    }

    if (case_x >= 0 && case_x < grid->nb_case_x && case_y >= 0 && case_y < grid->nb_case_y) {
        add_pnj_to_list(grid->grid[case_x][case_y], pnj);
    } else {
        fprintf(stderr, "PNJ position out of grid bounds\n");
    }
}

int sup_pnj_from_grid(pnj_grid_t* grid, pnj_t *pnj) {
    if (print_var) printf("sup_pnj_from_grid\n");
    int case_x = pnj->pos.x / grid->taille_w;
    int case_y = pnj->pos.y / grid->taille_h;

    if (case_x >= 0 && case_x < grid->nb_case_x && case_y >= 0 && case_y < grid->nb_case_y) {
        return sup_pnj_from_list(grid->grid[case_x][case_y], pnj);
    } else {
        fprintf(stderr, "PNJ position out of grid bounds\n");
        return 1;
    }
}

pnj_list_t * get_pnj_list_in(pnj_grid_t* grid, SDL_Rect area) {
    if (print_var) printf("get_pnj_list_in\n");
    int x = area.x / grid->taille_w;
    int y = area.y / grid->taille_h;
    int n_x = area.w / grid->taille_w;
    int n_y = area.h / grid->taille_h;
    pnj_list_t *result = init_pnj_list();

    for (int i = x; i < x + n_x && i < grid->nb_case_x; i++) {
        for (int j = y; j < y + n_y && j < grid->nb_case_y; j++) {
            if(grid->grid[i][j]->list ) {
                for (int k = 0; k < grid->grid[i][j]->nb_pnj; k++) {
                    pnj_t *pnj = grid->grid[i][j]->list[k];
                    if (area.x<pnj->pos.x && pnj->pos.x<area.x+area.w && area.y<pnj->pos.y && pnj->pos.y<area.y+area.h) {
                        add_pnj_to_list(result, pnj);
                    }
                }
            }
        }
    }
    return result;
}

pnj_list_t * get_pnj_list_in_radius(pnj_grid_t* grid, int x, int y, int radius) {
    if (print_var) printf("get_pnj_list_radius\n");
    int case_x = wrap((x - radius) , grid->taille_w*grid->nb_case_x)/grid->nb_case_x;
    int case_y = wrap((y - radius) , grid->taille_h*grid->nb_case_y)/grid->nb_case_y;
    int n_x = (radius * 2) / grid->taille_w;
    int n_y = (radius * 2) / grid->taille_h;
    pnj_list_t *result = init_pnj_list();
    pnj_list_t *check=NULL;

    for (int i = case_x; i < case_x + n_x; i++) {
        for (int j = case_y; j < case_y + n_y ; j++) {
            check=grid->grid[i% grid->nb_case_x][j%grid->nb_case_y];
            if(check->list ) {
                for (int k = 0; k < check->nb_pnj; k++) {
                    pnj_t *pnj = check->list[k];
                    if ((x-pnj->pos.x)*(x-pnj->pos.x) + (y-pnj->pos.y)*(y-pnj->pos.y) <= radius*radius) {
                        add_pnj_to_list(result, pnj);
                    }
                }
            }
        }
    }
    return result;
}

void kill_pnj(pnj_grid_t* monde_pnj, pnj_t * perso){
    if (print_var) printf("kill_pnj\n");
    sup_pnj_from_grid(monde_pnj, perso);
    free(perso);
}


//***************************DECOR***************************/


decor_t* create_decor(int id, SDL_Texture* visuel, int id_etat,int rang, SDL_Rect pos) {
    if (print_var) printf("create_decor\n");
    decor_t* new_decor = (decor_t*)malloc(sizeof(decor_t));
    
    new_decor->type_decor = id;
    new_decor->visu = visuel;
    new_decor->etat.id = id_etat;
    new_decor->etat.para = NULL;
    new_decor->etat.step =rang;
    new_decor->pos = pos;
    return new_decor;
}



void place_decor_in_grid(decor_grid_t* grid, decor_t *decor) {
    if (print_var) printf("place_decor_in_grid\n");
    int case_x = decor->pos.x / grid->taille_w;
    int case_y = decor->pos.y / grid->taille_h;
    int n=0;
    if(grid->grid[case_x][case_y]->list == NULL) {
        n=0;
    }
    else {
        n=grid->grid[case_x][case_y]->nb_decor;
    }

    if (case_x >= 0 && case_x < grid->nb_case_x && case_y >= 0 && case_y < grid->nb_case_y) {
        add_decor_to_list(grid->grid[case_x][case_y], decor);
    } else {
        fprintf(stderr, "Decor position out of grid bounds\n");
    }
}

void sup_decor_from_grid(decor_grid_t* grid, decor_t *decor) {
    if (print_var) printf("sup_decor_from_grid\n");
    int case_x = decor->pos.x / grid->taille_w;
    int case_y = decor->pos.y / grid->taille_h;

    if (case_x >= 0 && case_x < grid->nb_case_x && case_y >= 0 && case_y < grid->nb_case_y) {
        sup_decor_from_list(grid->grid[case_x][case_y], decor);
    } else {
        fprintf(stderr, "Decor position out of grid bounds\n");
    }
}

decor_list_t * get_decor_list_in(decor_grid_t* grid, SDL_Rect area) {
    if (print_var) printf("get_decor_list_in\n");
    int x = area.x / grid->taille_w;
    int y = area.y / grid->taille_h;
    int n_x = area.w / grid->taille_w;
    int n_y = area.h / grid->taille_h;
    decor_list_t *result = init_decor_list();

    for (int i = x; i < x + n_x && i < grid->nb_case_x; i++) {
        for (int j = y; j < y + n_y && j < grid->nb_case_y; j++) {
            if(grid->grid[i][j]->list ) {
                for (int k = 0; k < grid->grid[i][j]->nb_decor; k++) {
                    decor_t *decor = grid->grid[i][j]->list[k];
                    if (area.x<decor->pos.x && decor->pos.x<area.x+area.w && area.y<decor->pos.y && decor->pos.y<area.y+area.h) {
                        add_decor_to_list(result, decor);
                    }
                }
            }
        }
    }
    return result;
}

decor_list_t * get_decor_list_in_radius(decor_grid_t* grid, int x, int y, int radius) {
    if (print_var) printf("get_decor_list_radius\n");
    int case_x = wrap((x - radius) , grid->taille_w*grid->nb_case_x)/grid->nb_case_x;
    int case_y = wrap((y - radius) , grid->taille_h*grid->nb_case_y)/grid->nb_case_y;
    int n_x = (radius * 2) / grid->taille_w;
    int n_y = (radius * 2) / grid->taille_h;
    decor_list_t *result = init_decor_list();
    decor_list_t *check=NULL;

    for (int i = case_x; i < case_x + n_x; i++) {
        for (int j = case_y; j < case_y + n_y ; j++) {
            check=grid->grid[i% grid->nb_case_x][j%grid->nb_case_y];
            if(check->list ) {
                for (int k = 0; k < check->nb_decor; k++) {
                    decor_t *decor = check->list[k];
                    if ((x-decor->pos.x)*(x-decor->pos.x) + (y-decor->pos.y)*(y-decor->pos.y) <= radius*radius) {
                        add_decor_to_list(result, decor);
                    }
                }
            }
        }
    }
    return result;
}

void kill_decor(decor_grid_t* monde_decor, decor_t ** cible){
    if (print_var) printf("kill_decor\n");
    sup_decor_from_grid(monde_decor, *cible);
}

//***************************MONDE**************************

pnj_grid_t* creat_grid_pnj(int monde_x, int monde_y, int taille_w, int taille_h) {
    if (print_var) printf("create_grid_pnj\n");
    pnj_grid_t* grille =malloc(sizeof(pnj_grid_t));
    
    grille->nb_case_x = monde_x/taille_w;
    grille->nb_case_y = monde_y/taille_h;
    grille->taille_w = taille_w;
    grille->taille_h = taille_h;
    
    grille->grid = (pnj_list_t***)malloc(grille->nb_case_x * sizeof(pnj_list_t**));
    
    for (int i = 0; i < grille->nb_case_x; i++) {
        grille->grid[i] = (pnj_list_t**)malloc(grille->nb_case_y * sizeof(pnj_list_t*));
        for(int j=0; j<grille->nb_case_y;j++){
            grille->grid[i][j] = init_pnj_list();
        }
    }

    return grille;
}

void free_pnj_grid(pnj_grid_t* grid) {
    if (grid == NULL) return;
    if (print_var) printf("free_pnj_grid\n");
    for (int i = 0; i < grid->nb_case_x; i++) {
        for (int j =0;j < grid->nb_case_y; j++){
            free_list_pnj(grid->grid[i][j]);
        }
        free(grid->grid[i]);
    }
    free(grid->grid);
    free(grid);
}

decor_grid_t* creat_grid_decor(int monde_x, int monde_y, int taille_w, int taille_h) {
    if (print_var) printf("creat_grid_decor\n");
    decor_grid_t* grid= malloc(sizeof(decor_grid_t));
    
    grid->nb_case_x = monde_x/taille_w;
    grid->nb_case_y = monde_y/taille_h;
    grid->taille_w = taille_w;
    grid->taille_h = taille_h;
    
    grid->grid = (decor_list_t***)malloc(grid->nb_case_x * sizeof(decor_list_t**));
    
    for (int i = 0; i < grid->nb_case_x; i++) {
        grid->grid[i] = (decor_list_t**)malloc(grid->nb_case_y * sizeof(decor_list_t*));
        for(int j=0; j<grid->nb_case_y;j++){
            grid->grid[i][j] = init_decor_list();
        }
    }

    return grid;
}

void free_decor_grid(decor_grid_t* grid) {
    if (grid == NULL) return;
    if (print_var) printf("free_decor_grid\n");
    for (int i = 0; i < grid->nb_case_x; i++) {
        for (int j =0;j < grid->nb_case_y; j++){
            free_list_decor(grid->grid[i][j]);
        }
        free(grid->grid[i]);
    }
    free(grid->grid);
    free(grid);
}