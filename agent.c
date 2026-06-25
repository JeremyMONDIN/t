#include "structure.h"
#include "liste.h"
#include "agent.h"
#include "rl.h"
#include "reinforce.h"
#include <math.h>



reponse_t action(int s, int x, int y, decor_t* nourriture, pnj_t* allie, pnj_t* ennemi, pnj_t* proie){

    reponse_t rep={0,-1,-1,NULL};
    switch (s)
    {
    case 0: //explorer
        rep.act=s+1;
        rep.x = x;
        rep.y = y;
        break;
    case 1: //manger
        rep.act=s+1;
        rep.depla= nourriture;
        break;
    case 2: //defendre
        rep.act=s+1;
        rep.depla= ennemi;
        break;
    case 3: //chasser
        rep.act=s+1;
        rep.depla= proie;
        break;
    case 4: //reproduire
        rep.act=s+1;
        rep.depla= allie;
        break;
    case 5: //fuir
        rep.act=s+1;
        rep.depla= ennemi;
        break;
    case 6: //repos
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



float somme_tab(
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
            tab[k][1] * vie +
            tab[k][2] * energie +
            tab[k][3] * faim +
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

reponse_t situation_entite(perception_t per,float tab[7][14], float s[14])
{
    int perception_discrete = 1;


    //float *T = malloc(7 * sizeof(float)); // Intérêt de base
    // t={explorer, manger, defendre, attaquer, chaser, reproduiction, fuir, repos}
    int faim = per.perso->faim ;
    if(perception_discrete) faim /= 33;
    int energie = per.perso->energie;
    if(perception_discrete) energie /= 33;
    int vie = per.perso->vie ;
    if(perception_discrete) vie /= 33;
    int nb_nourriture = min(per.nb_nouriture,5);
    int nb_ennemi = min(per.nb_ennemi,5);
    int nb_allie = min(per.nb_allie,5);
    int nb_proie = min(per.nb_allie,5);
    
    float rapport_force = 0,
          rapport_vitesse = 0,
          d_nourriture = 0,
          d_ennemi = 0,
          d_allie = 0,
          d_proie = 0;

    if (per.ennemi != NULL){
        if (perception_discrete) d_ennemi = floor(4 * dist(per.perso->pos, per.ennemi->pos) / per.perso->caract.vision);
        else d_ennemi = 1/(1 + dist(per.perso->pos, per.ennemi->pos));
        rapport_force = per.ennemi->caract.force / per.perso->caract.force;
        if (perception_discrete){
            if (rapport_force > 1.3) rapport_force = -1;
            else if (rapport_force < 0.7) rapport_force = 1;
            else rapport_force = 0;
        }
        rapport_vitesse = per.ennemi->caract.vitesse / per.perso->caract.vitesse;
        if (perception_discrete){
            if (rapport_vitesse > 1.3) rapport_vitesse = -1;
            else if (rapport_vitesse < 0.7) rapport_vitesse = 1;
            else rapport_vitesse = 0;
        }
    }
    if (per.proie != NULL){
        if (perception_discrete) d_proie = floor(4 * dist(per.perso->pos, per.proie->pos) / per.perso->caract.vision);
        else d_ennemi = 1/(1 + dist(per.perso->pos, per.proie->pos));
    }

    if (per.nouriture != NULL) {
        if (perception_discrete) d_nourriture = floor(4 * dist(per.perso->pos, per.nouriture->pos) / per.perso->caract.vision);
        else d_nourriture = 1/(1 + dist(per.perso->pos, per.nouriture->pos));
    }

    if (per.allie != NULL) {
        if (perception_discrete) d_allie = floor(4 * dist(per.perso->pos, per.allie->pos) / per.perso->caract.vision);
        else d_allie = 1/(1 + dist(per.perso->pos, per.allie->pos));
    }

    s[0] = 1;
    s[1] = vie;
    s[2] =energie;
    s[3] = faim;
    s[4] = nb_nourriture;
    s[5] = nb_allie;
    s[6] = nb_ennemi;
    s[7] = nb_proie;
    s[8] = d_nourriture;
    s[9] = d_allie;
    s[10] = d_ennemi;
    s[11] = d_proie;
    s[12] = rapport_force;
    s[13] = rapport_vitesse;
    /*
    int x=per.perso->pos.x+rand()%100-50;
    int y=per.perso->pos.y+rand()%100-50;

    //mise a jour des intérers

    // explorer
    T[0] = somme_tab( tab, 0, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, nb_proie, d_nourriture, d_allie, d_ennemi, d_proie, rapport_force, rapport_vitesse);
    // manger
    T[1] = somme_tab( tab, 1, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, nb_proie, d_nourriture, d_allie, d_ennemi, d_proie, rapport_force, rapport_vitesse);
    // defendre
    T[2] = somme_tab( tab, 2, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, nb_proie, d_nourriture, d_allie, d_ennemi, d_proie, rapport_force, rapport_vitesse);
    //chaser
    T[3] = somme_tab( tab, 3, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, nb_proie, d_nourriture, d_allie, d_ennemi, d_proie, rapport_force, rapport_vitesse);
    // reproduction
    T[4] = somme_tab( tab, 4, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, nb_proie, d_nourriture, d_allie, d_ennemi, d_proie, rapport_force, rapport_vitesse);
    // fuite
    T[5] = somme_tab( tab, 5, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, nb_proie, d_nourriture, d_allie, d_ennemi, d_proie, rapport_force, rapport_vitesse);
    // repos
    T[6] = somme_tab( tab, 6, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, nb_proie, d_nourriture, d_allie, d_ennemi, d_proie, rapport_force, rapport_vitesse);

    //printf("T = %f %f %f %f %f %f %f\n",T[0],T[1],T[2],T[3],T[4],T[5],T[6]);

    //Fonction pour transformer des intérêts en probabilité
    float *T2 = malloc(7 * sizeof(float));
    float compt = 0;
    for (int i = 0; i < 7; i++)
    {
        T2[i] = exp(T[i] / 30);
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
production
    T[4] = somme_tab( tab, 4, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, nb_proie, d_nourriture, d_allie, d_ennemi, d_proie, rapport_force, rapport_vitesse);
    // fuite
    T[5] = somme_tab( tab, 5, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, nb_proie, d_nourriture, d_allie, d_ennemi, d_proie, rapport_force, rapport_vitesse);
    // r
    for (int j = 0; j < 7; j++)
    {
        T2[j] = T2[j] / compt;
    }

    //Fonction afin de determiner l'action à faire
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
    printf("alpha %f s %d tab %f %f %f %f %f %f %f \n",alpha,s,T2[0],T2[1],T2[2],T2[3],T2[4],T2[5],T2[6]);
    free(T);
    free(T2);

    //if(per.perso->espece->alim == 1) printf("%p reac nb_proie %d %p,nb_ennemie %d %p\n",per.perso,per.nb_proie,per.proie,per.nb_ennemi,per.ennemi);
    return action(s,x,y,per.nouriture,per.allie,per.ennemi,per.proie);*/
}



reponse_t reaction(perception_t p)
{
    float s[14] = {0};
    // Choisir l'action selon le type d'espèce et ses paramètres theta

    situation_entite(p,p.perso->espece->theta,s);
    //printf("tab %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",s[0],s[1],s[2],s[3],s[4],s[5],s[6],s[7],s[8],s[9],s[10],s[11],s[11],s[12],s[13]);
    int action_id = choisir_action_espece(s, p.perso->espece, 7, 14);
    p.perso->trajectoire.liste[p.perso->trajectoire.taille].a=action_id;
    for (int i=0; i<14; i++){
        p.perso->trajectoire.liste[p.perso->trajectoire.taille].s[i]=s[i];
    }
    p.perso->trajectoire.taille+=1;

    int x = p.perso->pos.x + nb_aleatoire()%100 - 50;
    int y = p.perso->pos.y + nb_aleatoire()%100 - 50;

    return action(action_id, x, y,
                  p.nouriture,
                  p.allie,
                  p.ennemi,
                  p.proie);
}