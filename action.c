#include <stdio.h>
#include <math.h>
#include "action.h"
#include "grid.h"
#include "structure.h"
#include "Mj.h"

#define quad(a) (a * a)





static int deplacement_elmentaire(pnj_t *perso, int x, int y, int vitesse)
{
    if (print_var)
        printf("deplacement elementaire\n");
    int n = 0;
    float d = sqrt(x * x + y * y);
    if (perso->energie > cout_deplacement + quad(vitesse/vitesse_base))
    {
        perso->energie -= cout_deplacement + quad(vitesse/vitesse_base);
        if (d > deplacemant_par_tic)
        {
            x = floor(x / d * deplacemant_par_tic);
            y = floor(y / d * deplacemant_par_tic);
            n = 1;
        }
        perso->pos.x = wrap(perso->pos.x + x,monde_pnj->nb_case_x * monde_pnj->taille_w);
        perso->pos.y = wrap(perso->pos.y + y,monde_pnj->nb_case_y * monde_pnj->taille_h);
        perso->etat.step--;
    }
    else
        perso->etat.step = 0;
    return n;
}

static void attaquer(pnj_t *attaquant, pnj_t *cible)
{
    if (print_var)
        printf("attaquer\n");
    if (attaquant->energie > cout_attaque && cible->etat.id != 0)
    {
        float factor = (float)attaquant->caract.force / cible->caract.force;
        attaquant->energie -= cout_attaque;
        cible->vie -= floor(degats_attaque + coef_bonus_force * factor);
        if (cible->vie <= 0)
        {
            cible->etat.id = 0;
            cible->etat.step = 5; //arbitraire pour eviter un nouveau tirage avant la mort
            printf("%s kill %s\n",attaquant->espece->nom,cible->espece->nom);
        }
        attaquant->etat.step--;
    }
    else
        attaquant->etat.step = 0;
}

static pnj_t *breed(pnj_t *mere, pnj_t *pere)
{
    if (print_var)
        printf("breed\n");
    if (mere->energie > cout_reproduction && pere->energie > cout_reproduction && mere->lock_repro==0 && pere->lock_repro==0)
    {
        pnj_t* enfant=malloc(sizeof(pnj_t));
        enfant->espece = mere->espece;
        enfant->caract.vitesse = (mere->caract.vitesse + pere->caract.vitesse) / 2 ;
        if(evolution) enfant->caract.vitesse += rand() % 5 - 2;
        enfant->caract.force = (mere->caract.force + pere->caract.force) / 2 ;
        if(evolution) enfant->caract.force += rand() % 5 - 2;
        enfant->caract.vision = (mere->caract.vision + pere->caract.vision) / 2 ;
        if(evolution) enfant->caract.vision += rand() % 5 - 2;
        enfant->pos.x = (mere->pos.x + pere->pos.x) / 2;
        enfant->pos.y = (mere->pos.y + pere->pos.y) / 2;
        enfant->pos.w = mere->pos.w;
        enfant->pos.h = mere->pos.h;
        enfant->vie = 50;
        enfant->energie = 0;
        enfant->faim = 50;
        mere->energie -= cout_reproduction;
        pere->energie -= cout_reproduction;
        mere->etat.step = 0;
        pere->etat.step = 0;
        pere->lock_repro=200;
        mere->lock_repro=200;
        enfant->lock_repro=200;
        return enfant;
    }
    mere->etat.step = 0;
    pere->etat.step = 0;
    return NULL;
}

//**************ETATS******************************

void mourir(pnj_t *perso)
{
    if (print_var)
        printf("etat_mourir\n");
    add_decor(2,texture_death,2,coef_nouriture_par_corp ,(perso)->pos);
}

void explorer(pnj_t *perso, int cor_x, int cor_y)
{
    if (print_var)
        printf("etat_explorer\n");
    int x = cor_x - perso->pos.x;
    int y = cor_y - perso->pos.y;
    int n = deplacement_elmentaire(perso, x, y, min(perso->caract.vitesse,vitesse_base));
    if (n == 0)
    {
        perso->etat.step = 0;

    }

}

void manger(pnj_t *perso, decor_t *cible)
{
    if (print_var)
        printf("etat_manger\n");
    if (perso != NULL && cible != NULL)
    {
        float d = quad(perso->pos.x - cible->pos.x) + quad(perso->pos.y - cible->pos.y);
        int inter = quad(perso->pos.w) + quad(perso->pos.h) + quad(cible->pos.w) + quad(cible->pos.h);
        if (d > distance_interaction + inter)
        {
            deplacement_elmentaire(perso, cible->pos.x-perso->pos.x, cible->pos.y-perso->pos.y, perso->caract.vitesse);
        }
        else
        {
            if (cible->etat.id != 0)
            {
                perso->faim = min(100, perso->faim + gain_faim_plante * cible->type_decor);
                perso->energie = min(100, perso->energie + gain_energie);
                perso->etat.step--;
                cible->etat.step--;
                if (cible->etat.step <= 0)
                    cible->etat.id = 0;
            }
        }
    }
    else if (perso != NULL)
        perso->etat.step = 0;
}

void defendre(pnj_t *perso, pnj_t *cible)
{
    if (print_var)
        printf("etat_defendre\n");
    if (perso != NULL && cible != NULL)
    {
        float d = quad(perso->pos.x - cible->pos.x) + quad(perso->pos.y - cible->pos.y);
        int inter = quad(perso->pos.w) + quad(perso->pos.h) + quad(cible->pos.w) + quad(cible->pos.h);
        if (d > distance_interaction + inter)
        {
            deplacement_elmentaire(perso, cible->pos.x-perso->pos.x, cible->pos.y-perso->pos.y, perso->caract.vitesse);
        }
        else
        {
            attaquer(perso, cible);
        }
    }
    else if (perso != NULL)
        perso->etat.step = 0;
}

void chasser(pnj_t *perso, pnj_t *cible)
{
    if (print_var)
        printf("etat_chasser\n");
    if(perso->espece->alim == 1) printf("%p chasse %p\n",perso,cible);
    if (perso != NULL && cible != NULL)
    {
        float d = quad(perso->pos.x - cible->pos.x) + quad(perso->pos.y - cible->pos.y);
        int inter = quad(perso->pos.w) + quad(perso->pos.h) + quad(cible->pos.w) + quad(cible->pos.h);
        if (d > distance_interaction + inter)
        {
            deplacement_elmentaire(perso, cible->pos.x-perso->pos.x, cible->pos.y-perso->pos.y, perso->caract.vitesse);
        }
        else
        {
            attaquer(perso, cible);
        }
    }
    else if (perso != NULL)
        perso->etat.step = 0;
}

void reproduction(pnj_t *perso, pnj_t *cible)
{
    if (print_var)
        printf("etat_reproduction\n");
    if (perso != NULL && cible != NULL)
    {
        float d = quad(perso->pos.x - cible->pos.x) + quad(perso->pos.y - cible->pos.y);
        int inter = quad(perso->pos.w) + quad(perso->pos.h) + quad(cible->pos.w) + quad(cible->pos.h);
        if (d > distance_interaction + inter)
        {
            deplacement_elmentaire(perso, cible->pos.x-perso->pos.x, cible->pos.y-perso->pos.y, perso->caract.vitesse);
        }
        else
        {
            pnj_t *bebe = breed(perso, cible);
            
            if (bebe){
            
                place_pnj_in_grid(monde_pnj, bebe);
                add_pnj_to_list(pnj_l, bebe);
            }
                
            perso->etat.step = 0;
        }
    }
    else if (perso != NULL)
        perso->etat.step = 0;
}

void fuir(pnj_t *perso, pnj_t *cible)
{
    if (print_var)
        printf("etat_fuir\n");
    if (perso != NULL && cible != NULL)
    {
        float d = quad(perso->pos.x - cible->pos.x) + quad(perso->pos.y - cible->pos.y);
        if (d < perso->caract.vision * 8)
        {
            deplacement_elmentaire(perso, (perso->pos.x - cible->pos.x) * 4, (perso->pos.y - cible->pos.y) * 4, perso->caract.vitesse);
        }
        else
        {
            perso->etat.step = 0;
        }
    }
    else if (perso != NULL)
        perso->etat.step = 0;
}

void repos(pnj_t *perso)
{
    if (print_var)
        printf("etat_fuir\n");
    if (perso != NULL)
    {
        perso->energie = min(100, perso->energie + floor(regen_energie * (vision_base / perso->caract.vision)));
        perso->vie = min(100, perso->vie + floor(regen_vie * (vision_base / perso->caract.vision)));
        perso->etat.step--;
        if (perso->vie == 100 && perso->energie == 100)
            perso->etat.step = 0;
    }
}