#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "thread_travail.h"
#include "reinforce.h"
#include "structure.h"

void * travail_thread(void * arg){
    thread_val_t * data = (thread_val_t *)arg;

    float **theta = malloc(data->nb_actions * sizeof(float *));
    for (int i = 0; i < data->nb_actions; i++) {
        theta[i] = malloc(14 * sizeof(float));
        memcpy(theta[i], data->theta_res[i], 14 * sizeof(float));
    }

    algo_REINFORCE(data->gamma,data->N,data->alpha,data->ecart_type,
                   data->nb_centres,data->nb_actions,data->trajectoires,
                   data->theta_res,data->tailles_traj,data->min_centre,data->max_centre);

    for (int i = 0; i < data->nb_actions; i++) {
        memcpy(data->theta_res[i], theta[i], 14 * sizeof(float));
        free(theta[i]);
    }
    free(theta);

    pthread_exit(NULL);
}