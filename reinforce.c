#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "reinforce.h"
#include "structure.h"


float puissance(float val, int n){
    if (n==0){
        return 1.0;
    }
    else{
        return val * puissance(val, n-1);
    }
}

float sigma(float * probas, int nb_etats){
    float E_x = 0; //espérance
    float E_x2 = 0; //espérance de X^2
    for (int i=0; i<nb_etats; i++){
        E_x += probas[i] * i;
        E_x2 += probas[i] * i * i;
    }
    float Var_x = E_x2 - (E_x * E_x); //Variance
    return sqrt(Var_x); //Ecart-type
}

float phi(float s, float centre, float ecart_type){
    float diff = s - centre;
    float val = -puissance(diff,2) / (2.0*puissance(ecart_type,2));
    return exp(val);
}

void calculer_probabilites(float s, float theta[7][14], int nb_centres, int nb_actions,
                           float ecart_type, float * P, int min_centre, int max_centre){
    float somme = 0.0;
    float centre;
    for (int i = 0; i <nb_actions; i++){
        float score_a = 0.0;
        for (int j = 0; j<nb_centres; j++){
            if (nb_centres>1)  centre = min_centre + j * (max_centre - min_centre) / (nb_centres - 1);
            score_a += theta[i][j] * phi(s,centre,ecart_type);
            //Tableau 2D vu en 1D (theta[i][j], si theta comporte plusieurs valeurs par coords)
        }
        P[i] = exp(score_a);
        somme += P[i];
    }
    //On normalise
    for (int i = 0; i< nb_actions;i++){
        P[i] /= somme;
    }
}

//

//version continue du reinforce
void algo_REINFORCE(float gamma, int N, float alpha, float ecart_type, int nb_centres, int nb_actions,
                    traj_t ** trajectoires, float theta[7][14], int * tailles_traj, int min_centre,
                    int max_centre){
    
    int taille_totale = nb_actions * nb_centres;
    //Matrice D 
    float * D = calloc(taille_totale, sizeof(float));
    //Vecteur de probabilités
    float * P = malloc(nb_actions * sizeof(float));

    for (int i = 0; i<N; i++){
        traj_t * traj = trajectoires[i]; //l'une des trajectoires prises
        int T = tailles_traj[i]; 
        float G = 0.0;

        
        for (int u = 0; u < T; u++){
            int t = T - 1 - u;
            float s = traj->liste[t].s; 
            int action = traj->liste[t].a;  
            float reward = traj->liste[t].r; 

            G = reward + gamma * G;
            float GG = puissance(gamma,t) * G;

            calculer_probabilites(s,theta,nb_centres,nb_actions,ecart_type,P, min_centre, max_centre);
            for (int a = 0; a<nb_actions; a++){
                for (int k = 0; k<nb_centres; k++){
                    float centre = min_centre + k * (max_centre - min_centre) / (nb_centres -1);
                    float phi_t_k = phi(s,centre,ecart_type);
                    float modif_a_k = 0.0;

                    if (a == action){
                        modif_a_k = phi_t_k;
                    }
                    modif_a_k -= P[a] * phi_t_k;
                    //Tableau 2D vu en 1D
                    D[a * nb_centres + k] += GG * modif_a_k;
            }
        }
    }
    for (int a = 0; a<nb_actions; a++){
        for (int k = 0; k <nb_centres; k++){
            theta[a][k]= alpha * (1.0/N) * D[a * nb_centres + k]; //2D => 1D
        }
    }
    }
    free(D);
    free(P);
}