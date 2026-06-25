#include <stdlib.h>
#include <SDL2/SDL.h>
#include "structure.h"
#include "liste.h"
#include "Mj.h"
#include "grid.h"


pnj_t* create_pnj(espece_t* espece, int energie,caract_t stat, SDL_Rect pos, int vie, int faim) {

    pnj_t* new_pnj = malloc(sizeof(pnj_t));
    etat_t base={7,-1,-1,0,NULL};
    new_pnj->espece = espece;
    new_pnj->energie = energie;
    new_pnj->caract=stat;
    new_pnj->pos = pos;
    new_pnj->vie = vie;
    new_pnj->faim = faim;
    new_pnj->etat=base;
    new_pnj->lock_repro=1;
    new_pnj->lock_anim=0;
    new_pnj->trajectoire=init_traj();
    return new_pnj;
}



void place_pnj_in_grid(pnj_grid_t* grid, pnj_t *pnj, SDL_Rect taille_monde) {

    int case_x = wrap(pnj->pos.x, taille_monde.x) / grid->taille_w;
    int case_y = wrap(pnj->pos.y, taille_monde.y) / grid->taille_h;

    case_x = wrap(case_x, grid->nb_case_x);
    case_y = wrap(case_y, grid->nb_case_y);

    if (case_x >= 0 && case_x < grid->nb_case_x && case_y >= 0 && case_y < grid->nb_case_y) {
        add_pnj_to_list(grid->grid[case_x][case_y], pnj);
    } else {
        fprintf(stderr, "PNJ position out of grid bounds\n");
    }
}

int sup_pnj_from_grid(pnj_grid_t* grid, pnj_t *pnj, SDL_Rect taille_monde) {

    int case_x = wrap(pnj->pos.x, taille_monde.x) / grid->taille_w;
    int case_y = wrap(pnj->pos.y, taille_monde.y) / grid->taille_h;

    case_x = wrap(case_x, grid->nb_case_x);
    case_y = wrap(case_y, grid->nb_case_y);


    if (case_x >= 0 && case_x < grid->nb_case_x && case_y >= 0 && case_y < grid->nb_case_y) {
        return sup_pnj_from_list(grid->grid[case_x][case_y], pnj);
    } else {
        fprintf(stderr, "PNJ position out of grid bounds\n");
        return 1;
    }
}

pnj_t * get_pnj(pnj_grid_t* grid, int x, int y,int w, SDL_Rect taille_monde) {

    pnj_t* select=NULL;

    int m_x = wrap(x, taille_monde.x);
    int m_y = wrap(y, taille_monde.y);
    //printf("pos click %d , %d ,%d ,%d", x-w, x, y-w, y);
    int case_x = wrap(x, taille_monde.w) / grid->taille_w;
    int case_y = wrap(y, taille_monde.w) / grid->taille_h;

    case_x = wrap(case_x, grid->nb_case_x);
    case_y = wrap(case_y, grid->nb_case_y);

    if(grid->grid[case_x][case_y]->list == NULL) {
        return NULL;
    }
    else {
        int i=0;
        while (select == NULL && i < grid->grid[case_x][case_y]->nb_pnj){
            pnj_t* current =grid->grid[case_x][case_y]->list[i];
            if (m_x-w < current->pos.x && current->pos.x < m_x &&
                m_y-w < current->pos.y && current->pos.y < m_y)
                    select = current;
            i++;
        }
        return select;
    }
}

pnj_list_t * get_pnj_list_in(pnj_grid_t* grid, SDL_Rect area, SDL_Rect taille_monde) {

    pnj_list_t *result = init_pnj_list();

    int W = taille_monde.w;
    int H = taille_monde.h;

    int x2 = area.x + area.w;
    int y2 = area.y + area.h;

    // Nombre max de splits (jusqu'à 4 rectangles)
    int nb_rect = 1;
    if (x2 > W && y2 > H) nb_rect = 4;
    else if (x2 > W || y2 > H) nb_rect = 2;

    for (int r = 0; r < nb_rect; r++) {

        SDL_Rect sub;

        // Cas sans wrap
        if (nb_rect == 1) {
            sub = area;
        }
        // Split horizontal
        else if (nb_rect == 2 && x2 > W) {
            if (r == 0) sub = (SDL_Rect){area.x, area.y, W - area.x, area.h};
            else        sub = (SDL_Rect){0, area.y, x2 - W, area.h};
        }
        // Split vertical
        else if (nb_rect == 2) {
            if (r == 0) sub = (SDL_Rect){area.x, area.y, area.w, H - area.y};
            else        sub = (SDL_Rect){area.x, 0, area.w, y2 - H};
        }
        // Split en 4
        else {
            if (r == 0) sub = (SDL_Rect){area.x, area.y, W - area.x, H - area.y};
            if (r == 1) sub = (SDL_Rect){0, area.y, x2 - W, H - area.y};
            if (r == 2) sub = (SDL_Rect){area.x, 0, W - area.x, y2 - H};
            if (r == 3) sub = (SDL_Rect){0, 0, x2 - W, y2 - H};
        }

        int start_x = sub.x / grid->taille_w;
        int start_y = sub.y / grid->taille_h;

        int nb_x = sub.w / grid->taille_w + 1;
        int nb_y = sub.h / grid->taille_h + 1;

        for (int i = 0; i < nb_x; i++) {
            for (int j = 0; j < nb_y; j++) {

                int ii = (start_x + i) % grid->nb_case_x;
                int jj = (start_y + j) % grid->nb_case_y;

                if (ii < 0) ii += grid->nb_case_x;
                if (jj < 0) jj += grid->nb_case_y;

                pnj_list_t *cell = grid->grid[ii][jj];

                if (cell->list) {
                    for (int k = 0; k < cell->nb_pnj; k++) {

                        pnj_t *pnj = cell->list[k];

                        if (sub.x <= pnj->pos.x &&
                            pnj->pos.x < sub.x + sub.w &&
                            sub.y <= pnj->pos.y &&
                            pnj->pos.y < sub.y + sub.h) {
                            if (pnj->etat.id!=0)
                            add_pnj_to_list(result, pnj);
                        }
                    }
                }
            }
        }
    }

    return result;
}


void kill_pnj(pnj_grid_t* monde_pnj, pnj_t * perso, SDL_Rect taille_monde){
    sup_pnj_from_grid(monde_pnj, perso, taille_monde);
    free(perso);
}


//***************************DECOR***************************/


decor_t* create_decor(int id, SDL_Texture* visuel, int id_etat,int rang, SDL_Rect pos) {
    decor_t* new_decor = (decor_t*)malloc(sizeof(decor_t));
    
    new_decor->type_decor = id;
    new_decor->visu = visuel;
    new_decor->etat.id = id_etat;
    new_decor->etat.para = NULL;
    new_decor->etat.step =rang;
    new_decor->pos = pos;
    return new_decor;
}



void place_decor_in_grid(decor_grid_t* grid, decor_t *decor, SDL_Rect taille_monde) {

    int case_x = wrap(decor->pos.x, taille_monde.x) / grid->taille_w;
    int case_y = wrap(decor->pos.y, taille_monde.y) / grid->taille_h;

    case_x = wrap(case_x, grid->nb_case_x);
    case_y = wrap(case_y, grid->nb_case_y);

    if (case_x >= 0 && case_x < grid->nb_case_x && case_y >= 0 && case_y < grid->nb_case_y) {
        add_decor_to_list(grid->grid[case_x][case_y], decor);
    } else {
        fprintf(stderr, "Decor position out of grid bounds\n");
    }
}

void sup_decor_from_grid(decor_grid_t* grid, decor_t *decor, SDL_Rect taille_monde) {

    int case_x = wrap(decor->pos.x, taille_monde.x) / grid->taille_w;
    int case_y = wrap(decor->pos.y, taille_monde.y) / grid->taille_h;

    case_x = wrap(case_x, grid->nb_case_x);
    case_y = wrap(case_y, grid->nb_case_y);

    if (case_x >= 0 && case_x < grid->nb_case_x && case_y >= 0 && case_y < grid->nb_case_y) {
        sup_decor_from_list(grid->grid[case_x][case_y], decor);
    } else {
        fprintf(stderr, "Decor position out of grid bounds\n");
    }
}

decor_list_t * get_decor_list_in(decor_grid_t* grid, SDL_Rect area, SDL_Rect taille_monde) {


    decor_list_t *result = init_decor_list();

    int W = taille_monde.w;
    int H = taille_monde.h;

    int x2 = area.x + area.w;
    int y2 = area.y + area.h;

    // Nombre max de splits (jusqu'à 4 rectangles)
    int nb_rect = 1;
    if (x2 > W && y2 > H) nb_rect = 4;
    else if (x2 > W || y2 > H) nb_rect = 2;

    for (int r = 0; r < nb_rect; r++) {

        SDL_Rect sub;

        // Cas sans wrap
        if (nb_rect == 1) {
            sub = area;
        }
        // Split horizontal
        else if (nb_rect == 2 && x2 > W) {
            if (r == 0) sub = (SDL_Rect){area.x, area.y, W - area.x, area.h};
            else        sub = (SDL_Rect){0, area.y, x2 - W, area.h};
        }
        // Split vertical
        else if (nb_rect == 2) {
            if (r == 0) sub = (SDL_Rect){area.x, area.y, area.w, H - area.y};
            else        sub = (SDL_Rect){area.x, 0, area.w, y2 - H};
        }
        // Split en 4
        else {
            if (r == 0) sub = (SDL_Rect){area.x, area.y, W - area.x, H - area.y};
            if (r == 1) sub = (SDL_Rect){0, area.y, x2 - W, H - area.y};
            if (r == 2) sub = (SDL_Rect){area.x, 0, W - area.x, y2 - H};
            if (r == 3) sub = (SDL_Rect){0, 0, x2 - W, y2 - H};
        }

        int start_x = sub.x / grid->taille_w;
        int start_y = sub.y / grid->taille_h;

        int nb_x = sub.w / grid->taille_w + 1;
        int nb_y = sub.h / grid->taille_h + 1;

        for (int i = 0; i < nb_x; i++) {
            for (int j = 0; j < nb_y; j++) {

                int ii = (start_x + i) % grid->nb_case_x;
                int jj = (start_y + j) % grid->nb_case_y;

                if (ii < 0) ii += grid->nb_case_x;
                if (jj < 0) jj += grid->nb_case_y;

                decor_list_t *cell = grid->grid[ii][jj];

                if (cell->list) {
                    for (int k = 0; k < cell->nb_decor; k++) {

                        decor_t *decor = cell->list[k];

                        if (sub.x <= decor->pos.x &&
                            decor->pos.x < sub.x + sub.w &&
                            sub.y <= decor->pos.y &&
                            decor->pos.y < sub.y + sub.h) {
                            if (decor->etat.id!=0)
                            add_decor_to_list(result, decor);
                        }
                    }
                }
            }
        }
    }

    return result;
}

void kill_decor(decor_grid_t* monde_decor, decor_t ** cible, SDL_Rect taille_monde){

    sup_decor_from_grid(monde_decor, *cible, taille_monde);
}

//***************************MONDE**************************

pnj_grid_t* creat_grid_pnj(int monde_x, int monde_y, int taille_w, int taille_h) {

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

    for (int i = 0; i < grid->nb_case_x; i++) {
        for (int j =0;j < grid->nb_case_y; j++){
            free_list_decor(grid->grid[i][j]);
        }
        free(grid->grid[i]);
    }
    free(grid->grid);
    free(grid);
}