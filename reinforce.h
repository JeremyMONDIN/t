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
 * @brief Permet de calculer le vecteur des probabilités P
 * @param s l'état entrant
 * @param theta le vecteur des paramètres
 * @param nb_centres le nombre de fonctions de base
 * @param nb_actions le nombre d'actions possibles
 * @param ecart_type l'écart type (sigma)
 * @param P le tableau des probabilités
 */
void calculer_probabilites(float s, float theta[7][14], int nb_centres, int nb_actions,
                           float ecart_type, float * P, int min_centre, int max_centre);
/** 
 * @brief permet l'apprentissage par renforcement
 * @param gamma Facteur d'actualisation (permet de converger vers une unique valeur)
 * @param N le nombre de trajectoires
 * @param alpha Le pas (ou taux) d'apprentissage modifiant l'ampleur des modifs
 * @param ecart_type L'ecart-type de la fonction étudiée
 * @param nb_centres nombre de fonctions de base
 * @param nb_actions le nombre d'actions différentes possibles
 * @param trajectoires le tableau de trajectoires à étudier
 * @param theta les paramètres theta 
 * @param tailles_traj tableau de la longueur des trajectoires
*/
void algo_REINFORCE(float gamma, int N, float alpha, float ecart_type, int nb_centres, int nb_actions,
                    traj_t ** trajectoires, float theta[7][14], int * tailles_traj, int min_centre,
                    int max_centre);
    
#endif