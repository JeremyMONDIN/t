#ifndef REINFORCE
#define REINFORCE

#include "structure.h"
#include <stdlib.h>
#include <math.h>

/**
 * @brief Calcule val^n
 * @param val la valeur d'entrée
 * @param n le degré de la puissance
 * @return (val)^n
 */
float puissance(float val, int n);

/**
 * @brief Fonction de base radiale gaussienne (RBF)
 * @param s l'état d'entrée (continu)
 * @param centre le centre de la fonction gaussienne
 * @param ecart_type l'écart-type sigma
 * @return exp(-(s - centre)^2 / (2 * sigma^2))
 */
float phi(float s, float centre, float ecart_type);

/**
 * @brief Calcule la moyenne des actions pondérée par les poids theta
 * @param s l'état d'entrée
 * @param theta les paramètres de la politique
 * @param nb_centres nombre de centres RBF
 * @param ecart_type écart-type des RBF
 * @return mu(s) = sum(theta_i * phi_i(s))
 */
float politique_moyenne(float s, float * theta, int nb_centres, float ecart_type);

/**
 * @brief Calcule le gradient du log de la politique par rapport à theta
 * @param s l'état d'entrée
 * @param action l'action prise
 * @param mu la moyenne de la politique mu(s)
 * @param sigma l'écart-type de la politique
 * @param theta les paramètres de la politique
 * @param nb_centres nombre de centres RBF
 * @param ecart_type écart-type des RBF
 * @param gradient tableau où stocker le gradient [OUTPUT]
 */
void gradient_log_policy(float s, float action, float mu, float sigma, 
                         float *theta, int nb_centres, float ecart_type, 
                         float *gradient);

/**
 * @brief Algorithme REINFORCE pour actions continues
 * @param gamma Facteur d'actualisation
 * @param N nombre de trajectoires
 * @param alpha Taux d'apprentissage
 * @param sigma Écart-type de la politique (exploration)
 * @param nb_centres nombre de fonctions de base (RBF)
 * @param ecart_type écart-type des RBF
 * @param trajectoires trajectoires collectées [état, action, récompense] pour chaque pas
 * @param theta paramètres de la politique à mettre à jour [INPUT/OUTPUT]
 * @param tailles_traj tableau contenant la longueur de chaque trajectoire
 */
void algo_REINFORCE(float gamma, int N, float alpha, float sigma,
                    int nb_centres, float ecart_type,
                    float ***trajectoires, float *theta, int *tailles_traj);

#endif
