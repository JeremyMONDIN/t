#ifndef STRUCT
#define STRUCT

#include <SDL2/SDL.h>

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#define wrap(v,max) (((v)%(max)+(max))%(max))

typedef struct caracteristics{
    int vitesse;
    int force;
    int vision;
} caract_t;

typedef struct etat{
    int id;
    int step;
    int x;
    int y;
    void * para;
}etat_t;

typedef struct espece{
    char nom[50];
    int alim;
    SDL_Texture * visuel;
    caract_t caract;
    float theta[7][14];
} espece_t;

typedef struct maillon_traj{
    float s[14];
    int a;
    float r;
} maillon_traj_t;

typedef struct traj{
    float recomp_totale;
    int taille;
    int capa;
    maillon_traj_t * liste;
} traj_t;

typedef struct pnj{
    espece_t* espece;
    int energie;
    int vie;
    int faim;
    caract_t caract;
    SDL_Rect pos;
    etat_t etat;
    int lock_repro;
    int lock_anim;
    traj_t trajectoire;
} pnj_t;

typedef struct pnj_list{
    int nb_pnj;
    pnj_t ** list;
}pnj_list_t;

typedef struct decor{
    SDL_Rect pos;
    int type_decor;
    SDL_Texture * visu;
    etat_t etat;
} decor_t;

typedef struct decor_list{
    int nb_decor;
    decor_t ** list;
}decor_list_t;

typedef struct perc
{
    pnj_t *perso;
    int nb_nouriture;
    decor_t *nouriture;
    int nb_allie;
    pnj_t *allie;
    int nb_ennemi;
    pnj_t *ennemi;
    int nb_proie;
    pnj_t* proie;
} perception_t;

typedef struct reponse{
    int act;
    int x;
    int y;
    void * depla;
} reponse_t;

typedef struct space_grid{
    int nb_case_x;
    int nb_case_y;
    int taille_w;
    int taille_h;
    pnj_list_t *** grid;
} pnj_grid_t;



typedef struct decor_grid{
    int nb_case_x;
    int nb_case_y;
    int taille_w;
    int taille_h;
    decor_list_t *** grid;
} decor_grid_t;





/*typedef enum {
    ENTITY_PNJ,
    ENTITY_DECOR,
} entity_t;*/

#endif