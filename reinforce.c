#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "reinforce.h"
#include "structure.h"

#define M_PI 3.14159265358979323846

/**
 * Calcule val^n de manière récursive
 */
float puissance(float val, int n) {
    if (n == 0) {
        return 1.0;
    }
    if (n == 1) {
        return val;
    }
    return val * puissance(val, n - 1);
}

/**
 * Fonction de base radiale (RBF) gaussienne
 * Permet d'approximer une fonction continue avec des paramètres theta
 */
float phi(float s, float centre, float ecart_type) {
    float diff = s - centre;
    float exponent = -puissance(diff, 2) / (2.0 * puissance(ecart_type, 2));
    return exp(exponent);
}

/**
 * Calcule la moyenne de la politique mu(s) = sum_i theta_i * phi_i(s)
 * C'est l'approximateur de fonction : on combine les RBF avec les poids theta
 */
float politique_moyenne(float s, float *theta, int nb_centres, float ecart_type) {
    float mu = 0.0;
    
    // Les centres sont uniformément espacés dans [-1, 1] par exemple
    float min_centre = -1.0;
    float max_centre = 1.0;
    
    for (int i = 0; i < nb_centres; i++) {
        // Position du centre i
        float centre = min_centre + (float)i * (max_centre - min_centre) / (nb_centres - 1);
        
        // RBF évaluée à l'état s
        float phi_i = phi(s, centre, ecart_type);
        
        // Contribution pondérée
        mu += theta[i] * phi_i;
    }
    
    return mu;
}

/**
 * Calcule le gradient du log de la politique gaussienne
 * log p(a|s,theta) = -(a - mu(s))^2 / (2*sigma^2) - log(sigma*sqrt(2*pi))
 * 
 * gradient = d(log p)/d(theta) = (a - mu(s))/sigma^2 * d(mu)/d(theta)
 * où d(mu)/d(theta_i) = phi_i(s)
 */
void gradient_log_policy(float s, float action, float mu, float sigma,
                         float *theta, int nb_centres, float ecart_type,
                         float *gradient) {
    (void)theta;  // Inutilisé mais garde la signature
    
    // Terme commun : (action - mu(s)) / sigma^2
    float score = (action - mu) / (sigma * sigma);
    
    // Calculer les RBF pour chaque centre
    float min_centre = -1.0;
    float max_centre = 1.0;
    
    for (int i = 0; i < nb_centres; i++) {
        float centre = min_centre + (float)i * (max_centre - min_centre) / (nb_centres - 1);
        float phi_i = phi(s, centre, ecart_type);
        
        // gradient[i] = score * phi_i(s)
        gradient[i] = score * phi_i;
    }
}

/**
 * Algorithme REINFORCE - version continue avec politique gaussienne
 * 
 * Pour chaque trajectoire:
 *   - Pour chaque pas t dans la trajectoire (de T-1 à 0):
 *     - Calculer le retour cumulatif G_t = sum_{k=t}^{T-1} gamma^(k-t) * r_k
 *     - Calculer le gradient du log de la politique
 *     - theta += alpha * G_t * gradient
 */
void algo_REINFORCE(float gamma, int N, float alpha, float sigma,
                    int nb_centres, float ecart_type,
                    float ***trajectoires, float *theta, int *tailles_traj) {
    
    // Boucle sur chaque trajectoire
    for (int i = 0; i < N; i++) {
        float **traj = trajectoires[i];
        int T = tailles_traj[i];  // Longueur réelle de la trajectoire
        
        if (T < 1) continue;  // Vérifier trajectoire valide
        
        float G = 0.0;  // Retour cumulatif, calculé en arrière
        
        // Parcourir la trajectoire en sens inverse
        for (int t = T - 1; t >= 0; t--) {
            // Structure de la trajectoire : traj[t] = [état, action, récompense]
            float s = traj[t][0];           // État
            float action = traj[t][1];      // Action prise
            float reward = traj[t][2];      // Récompense reçue
            
            // Actualiser le retour cumulatif
            // G_t = r_t + gamma * G_{t+1}
            G = reward + gamma * G;
            
            // Calculer la moyenne de la politique au pas t
            float mu = politique_moyenne(s, theta, nb_centres, ecart_type);
            
            // Calculer le gradient du log de la politique
            float *grad = malloc(nb_centres * sizeof(float));
            gradient_log_policy(s, action, mu, sigma, theta, nb_centres, ecart_type, grad);
            
            // Mettre à jour les paramètres
            // theta += alpha * G_t * grad(log p(a|s,theta))
            for (int j = 0; j < nb_centres; j++) {
                theta[j] += alpha * G * grad[j];
            }
            
            free(grad);
            
            if (print_var) {
                printf("[REINFORCE] traj %d, step %d: s=%.3f, a=%.3f, r=%.3f, G=%.3f, mu=%.3f\n",
                       i, t, s, action, reward, G, mu);
            }
        }
    }
}
