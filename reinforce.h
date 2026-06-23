#ifndef REINFORCE
#define REINFORCE

#include "structure.h"

/**
 * @brief permet de calculer la valeur d'une puissance
 * @param val la valeur d'entrée qui subit la puissance
 * @param n le degré de la puissance
 * @return (val)^n
 */
float puissance(float val, int n);

/**
 * @brief Calcule l'ecart-type basé sur les probas de chaque action
 * @param probas le tableau des probabilités
 * @param nb_etats les états possibles des animaux
 * @return l'écart-type
 */
float sigma(float * probas, int nb_etats);

/**
 * @brief permet de calculer le phi à un certain indice
 * @param s état d'entrée continu
 * @param centre centre de la fonction gaussienne
 * @param ecart_type l'écart type sigma
 * @return le phi
 */
float phi(float s, float centre, float ecart_type);

/**
 * @brief Calcule la politique moyenne (mu) pour un certain état courant
 * @param s l'état d'entrée (continu)
 * @param theta la fonction des paramètres à modifier
 * @param nb_centres nombre de fonctions de base
 * @param ecart_type l'écart type de la fonction étudiée
 * @return Mu (politique moyenne)
 */
float politique_moyenne (float s, float * theta, int nb_centres, float ecart_type);

/**
 * @brief Permet de calculer le log policy du Gradient pour le renforcement
 * @param s l'état d'entrée (continu)
 * @param action l'action à l'état t
 * @param mu la politique moyenne de l'état t
 * @param ecart_type l'écart type de la fonction étudiée
 * @param theta la fonction des paramètres à modifier
 * @param nb_centres le nombre de fonctions de base
 * @param gradient la fonction gradient qu est utilisée pour la montée de grad
 */
void gradient_log_policy(float s, float action, float mu, float ecart_type,
                         float * theta,
                         int nb_centres, float * gradient);
/** 
 * @brief permet l'apprentissage par renforcement
 * @param gamma Facteur d'actualisation (permet de converger vers une unique valeur)
 * @param N le nombre de trajectoires
 * @param alpha Le pas (ou taux) d'apprentissage modifiant l'ampleur des modifs
 * @param ecart_type L'ecart-type de la fonction étudiée
 * @param nb_centres nombre de fonctions de base
 * @param trajectoires le tableau de trajectoires à étudier
 * @param theta les paramètres theta 
 * @param tailles_traj tableau de la longueur des trajectoires
*/
void algo_REINFORCE(float gamma, int N, int alpha, float ecart_type, int nb_centres,
                    float *** trajectoires, float * theta, int * tailles_traj);
#endif