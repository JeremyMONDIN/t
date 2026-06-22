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
 * @brief permet de calculer le phi à un certain indice
 * @param etat_suivant le s de l'état suivant
 * @param indice l'indice du centre choisi
 * @param ecart_type l'écart type pour cette valeur de centre
 * @return le phi
 */
float phi(int etat_suivant, int indice, float ecart_type);

/** 
 * @brief permet l'apprentissage par renforcement
 * @param gamma Facteur d'actualisation (permet de converger vers une unique valeur)
 * @param N le nombre de trajectoires
 * @param alpha Le pas (ou taux) d'apprentissage modifiant l'ampleur des modifs
 * @param ecart_type L'ecart-type de la fonction étudiée
 * @param trajectoires le tableau de trajectoires à étudier
 * @param theta les paramètres theta 
*/
void algo_REINFORCE(int gamma, int N, int alpha, float ecart_type,
               float *** trajectoires, int * theta);

#endif