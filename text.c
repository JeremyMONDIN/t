#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "structure.h"
#include "liste.h"
#include "Mj.h"
#include "text.h"




maillon_traj_t creer_maillon(int a, float s[14], float r)
{
    maillon_traj_t mail;
    mail.a = a;
    for (int i=0; i<14; i++){
        mail.s[i] = s[i];
    }
    mail.r = r;
    return mail;
}


void save_traj(FILE *file, traj_t traj)
{
    if (!file) return;

    fprintf(file, "%d %.6f {\n", traj.taille, traj.recomp_totale);

    for (int i = 0; i < traj.taille; i++)
    {
        fprintf(file, "([");
        for(int j=0;j<14;j++){
            fprintf(file,"%.6f ",traj.liste[i].s[j]);
        }
        fprintf(file, "] %d %.6f)\n",
                traj.liste[i].a,
                traj.liste[i].r);
    }

    fprintf(file, "}\n");
}

traj_t load_traj(FILE *file)
{
    traj_t traj = init_traj();
    if (!file) return traj;

    fscanf(file, "%d %f {", &traj.taille, &traj.recomp_totale);


    if (traj.taille > traj.capa)
    {
        traj.capa = traj.taille;
        traj.liste = realloc(traj.liste, traj.capa * sizeof(maillon_traj_t));
    }

    for (int i = 0; i < traj.taille; i++)
    {
        fscanf(file, "([");
        for(int j=0;j<14;j++){
            fscanf(file,"%.6f ",&traj.liste[i].s[j]);
        }
        fscanf(file, "] %d %.6f)\n",
                &traj.liste[i].a,
                &traj.liste[i].r);
    }
    return traj;
}


void save_tab(FILE *file, float tab[7][14])
{
    if (!file) return;

    fprintf(file, "[\n");
    for (int i = 0; i < 7; i++)
    {
        fprintf(file, "(");
        for (int j = 0; j < 14; j++)
        {
            fprintf(file, "%.6f", tab[i][j]);
            if (j < 13) fprintf(file, ",");
        }
        fprintf(file, ")\n");
    }
    fprintf(file, "]\n");
}

void load_tab(FILE *file, float tab[7][14])
{
    if (!file) return;

    fscanf(file, " [");

    for (int i = 0; i < 7; i++)
    {
        fscanf(file, " (");
        for (int j = 0; j < 14; j++)
        {
            fscanf(file, "%f", &tab[i][j]);
            if (j < 13) fscanf(file, ",");
        }
        fscanf(file, " )");
    }

    fscanf(file, " ]");
}


void save_espece(FILE *file, espece_t esp)
{
    if (!file) return;

    fprintf(file, "%s\n", esp.nom);
    fprintf(file, "alim:%d\n", esp.alim);
    fprintf(file, "caract:%d %d %d\n",
            esp.caract.force,
            esp.caract.vitesse,
            esp.caract.vision);

    save_tab(file, esp.theta);
}

espece_t load_espece(FILE *file)
{
    caract_t stat = {0};
    char nom[50];
    int alim;

    fscanf(file, " %49[^\n]", nom);
    fgetc(file);
    fscanf(file, "alim:%d\n", &alim);
    fscanf(file, "caract:%d %d %d\n",
           &stat.force,
           &stat.vitesse,
           &stat.vision);

    float tab[7][14];
    load_tab(file, tab);

    return create_espece(nom, alim, NULL, stat, tab);
}


void save_all_traj(FILE *file, pnj_list_t* pnj_l)
{
    if (!file || !pnj_l) return;

    fprintf(file, "%d\n", pnj_l->nb_pnj);

    for (int i = 0; i < pnj_l->nb_pnj; i++)
    {
        save_traj(file, pnj_l->list[i]->trajectoire);
    }
}



void save_all_espece(FILE *file, espece_t *tab_esp, int n)
{
    if (!file) return;

    fprintf(file, "%d\n", n);

    for (int i = 0; i < n; i++)
    {
        save_espece(file, tab_esp[i]);
    }
}

espece_t* load_all_espece(FILE *file, int *n_out)
{
    if (!file) return NULL;

    fscanf(file, "%d", n_out);
    fgetc(file); // ✅ enlève le \n

    espece_t* tab_esp = malloc((*n_out) * sizeof(espece_t));

    for (int i = 0; i < *n_out; i++)
    {
        tab_esp[i] = load_espece(file);
    }

    return tab_esp;
}