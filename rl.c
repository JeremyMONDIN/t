#include <stdlib.h>
#include <math.h>
#include "rl.h"
#include "structure.h"

#define NB_ACTIONS 7
#define NB_CENTRES 5

// Dans une fonction d'initialisation
void initialiser_espece_rl(espece_t * espece, int nb_actions, int nb_centres) {
    espece->theta = calloc(nb_actions * nb_centres, sizeof(float));
    espece->trained = 0;
}

// Créer une wrapper dans rl.c
void entrainer_espece(espece_t * espece, 
                      traj_t ** trajectoires, 
                      int * tailles_traj,
                      int N, 
                      int nb_actions,
                      int nb_centres)
{
    float gamma = 0.999;
    float alpha = 0.01;  // taux d'apprentissage
    float ecart_type = 0.2;
    
    algo_REINFORCE(gamma, N, alpha, ecart_type, 
                   nb_centres, nb_actions, 
                   trajectoires, espece->theta, tailles_traj);
    
    espece->trained = 1;
}

int choisir_action_espece(float s, espece_t * espece, int nb_actions, int nb_centres)
{
    float * P = malloc(nb_actions * sizeof(float));
    float sigma = 0.2;
    float somme = 0;

    for (int a = 0; a < nb_actions; a++)
    {
        float score = 0;
        for (int k = 0; k < nb_centres; k++)
        {
            float centre = (float)k / (nb_centres - 1);
            score += espece->theta[a * nb_centres + k] * phi(s, centre, sigma);
        }
        P[a] = exp(score);
        somme += P[a];
    }

    for (int a = 0; a < nb_actions; a++)
        P[a] /= somme;

    float r = (float)rand() / RAND_MAX;
    float cumul = 0;

    int action = 0;
    for (int a = 0; a < nb_actions; a++)
    {
        cumul += P[a];
        if (r < cumul) {
            action = a;
            break;
        }
    }
    
    free(P);
    return action;
}