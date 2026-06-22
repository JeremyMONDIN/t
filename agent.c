#include "structure.h"
#include "liste.h"
#include "agent.h"
#include <math.h>



reponse_t action(int s, int x, int y, decor_t* nourriture, pnj_t* allie, pnj_t* ennemi, pnj_t* proie){
    /*Fonction afin de determiner l'action à faire*/
    if (print_var) printf("action\n");
    reponse_t rep={0,-1,-1,NULL};
    switch (s)
    {
    case 0:
        rep.act=s+1;
        rep.x = x;
        rep.y = y;
        break;
    case 1:
        rep.act=s+1;
        rep.depla= nourriture;
        break;
    case 2:
        rep.act=s+1;
        rep.depla= ennemi;
        break;
    case 3:
        rep.act=s+1;
        rep.depla= proie;
        break;
    case 4:
        rep.act=s+1;
        rep.depla= allie;
        break;
    case 5:
        rep.act=s+1;
        rep.depla= ennemi;
        break;
    case 6:
        rep.act=s+1;
        rep.depla= NULL;
        break;
    }
    return rep;
}

float dist(SDL_Rect a, SDL_Rect b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

//{base, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, nb_proie, d_nourriture, d_allie, d_ennemi, d_proie, rapport_force, rapport_vitesse}

float coef_proie[7][14]={

/* EXPLORER */
{0.5, 0, 0.5,-0.5,-1.0, 0.2,-1.0,-1.0, 0.1,-2.0, 0,0,0,0},

/* MANGER */
{0.0, 0, 0.2, 3.0, 1.0, 0.0,-2.0, 3.0, 0.0,-3.0, 0,0,0,0},

/* DEFENDRE */
{-1.0,1.0,0.5,-0.5,0,0,-0.5,0,0,2.0,2.0,1.5,0,0},

/* ATTAQUER */
{-5.0,1.0,0.5,-1.0,0,0,-1.0,0,0,1.5,1.5,1.0,0,0},

/* REPRODUCTION */
{-1.0,0.5,2.0,-2.0,0.5,1.0,-3.0,0.5,2.0,-4.0,0,0,0,0},

/* FUIR */
{0.0,0.5,0.5,-0.5,0,0,2.0,0,0,4.0,-2.0,-2.0,0,0},

/* REPOS */
{0.0,0.5,-2.0,-1.0,0,0,-1.0,0,0,-2.0,0,0,0,0}
};


float coef_carn[7][14]={

/* EXPLORER */
{0.5,0,0.5,-0.5,0,0.2,-0.5,0,0.1,-1.0,0,0,1.0,1.0},

/* MANGER */
{0.0,0,0.2,2.5,1.0,0,-1.0,2.0,0,-2.0,0,0,1.0,2.0},

/* DEFENDRE */
{-0.5,1.0,0.5,-0.5,0,0,-0.5,0,0,2.0,2.0,2.0,0,0},

/* ATTAQUER */
{100.5,1.0,2.0,0,0,-0.5,0,0,-1.0,2.5,2.5,2.0,3.0},

/* REPRODUCTION */
{-1.0,0.5,2.0,-2.0,0,1.0,-2.0,0,2.0,-2.0,0,0,0,0},

/* FUIR */
{-1.0,0.5,0.5,-0.5,0,0,2.0,0,0,3.0,-3.0,-3.0,0,0},

/* REPOS */
{0.0,0.5,-2.0,-1.0,0,0,-0.5,0,0,-1.0,0,0,0,0}
};


float coef_omni[7][14]={

/* EXPLORER */
{0.5,0,0.5,-0.5,-0.5,0.2,-0.5,-0.5,0.1,-1.0,0,0,0.5,0.5},

/* MANGER */
{0.0,0,0.5,2.5,1.5,0,-1.5,2.0,0,-2.0,0,0,1.0,1.5},

/* DEFENDRE */
{-0.5,1.0,0.5,-0.5,0,0,-0.5,0,0,2.0,2.0,2.0,0,0},

/* ATTAQUER */
{-0.5,0.5,1.0,1.5,0,0,-0.5,0,0,-1.0,2.0,2.0,1.5,2.0},

/* REPRODUCTION */
{-1.0,0.5,2.0,-2.0,0.5,1.0,-2.5,0.5,2.0,-3.0,0,0,0,0},

/* FUIR */
{-0.5,0.5,0.5,-0.5,0,0,2.0,0,0,3.0,-2.5,-2.5,0,0},

/* REPOS */
{0.0,0.5,-2.0,-1.0,0,0,-0.5,0,0,-1.0,0,0,0,0}
};

static float somme_tab(
    float tab[7][14],
    int k,
    int vie,
    int energie,
    int faim,
    int nb_nourriture,
    int nb_allie,
    int nb_ennemi,
    int nb_proie,
    float d_nourriture,
    float d_allie,
    float d_ennemi,
    float d_proie,
    float rapport_force,
    float rapport_vitesse)
{
    return  tab[k][0] +
            tab[k][1] * vie/100 +
            tab[k][2] * energie/100 +
            tab[k][3] * faim/100 +
            tab[k][4] * nb_nourriture +
            tab[k][5] * nb_allie +
            tab[k][6] * nb_ennemi +
            tab[k][7] * d_nourriture +
            tab[k][8] * d_allie +
            tab[k][9] * d_ennemi +
            tab[k][10] * rapport_force +
            tab[k][11] * rapport_vitesse +
            tab[k][12] * nb_proie +
            tab[k][13] * d_proie;
}

reponse_t reaction_entite(perception_t per,float tab[7][14])
{
    if (print_var)
        printf("reaction_entite\n");
    float T[7] = {0, 0, 0, 0, 0, 0}; // Intérêt de base
    // t={explorer, manger, defendre, attaquer, chaser, reproduiction, fuir, repos}
    int faim = per.perso->faim,
        energie = per.perso->energie,
        vie = per.perso->vie,
        nb_nourriture = min(per.nb_nouriture,10),
        nb_ennemi = min(per.nb_ennemi,10),
        nb_allie = min(per.nb_allie,10);
    
    float rapport_force = 0,
          rapport_vitesse = 0,
          d_nourriture = 0,
          d_ennemi = 0,
          d_allie = 0;

    if (per.ennemi != NULL){
        d_ennemi =1/(1+ dist(per.perso->pos, per.ennemi->pos));
        rapport_force =1/(1 + per.ennemi->caract.force / per.perso->caract.force);
        rapport_vitesse =1/(1 + per.ennemi->caract.vitesse / per.perso->caract.vitesse);
    }
    if (per.nouriture != NULL) d_nourriture = dist(per.perso->pos, per.nouriture->pos);

    if (per.allie != NULL) d_allie = dist(per.perso->pos, per.allie->pos);

    
    int x=per.perso->pos.x+rand()%100-50;
    int y=per.perso->pos.y+rand()%100-50;

    //mise a jour des intérers

    // explorer
    T[0] = somme_tab( tab, 0, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, 0, d_nourriture, d_allie, d_ennemi, 0, rapport_force, rapport_vitesse);
    // manger
    T[1] = somme_tab( tab, 1, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, 0, d_nourriture, d_allie, d_ennemi, 0, rapport_force, rapport_vitesse);
    // defendre
    T[2] = somme_tab( tab, 2, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, 0, d_nourriture, d_allie, d_ennemi, 0, rapport_force, rapport_vitesse);
    //chaser
    T[3] = somme_tab( tab, 3, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, 0, d_nourriture, d_allie, d_ennemi, 0, rapport_force, rapport_vitesse);
    // reproduction
    T[4] = somme_tab( tab, 4, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, 0, d_nourriture, d_allie, d_ennemi, 0, rapport_force, rapport_vitesse);
    // fuite
    T[5] = somme_tab( tab, 5, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, 0, d_nourriture, d_allie, d_ennemi, 0, rapport_force, rapport_vitesse);
    // repos
    T[6] = somme_tab( tab, 6, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, 0, d_nourriture, d_allie, d_ennemi, 0, rapport_force, rapport_vitesse);

    //printf("T = %f %f %f %f %f %f %f\n",T[0],T[1],T[2],T[3],T[4],T[5],T[6]);

    /*Fonction pour transformer des intérêts en probabilité*/
    float *T2 = malloc(7 * sizeof(float));
    float compt = 0;
    for (int i = 0; i < 7; i++)
    {
        T2[i] = exp(T[i] / 100);
        compt += T2[i];
    }

    if (per.ennemi == NULL){
        compt -= T2[2] + T2[3] + T2[5];
        T2[2]=0;
        T2[3]=0;
        T2[5]=0;
    }
    if (per.nouriture == NULL) {
        compt -= T2[1];
        T2[1]=0;
    }
    if (per.allie == NULL) {
        compt -= T2[4];
        T2[4]=0;
    }

    for (int j = 0; j < 7; j++)
    {
        T2[j] = T2[j] / compt;
    }

    /*Fonction afin de determiner l'action à faire*/
    float alpha=(float)rand()/(float)RAND_MAX;
    int s=6;
    float cumul=0;
    for (int i=0; i<s; i++){
        cumul+=T2[i];
        if(alpha<cumul){
            s=i;
            break;
        }
    }
    free(T2);
    //printf("alpha %f s %d tab %f %f %f %f %f %f %f \n",alpha,s,T2[0],T2[1],T2[2],T2[3],T2[4],T2[5],T2[6]);

    return action(s,x,y,per.nouriture,per.allie,per.ennemi,per.proie);
}



reponse_t reaction(perception_t p)
{
    if (print_var)
        printf("reaction\n");
    if (p.perso->espece->alim == 0)
    {
        return reaction_entite(p,coef_proie);
    }
    else if (p.perso->espece->alim == 1)
    {
        return reaction_entite(p,coef_carn);
    }
    else
    {
        return reaction_entite(p,coef_omni);
    }
}