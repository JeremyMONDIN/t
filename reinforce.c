#include <stdio.h>
#include <math.h>

#include "reinforce.h"
#include "structure.h"

static float val_gamma = 0.999;
static float min_centre = 0.0;
static float max_centre = 1.0;

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
        float E_x = probas[i] * i;
        float E_x2 = probas[i] * i * i;
    }
    float Var_x = E_x2 - (E_x * E_x); //Variance
    return root(Var_x); //Ecart-type
}

float phi(float s, float centre, float ecart_type){
    float diff = s - centre;
    float val = -puissance(diff,2) / (2.0*puissance(ecart_type,2));
    return exp(val);
}

float politique_moyenne (float s, float * theta, int nb_centres, float ecart_type){

    float mu = 0.0;
    for (int i = 0; i<nb_centres ; i++){
        //Centres unirepartis sur la distance max-min
        float centre = min_centre + i * (max_centre - min_centre)/(nb_centres-1);

        mu = mu + theta[i] * phi(s,centre,ecart_type);
    }

    return mu;
}

void gradient_log_policy(float s, float action, float mu, float ecart_type,
                         float * theta,
                         int nb_centres, float * gradient){
    //Terme commun
    float score = (action - mu)/puissance(ecart_type,2);

    for (int i = 0; i<nb_centres; i++){
        float centre = min_centre + i * (max_centre - min_centre)/(nb_centres-1);
        float phi_i = phi(s,centre,ecart_type);

        gradient[i] = score * phi_i; //gradient du log policy
    }

}

//version continue du reinforce
void algo_REINFORCE(float gamma, int N, int alpha, float ecart_type, int nb_centres,
                    traj_t ** trajectoires, float * theta, int * tailles_traj){

    for (int i = 0; i<N; i++){
        traj_t * traj = trajectoires[i]; //l'une des trajectoires prises
        int T = tailles_traj[i]; 
        float G = 0.0;

        for (int t = T-1; t>=0; t--){
            float s = traj->liste[t][0].s; //(s,a,r)
            float action = traj->liste[t][1].a;
            float reward = traj->liste[t][2].r;

            G = reward + gamma * G;
            float action_moy = politique_moyenne(s,theta,nb_centres,ecart_type); // == mu
            
            float * grad = malloc(nb_centres * sizeof(float));
            
            gradient_log_policy(s,action,action_moy,ecart_type,theta,nb_centres,grad);
            //Maj de theta
            for (int j = 0; j<nb_centres; j++){
                theta[j] += alpha * G * grad[j];
            }
            free(grad);
            
        }
        
    }
}