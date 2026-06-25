#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "agent.h"
#include "action.h"
#include "grid.h"
#include "liste.h"
#include "moteur_rendu.h"
#include "Mj.h"
#include "structure.h"

void init_monde(pnj_grid_t **monde_pnj,
                decor_grid_t **monde_decor,
                SDL_Rect taille_monde)
{

    *monde_decor = creat_grid_decor(taille_monde.x, taille_monde.y, taille_monde.w, taille_monde.h);
    *monde_pnj = creat_grid_pnj(taille_monde.x, taille_monde.y, taille_monde.w, taille_monde.h);
}

void free_monde(pnj_grid_t *monde_pnj,
                decor_grid_t *monde_decor)
{
    free_decor_grid(monde_decor);
    free_pnj_grid(monde_pnj);
}

//**************Mj****************/

espece_t create_espece(char nom[50], int alim, SDL_Texture *visu, caract_t stat, float tab[7][14])
{

    espece_t new;

    strncpy(new.nom, nom, 49);
    new.nom[49] = '\0';

    new.alim = alim;
    new.visuel = visu;
    new.caract = stat;
    memcpy(new.theta, tab, sizeof(new.theta));

    return new;
}

void add_pnj(espece_t *espece,
             SDL_Rect pos,
             pnj_grid_t *monde_pnj,
             pnj_list_t *pnj_l,
             SDL_Rect taille_monde)
{
    pnj_t *new = create_pnj(espece, 100, espece->caract, pos, 100, 100);
    place_pnj_in_grid(monde_pnj, new, taille_monde);
    add_pnj_to_list(pnj_l, new);
}

void add_decor(int id,
               SDL_Texture *visuel,
               int id_etat,
               int rang,
               SDL_Rect pos,
               decor_grid_t *monde_decor,
               decor_list_t *decor_l,
               SDL_Rect taille_monde)
{

    decor_t *new = create_decor(id, visuel, id_etat, rang, pos);
    place_decor_in_grid(monde_decor, new, taille_monde);
    add_decor_to_list(decor_l, new);
}

int check_food(int a, int b)
{
    int n = 0;
    if (a == 0 && b == 1)
        n = 1;
    if (a == 1 && b == 2)
        n = 1;
    if (a == 2 && (b == 1 || b == 2))
        n = 1;
    return n;
}

perception_t donne_perception(pnj_grid_t *monde_pnj, decor_grid_t *monde_decor, pnj_t *perso)
{
    float coef_ennemi_proie = 1.5;

    int radius = perso->caract.vision * 8,
        x = perso->pos.x,
        y = perso->pos.y,
        case_x = wrap((x - radius), monde_pnj->taille_w * monde_pnj->nb_case_x) / monde_pnj->nb_case_x,
        case_y = wrap((y - radius), monde_pnj->taille_h * monde_pnj->nb_case_y) / monde_pnj->nb_case_y,
        n_x = (radius * 2) / monde_pnj->taille_w,
        n_y = (radius * 2) / monde_pnj->taille_h,
        d = 0,
        n_min = radius,
        a_min = radius,
        e_min = radius,
        p_min = radius;
    perception_t per = {perso, 0, NULL, 0, NULL, 0, NULL, 0, NULL};
    pnj_list_t *l_pnj = NULL;
    decor_list_t *l_decor = NULL;

    for (int i = case_x; i < case_x + n_x; i++)
    {
        for (int j = case_y; j < case_y + n_y; j++)
        {
            l_decor = monde_decor->grid[i % monde_decor->nb_case_x][j % monde_decor->nb_case_y];
            if (l_decor->list)
            {
                for (int k = 0; k < l_decor->nb_decor; k++)
                {
                    if (check_food(perso->espece->alim, l_decor->list[k]->type_decor))
                    {
                        d = dist(perso->pos, l_decor->list[k]->pos);
                        if (d <= radius)
                        {
                            per.nb_nouriture++;
                            if (d <= n_min)
                            {
                                per.nouriture = l_decor->list[k];
                                n_min = d;
                            }
                        }
                    }
                }
            }
            l_pnj = monde_pnj->grid[i % monde_pnj->nb_case_x][j % monde_pnj->nb_case_y];
            if (l_pnj->list)
            {
                for (int k = 0; k < l_pnj->nb_pnj; k++)
                {

                    d = dist(perso->pos, l_pnj->list[k]->pos);
                    if (d <= radius)
                    {
                        if (perso != l_pnj->list[k])
                        {
                            if (perso->espece == l_pnj->list[k]->espece)
                            {
                                per.nb_allie++;
                                if (d <= a_min)
                                {
                                    per.allie = l_pnj->list[k];
                                    a_min = d;
                                }
                            }
                            else
                            {
                                if (l_pnj->list[k]->espece->alim > 0 && l_pnj->list[k]->caract.force > perso->caract.force / coef_ennemi_proie)
                                {
                                    per.nb_ennemi++;
                                    if (d <= e_min)
                                    {
                                        per.ennemi = l_pnj->list[k];
                                        e_min = d;
                                    }
                                }
                                else
                                {
                                    per.nb_proie++;
                                    if (d <= p_min)
                                    {
                                        per.proie = l_pnj->list[k];
                                        p_min = d;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return per;
}

reponse_t get_action(pnj_t *perso,
                     pnj_grid_t *monde_pnj,
                     decor_grid_t *monde_decor)
{
    return reaction(donne_perception(monde_pnj, monde_decor, perso));
}

void agir(pnj_t *perso,
          pnj_grid_t *monde_pnj,
          pnj_list_t *pnj_l,
          decor_grid_t *monde_decor,
          decor_list_t *decor_l,
          SDL_Rect taille_monde,
          SDL_Texture *texture_death,
          int vitesse_base,
          int vision_base,
          int affichage_graphique)
{
    switch (perso->etat.id)
    {
    case 0: // mourir
        if (perso->etat.step == 5)
        {
            mourir(perso, texture_death, monde_decor, decor_l,taille_monde, monde_pnj);
            perso->etat.step = 12;
        }
        break;
    case 1: // explorer
        explorer(perso, perso->etat.x, perso->etat.y, vitesse_base, taille_monde);
        break;
    case 2: // manger
        manger(perso, (decor_t *)perso->etat.para, vitesse_base, taille_monde);
        break;
    case 3: // defendre
        defendre(perso, (pnj_t *)perso->etat.para, vitesse_base, taille_monde, affichage_graphique);
        break;
    case 4: // chasser

        chasser(perso, (pnj_t *)perso->etat.para, vitesse_base, taille_monde, affichage_graphique);
        break;
    case 5: // reproduction
        reproduction(perso, (pnj_t *)perso->etat.para, monde_pnj, pnj_l, vitesse_base, taille_monde, affichage_graphique);
        break;
    case 6: // fuir
        fuir(perso, (pnj_t *)perso->etat.para, vitesse_base, taille_monde);
        break;
    case 7: // repos
        repos(perso, vision_base, affichage_graphique);
        break;
    default:
        printf("???\n");
        perso->etat.step = 0;
        break;
    }
}

void etape_suivante(pnj_list_t *pnj_l,
                    decor_list_t *decor_l,
                    int faim,
                    pnj_grid_t *monde_pnj,
                    decor_grid_t *monde_decor,
                    SDL_Rect taille_monde,
                    SDL_Texture *texture_death,
                    int affichage_graphique,
                    int print_texte,
                    FILE *log)
{
    int duree_action = 10;
    int perte_faim = 5;
    int degat_faim = 15;

    int //force_base = 10,
        vitesse_base = 10,
        vision_base = 50;

    int pnj_ini = pnj_l->nb_pnj;
    for (int i = 0; i < pnj_ini; i++)
    {
        if (pnj_l->list[i]->etat.step <= 0 && pnj_l->list[i]->lock_anim == 0)
        {
            reponse_t rep = get_action(pnj_l->list[i], monde_pnj, monde_decor);
            pnj_t *p = pnj_l->list[i];

            // état simple
            float s = pnj_l->list[i]->etat.id;

            float recomp = 0;

            if (p->espece->alim==0){
                switch (p->etat.id)
                {
                case 1:
                    recomp=20;
                    break;

                case 2:
                    recomp=10;
                    break;

                case 3:
                    recomp=0;
                    break;
                
                case 4:
                    recomp=-10;
                    break;
                
                case 5:
                    recomp=20;
                    break;

                case 6:
                    recomp=15;
                    break;
                
                case 7:
                    recomp=0;
                    break;

                default:
                    break;
                }
            }

            if (p->espece->alim==1){
                switch (p->etat.id)
                {
                case 1:
                    recomp=15;
                    break;

                case 2:
                    recomp=10;
                    break;

                case 3:
                    recomp=10;
                    break;
                
                case 4:
                    recomp=20;
                    break;
                
                case 5:
                    recomp=20;
                    break;

                case 6:
                    recomp=0;
                    break;
                
                case 7:
                    recomp=0;
                    break;

                default:
                    break;
                }
            }

            if (p->espece->alim==2){
                switch (p->etat.id)
                {
                case 1:
                    recomp=15;
                    break;

                case 2:
                    recomp=10;
                    break;

                case 3:
                    recomp=0;
                    break;
                
                case 4:
                    recomp=0;
                    break;
                
                case 5:
                    recomp=20;
                    break;

                case 6:
                    recomp=5;
                    break;
                
                case 7:
                    recomp=0;
                    break;

                default:
                    break;
                }
            }

            // ajout trajectoire

            if (p->trajectoire.taille >= p->trajectoire.capa) {
                p->trajectoire.capa *= 2;
                p->trajectoire.liste = realloc(
                                                p->trajectoire.liste,
                                                p->trajectoire.capa * sizeof(maillon_traj_t)
                                                );
            }       

            int t = p->trajectoire.taille;

            p->trajectoire.liste[t].s = s;
            p->trajectoire.liste[t].a = rep.act;
            p->trajectoire.liste[t].r = recomp;

            p->trajectoire.taille++;


            pnj_l->list[i]->etat.id = rep.act;
            pnj_l->list[i]->etat.para = rep.depla;
            pnj_l->list[i]->etat.x = rep.x;
            pnj_l->list[i]->etat.y = rep.y;
            pnj_l->list[i]->etat.step = duree_action;
        }
        fprintf(log,"%p %s  vie:%d energie:%d faim:%d x=%d  y=%d  anim=%d repro=%d etat: id=%d step=%d x=%d y=%d %p\n",
                   pnj_l->list[i], pnj_l->list[i]->espece->nom, 
                   pnj_l->list[i]->vie, pnj_l->list[i]->energie, pnj_l->list[i]->faim, 
                   pnj_l->list[i]->pos.x, pnj_l->list[i]->pos.y, pnj_l->list[i]->lock_anim,pnj_l->list[i]->lock_repro,
                   pnj_l->list[i]->etat.id, pnj_l->list[i]->etat.step, pnj_l->list[i]->etat.x, pnj_l->list[i]->etat.y, pnj_l->list[i]->etat.para
                   );
        
        if (print_texte && pnj_l->list[i]->etat.id != 0)
            printf("%s  stat %d %d %d etat %d %d  x=%d  y=%d  point=%d\n",
                   pnj_l->list[i]->espece->nom, pnj_l->list[i]->vie, pnj_l->list[i]->energie, pnj_l->list[i]->faim, pnj_l->list[i]->etat.id, pnj_l->list[i]->etat.step, pnj_l->list[i]->pos.x, pnj_l->list[i]->pos.y, pnj_l->list[i]->lock_anim);
        sup_pnj_from_grid(monde_pnj, pnj_l->list[i],taille_monde);

        agir(pnj_l->list[i],monde_pnj,pnj_l,monde_decor,decor_l,taille_monde,texture_death,vitesse_base,vision_base,affichage_graphique);

        if (pnj_l->list[i]->etat.id != 0)
            place_pnj_in_grid(monde_pnj, pnj_l->list[i],taille_monde);

        if (faim == 0)
        {
            pnj_l->list[i]->lock_repro = max(0, pnj_l->list[i]->lock_repro - 1);
            pnj_l->list[i]->faim = max(0, pnj_l->list[i]->faim - perte_faim /**(1 + pnj_l->list[i]->caract.force / force_base)*/);
            if (pnj_l->list[i]->faim == 0)
                pnj_l->list[i]->vie -= degat_faim;
        }
        if (pnj_l->list[i]->vie <= 0 && pnj_l->list[i]->etat.id != 0)
        {
            pnj_l->list[i]->etat.id = 0;
            pnj_l->list[i]->etat.step = 5; // arbitraire pour eviter un nouveau tirage avant la mort
        }
    }
    int dec_ini = decor_l->nb_decor;
    for (int j = 0; j < dec_ini; j++)
    {
        if (decor_l->list[j]->etat.id == 0)
        {
            kill_decor(monde_decor, &decor_l->list[j], taille_monde);
            decor_l->list[j]->etat.step++;
        }
        else if (print_texte)
            printf("id=%d etat id:%d step:%d  x=%d  y=%d\n",
                   decor_l->list[j]->type_decor, decor_l->list[j]->etat.id, decor_l->list[j]->etat.step, decor_l->list[j]->pos.x, decor_l->list[j]->pos.y);
        fprintf(log,"%p id=%d etat id:%d step:%d  x=%d  y=%d\n",
                   decor_l->list[j], decor_l->list[j]->type_decor, 
                   decor_l->list[j]->etat.id, decor_l->list[j]->etat.step, 
                   decor_l->list[j]->pos.x, decor_l->list[j]->pos.y);
    }
}

void affichage(SDL_Window *window,
               SDL_Renderer *renderer,
               SDL_Texture *background,
               SDL_Rect user,
               pnj_grid_t *monde_pnj,
               decor_grid_t *monde_decor,
               SDL_Rect taille_monde,
                int temp_on)
{

    SDL_Rect rect = {wrap(user.x, taille_monde.w), wrap(user.y, taille_monde.y), 100, 100};
    SDL_GetWindowSize(window, &rect.w, &rect.h);
    pnj_list_t *l_pnj = get_pnj_list_in(monde_pnj, rect,taille_monde);
    decor_list_t *l_decor = get_decor_list_in(monde_decor, rect,taille_monde);
    affiche_fond2(window, renderer, background, user);

    for (int i = 0; i < l_decor->nb_decor; i++)
    {
        afficher_decor(window, renderer, l_decor->list[i], user,taille_monde);
    }
    for (int i = 0; i < l_pnj->nb_pnj; i++)
    {
        afficher_PNJ(window, renderer, l_pnj->list[i], user,taille_monde,temp_on);
    }

    free_list_pnj(l_pnj);
    free_list_decor(l_decor);
}
