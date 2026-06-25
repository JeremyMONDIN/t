#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "action.h"
#include "grid.h"
#include "structure.h"
#include "Mj.h"
#include "rl.h"

#define quad(a) (a * a)

int deplacement_elmentaire(pnj_t *perso,
                           int x, int y,
                           int vitesse, int vitesse_base,
                           SDL_Rect taille_monde)
{

    int deplacemant_par_tic = 10,
        cout_deplacement = -1;

    int n = 0;
    float d = sqrt(x * x + y * y);
    if (perso->lock_anim == 0)
    {
        if (perso->energie > cout_deplacement + quad(vitesse / vitesse_base))
        {
            perso->energie -= cout_deplacement + quad(vitesse / vitesse_base);
            if (d > deplacemant_par_tic)
            {
                x = floor(x / d * deplacemant_par_tic);
                y = floor(y / d * deplacemant_par_tic);
                n = 1;
            }
            perso->pos.x = wrap(perso->pos.x + x, taille_monde.x);
            perso->pos.y = wrap(perso->pos.y + y, taille_monde.y);
            perso->etat.step--;
            if (n == 0)
                perso->etat.step = 0;
        }
        else
            perso->etat.step = 0;
    }
    return n;
}

void attaquer(pnj_t *attaquant, pnj_t *cible, int affichage_graphique)
{

    int degats_attaque = 50,
        coef_bonus_force = 5,
        cout_attaque = 3;

    if (attaquant->lock_anim == 0 && cible->lock_anim == 0)
    {
        if (attaquant->energie > cout_attaque && cible->etat.id != 0)
        {
            float factor = (float)attaquant->caract.force / cible->caract.force;
            attaquant->energie -= cout_attaque;
            cible->vie -= floor(degats_attaque + coef_bonus_force * factor);
            if (cible->vie <= 0)
            {
                cible->etat.id = 0;
                cible->etat.step = 5; // arbitraire pour eviter un nouveau tirage avant la mort
                printf("%s kill %s\n", attaquant->espece->nom, cible->espece->nom);
            }
            attaquant->etat.step--;
            if (affichage_graphique)
            {
                attaquant->lock_anim = 20;
                cible->lock_anim = -1;
            }
        }
        else
            attaquant->etat.step = 0;
    }
}

pnj_t *breed(pnj_t *mere, pnj_t *pere, int affichage_graphique)
{
    int evolution = 0;
    uint32_t seed = 42; // temp
    int cout_reproduction = 10;

    if (mere->lock_anim == 0 && pere->lock_anim == 0)
    {
        if (mere->energie > cout_reproduction && pere->energie > cout_reproduction && mere->lock_repro == 0 && pere->lock_repro == 0)
        {
            caract_t stat={0};
            stat.vitesse = (mere->caract.vitesse + pere->caract.vitesse) / 2;
            if (evolution)
                stat.vitesse += nb_aleatoire(seed) % 5 - 2;
            stat.force = (mere->caract.force + pere->caract.force) / 2;
            if (evolution)
                stat.force += nb_aleatoire(seed) % 5 - 2;
            stat.vision = (mere->caract.vision + pere->caract.vision) / 2;
            if (evolution)
                stat.vision += nb_aleatoire(seed) % 5 - 2;
            SDL_Rect pos = {(mere->pos.x + pere->pos.x) / 2,
                            (mere->pos.y + pere->pos.y) / 2, 30,30};
            pnj_t * enfant=create_pnj(mere->espece, 0, stat, pos, 50,50);
            mere->energie -= cout_reproduction;
            pere->energie -= cout_reproduction;
            mere->etat.step = 0;
            pere->etat.step = 0;
            pere->lock_repro = 5;
            mere->lock_repro = 5;
            enfant->lock_repro = 10;
            if (affichage_graphique)
            {
                mere->lock_anim = 20;
                pere->lock_anim = -1;
                enfant->lock_anim = 0;
            }

            return enfant;
        }
        mere->etat.step = 0;
        pere->etat.step = 0;
    }
    return NULL;
}

//**************ETATS******************************

void mourir(pnj_t *perso, SDL_Texture *texture_death, decor_grid_t *monde_decor, decor_list_t *decor_l, SDL_Rect taille_monde, pnj_grid_t *monde_pnj)
{
    float coef_nouriture_par_corp = 5;

    add_decor(2, texture_death, 2, coef_nouriture_par_corp, (perso)->pos, monde_decor, decor_l, taille_monde);
    sup_pnj_from_grid(monde_pnj, perso, taille_monde);
}

void explorer(pnj_t *perso,
              int cor_x, int cor_y,
              int vitesse_base,
              SDL_Rect taille_monde)
{
    int x = cor_x - perso->pos.x;
    int y = cor_y - perso->pos.y;
    deplacement_elmentaire(perso, x, y, min(perso->caract.vitesse, vitesse_base), vitesse_base, taille_monde);
}

void manger(pnj_t *perso,
            decor_t *cible,
            int vitesse_base,
            SDL_Rect taille_monde)
{
    int distance_interaction = 30;
    int gain_faim_plante = 20, // doublé pour la viande
        gain_energie = 10;

    if (perso != NULL && cible != NULL)
    {
        if (perso->lock_anim == 0)
        {
            float d = quad(perso->pos.x - cible->pos.x) + quad(perso->pos.y - cible->pos.y);
            int inter = quad(perso->pos.w) + quad(perso->pos.h) + quad(cible->pos.w) + quad(cible->pos.h);
            if (d > distance_interaction + inter)
            {
                deplacement_elmentaire(perso, cible->pos.x - perso->pos.x, cible->pos.y - perso->pos.y, min(perso->caract.vitesse, vitesse_base), vitesse_base, taille_monde);
            }
            else
            {
                if (cible->etat.id != 0)
                {
                    perso->faim = min(100, perso->faim + gain_faim_plante * cible->type_decor);
                    perso->energie = min(100, perso->energie + gain_energie);
                    perso->etat.step--;
                    perso->lock_anim = 10;
                    cible->etat.step--;
                    if (cible->etat.step <= 0)
                        cible->etat.id = 0;
                }
            }
        }
    }
    else if (perso != NULL)
        perso->etat.step = 0;
}

void defendre(pnj_t *perso,
              pnj_t *cible,
              int vitesse_base,
              SDL_Rect taille_monde,
              int affichage_graphique)
{
    int distance_interaction = 30;

    if (perso != NULL && cible != NULL)
    {
        float d = quad(perso->pos.x - cible->pos.x) + quad(perso->pos.y - cible->pos.y);
        int inter = quad(perso->pos.w) + quad(perso->pos.h) + quad(cible->pos.w) + quad(cible->pos.h);
        if (d > distance_interaction + inter)
        {
            deplacement_elmentaire(perso, cible->pos.x - perso->pos.x, cible->pos.y - perso->pos.y, min(perso->caract.vitesse, vitesse_base), vitesse_base, taille_monde);
        }
        else
        {
            attaquer(perso, cible, affichage_graphique);
        }
    }
    else if (perso != NULL)
        perso->etat.step = 0;
}

void chasser(pnj_t *perso,
             pnj_t *cible,
             int vitesse_base,
             SDL_Rect taille_monde,
             int affichage_graphique)
{
    int distance_interaction = 30;

    if (perso != NULL && cible != NULL)
    {
        float d = quad(perso->pos.x - cible->pos.x) + quad(perso->pos.y - cible->pos.y);
        int inter = quad(perso->pos.w) + quad(perso->pos.h) + quad(cible->pos.w) + quad(cible->pos.h);
        if (d > distance_interaction + inter)
        {
            deplacement_elmentaire(perso, cible->pos.x - perso->pos.x, cible->pos.y - perso->pos.y, perso->caract.vitesse, vitesse_base, taille_monde);
        }
        else
        {
            attaquer(perso, cible, affichage_graphique);
        }
    }
    else if (perso != NULL)
        perso->etat.step = 0;
}

void reproduction(pnj_t *perso,
                  pnj_t *cible,
                  pnj_grid_t *monde_pnj,
                  pnj_list_t *pnj_l,
                  int vitesse_base,
                  SDL_Rect taille_monde,
                  int affichage_graphique)
{
    int distance_interaction = 30;

    if (perso != NULL && cible != NULL)
    {
        if (perso->lock_anim == 0 && cible->lock_anim == 0)
        {
            float d = quad(perso->pos.x - cible->pos.x) + quad(perso->pos.y - cible->pos.y);
            int inter = quad(perso->pos.w) + quad(perso->pos.h) + quad(cible->pos.w) + quad(cible->pos.h);
            if (d > distance_interaction + inter)
            {
                deplacement_elmentaire(perso, cible->pos.x - perso->pos.x, cible->pos.y - perso->pos.y, perso->caract.vitesse, vitesse_base, taille_monde);
            }
            else
            {
                pnj_t *bebe = breed(perso, cible, affichage_graphique);

                if (bebe)
                {

                    place_pnj_in_grid(monde_pnj, bebe, taille_monde);
                    add_pnj_to_list(pnj_l, bebe);
                }
            }
        }
    }
    else if (perso != NULL)
        perso->etat.step = 0;
}

void fuir(pnj_t *perso,
          pnj_t *cible,
          int vitesse_base,
          SDL_Rect taille_monde)
{

    if (perso != NULL && cible != NULL)
    {
        if (perso->lock_anim == 0 && cible->lock_anim == 0)
        {
            float d = quad(perso->pos.x - cible->pos.x) + quad(perso->pos.y - cible->pos.y);
            if (d < perso->caract.vision * 8)
            {
                deplacement_elmentaire(perso, (perso->pos.x - cible->pos.x) * 4, (perso->pos.y - cible->pos.y) * 4, perso->caract.vitesse, vitesse_base, taille_monde);
            }
            else
            {
                perso->etat.step = 0;
            }
        }
    }
    else if (perso != NULL)
        perso->etat.step = 0;
}

void repos(pnj_t *perso, int vision_base, int affichage_graphique)
{
    // repos
    int regen_energie = 3,
        regen_vie = 3;
    if (perso != NULL)
    {
        if (perso->lock_anim == 0)
        {
            perso->energie = min(100, perso->energie + floor(regen_energie * (vision_base / perso->caract.vision)));
            perso->vie = min(100, perso->vie + floor(regen_vie * (vision_base / perso->caract.vision)));
            perso->etat.step--;
            if (affichage_graphique)
                perso->lock_anim = 10;
            if (perso->vie == 100 && perso->energie == 100)
                perso->etat.step = 0;
        }
    }
}