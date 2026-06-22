#include <stdio.h>
#include <math.h>

#include "reinforce.h"
#include "structure.h"

#define val_gamma 0.999

float puissance(float val, int n){
    if (n==1){
        return val;
    }
    else{
        return val * puissance(val, n-1);
    }
}

float phi(int s, int indice, float ecart_type){
    //float val = exp(-(puissance(s - ,2)/(2*puissance(ecart_type,2))));
    float val = 0;
    return val;
}

//version continue du reinforce
void algo_REINFORCE(int gamma, int N, int alpha, float ecart_type,
               float *** trajectoires, int * theta){
    int nb_element = 10; //tempo
    int * D = malloc(nb_element * sizeof(int));

    for (int i = 0; i<N; i++){
        float ** traj = trajectoires[i];
        int T = sizeof(traj); //pb taille 
        int G = 0;
        for (int u = 0; u<T ; u++){
            int indice = T - 1 - u;
            traj[indice] = trajectoires[indice];
            G = traj[indice][2] + gamma * G;
            int GG = puissance(gamma,indice) * G; //G*G, Pondération du retour à l'instant t
            float action_moy = 0.0; //temporaire
            //Gradient du log-policy à l'instant t
            float score = ((traj[indice][1] - action_moy) * 1)/(puissance(ecart_type,2));
            //accumulation de la contribution 
        }
        
    }
}