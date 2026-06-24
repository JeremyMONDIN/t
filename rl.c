#include <stdlib.h>
#include <math.h>
#include "rl.h"

#define NB_ACTIONS 7
#define NB_CENTRES 5

float theta[NB_ACTIONS * NB_CENTRES] = {0};

// calcule probabilités (softmax)
void calculer_proba(float s, float *P)
{
    float sigma = 0.2;
    float somme = 0;

    for (int a = 0; a < NB_ACTIONS; a++)
    {
        float score = 0;
        for (int k = 0; k < NB_CENTRES; k++)
        {
            float centre = (float)k / (NB_CENTRES - 1);
            score += theta[a * NB_CENTRES + k] * phi(s, centre, sigma);
        }

        P[a] = exp(score);
        somme += P[a];
    }

    for (int a = 0; a < NB_ACTIONS; a++)
        P[a] /= somme;
}

// tirage aléatoire
int choisir_action(float s)
{
    float P[NB_ACTIONS];
    calculer_proba(s, P);

    float r = (float)rand() / RAND_MAX;
    float cumul = 0;

    for (int a = 0; a < NB_ACTIONS; a++)
    {
        cumul += P[a];
        if (r < cumul) return a;
    }

    return NB_ACTIONS - 1;
}