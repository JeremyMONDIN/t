#ifndef THREAD_WORKER_H
#define THREAD_WORKER_H

#include <pthread.h>
#include "agent.h"
#include "reinforce.h"
#include "structure.h"

typedef struct thread_val{
    traj_t ** trajectoires; //
    float theta_res[7][14]; //
    int * tailles_traj; //
    int id_espece; //
    int min_centre; //
    int max_centre; //
    float gamma; //
    float alpha; //
    float ecart_type; //
    int N; //
    int nb_centres;
    int nb_actions; //
} thread_val_t;

void * travail_thread(void * arg);

#endif