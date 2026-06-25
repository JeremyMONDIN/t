#ifndef TEXT
#define TEXT

#include "structure.h"

maillon_traj_t creer_maillon(int a, float s[14], float r);

void save_traj(FILE *file,traj_t traj);

traj_t load_traj(FILE *file);

void save_tab(FILE *file,float tab[7][14]);

void load_tab(FILE *file, float tab[7][14]);

void save_espece(FILE *file, espece_t esp);

espece_t load_espece(FILE *file);

void save_all_traj(FILE * file,pnj_list_t* pnj_l);

void save_all_espece(FILE *file,espece_t *tab_esp,int n);

espece_t* load_all_espece(FILE *file, int *n_out);



#endif