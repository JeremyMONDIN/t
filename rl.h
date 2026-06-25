#ifndef RL_H
#define RL_H

#include "structure.h"

int nb_aleatoire(uint32_t seed);
void initialiser_espece_rl(espece_t * espece, int nb_actions, int nb_centres);
int choisir_action_espece(float s, espece_t * espece, int nb_actions, int nb_centres);
void entrainer_espece(espece_t * espece, 
                      traj_t ** trajectoires, 
                      int * tailles_traj,
                      int N, 
                      int nb_actions,
                      int nb_centres, int min_centre,
                    int max_centre );
#endif