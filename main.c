#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <string.h>
#include "Mj.h"
#include "agent.h"
#include "moteur_rendu.h"

char *iamge_background = "backgroundColorFall.png";
char *image_pnj[5] = {"panda.png", "penguin.png", "rabbit.png", "snake.png", NULL};
char *image_decor[4] = {"Carotte.png", "Potato.png", "Steak.png", NULL};

const char *chemin_background = "assets/backgrounds/";
const char *chemin_pnj = "assets/pnj/";
const char *chemin_decor = "assets/decors/";

//"C:\Users\gaill\Documents\projet-zz1\projet global\Jeu SMA\assets\backgrounds\backgroundColorFall.png"
//"C:\Users\gaill\Documents\projet-zz1\projet global\Jeu SMA\assets\pnj\panda.png"

SDL_Texture *texture_death = NULL;

int width = 1000,
    height = 700,
    print_var = 0;

pnj_list_t *pnj_l = NULL;
decor_list_t *decor_l = NULL;

// MJ
pnj_grid_t *monde_pnj;
decor_grid_t *monde_decor;

SDL_Rect taille_monde = {1000, 700, 50, 50};

int duree_action = 10;
float coef_ennemi_proie = 1.5;

int perte_faim = 0;
int degat_faim = 4;

// action

// mort
float coef_nouriture_par_corp = 5;

// deplacement
int deplacemant_par_tic = 10,
    cout_deplacement = 1;

// manger
int gain_faim_plante = 20, // doublé pour la viande
    gain_energie = 10;

// attaque
int degats_attaque = 10,
    coef_bonus_force = 5,
    cout_attaque = 3;

// reproduction
int cout_reproduction = 10;

// repos
int regen_energie = 3,
    regen_vie = 3;

// stat de base
int force_base = 10,
    vitesse_base = 10,
    vision_base = 50,
    distance_interaction = 15;

int main()
{

    srand(time(NULL));

    SDL_Rect rect = {0, 0, 60, 0},
             user = {0, 0, 1000, 700};

    rect.w = width;
    rect.h = height;
    if (rect.x == 1)
        rect.x = 1;

    SDL_Event event;

    int program_on = 1,
        nb_texture_pnj = 0,
        nb_texture_decor = 0;

    int nb_pnj[4] = {10, 0, 20, 50},
        nb_decor[3] = {50, 10, 0};

    caract_t stat_panda = {5, 40, 20};
    caract_t stat_pinguin = {100, 100, 100};
    caract_t stat_lapin = {10, 7, 50};
    caract_t stat_snake = {20, 20, 50};

    pnj_grid_t *check_monde_pnj = monde_pnj;
    decor_grid_t *check_monde_decor = monde_decor;

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture *background = NULL;
    SDL_Texture **texture_pnj = malloc(10 * sizeof(SDL_Texture *));
    SDL_Texture **texture_decor = malloc(10 * sizeof(SDL_Texture *));

    pnj_l = init_pnj_list();
    decor_l = init_decor_list();

    // définition des fenêtre et des textures d'affichage
    printf("init texture\n");

    window = SDL_CreateWindow("Grille", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                              // SDL_WINDOW_FULLSCREEN );
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == 0)
    {
        fprintf(stderr, "Erreur a la creation de la fenetre : %s\n", SDL_GetError());
        SDL_Delay(5000);
    }
    else
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); /* | SDL_RENDERER_PRESENTVSYNC */
        if (renderer == 0)
        {
            fprintf(stderr, "Erreur de creation d'un renderer : %s\n", SDL_GetError());
        }
    }
    SDL_Surface *my_image = NULL;

    char *nom = malloc(100 * sizeof(char));
    sprintf(nom, "%s%s", chemin_background, iamge_background);
    my_image = IMG_Load(nom);
    if (!my_image)
        printf("IMG_Load: %s\n", IMG_GetError());
    background = SDL_CreateTextureFromSurface(renderer, my_image);
    if (!background)
        printf("SDL_CreateTextureFromSurface1: %s\n", IMG_GetError());
    SDL_FreeSurface(my_image);

    my_image = IMG_Load("assets/decors/Steak.png");
    if (!my_image)
        printf("IMG_Load: %s\n", IMG_GetError());
    texture_death = SDL_CreateTextureFromSurface(renderer, my_image);
    if (!texture_death)
        printf("SDL_CreateTextureFromSurface1: %s\n", IMG_GetError());
    SDL_FreeSurface(my_image);

    while (image_pnj[nb_texture_pnj] != NULL)
    {
        sprintf(nom, "%s%s", chemin_pnj, image_pnj[nb_texture_pnj]);
        my_image = IMG_Load(nom);
        if (!my_image)
            printf("IMG_Load: %s\n", IMG_GetError());
        texture_pnj[nb_texture_pnj] = SDL_CreateTextureFromSurface(renderer, my_image);
        if (!texture_pnj[nb_texture_pnj])
            printf("SDL_CreateTextureFromSurface1: %s\n", IMG_GetError());
        SDL_FreeSurface(my_image);
        nb_texture_pnj++;
        free_monde();
    }
    while (image_decor[nb_texture_decor])
    {
        sprintf(nom, "%s%s", chemin_decor, image_decor[nb_texture_decor]);
        my_image = IMG_Load(nom);
        if (!my_image)
            printf("IMG_Load: %s\n", IMG_GetError());
        texture_decor[nb_texture_decor] = SDL_CreateTextureFromSurface(renderer, my_image);
        if (!texture_decor[nb_texture_decor])
            printf("SDL_CreateTextureFromSurface1: %s\n", IMG_GetError());
        SDL_FreeSurface(my_image);

        nb_texture_decor++;
    }
    free(nom);

    // initialisation du monde
    printf("init monde\n");
    init_monde();
    SDL_Rect pos = {0, 0, 30, 30};

    espece_t tab_espece[4] = {
        create_espece("panda", 0, texture_pnj[0], stat_panda),
        create_espece("pinguin", 2, texture_pnj[1], stat_pinguin),
        create_espece("lapin", 0, texture_pnj[2], stat_lapin),
        create_espece("snake", 1, texture_pnj[3], stat_snake)};

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < nb_pnj[i]; j++)
        {
            pos.x = rand() % width;
            pos.y = rand() % height;
            add_pnj(&tab_espece[i], pos);
        }
    }

    // patate
    for (int j = 0; j < nb_decor[1]; j++)
    {
        pos.x = rand() % width;
        pos.y = rand() % height;
        add_decor(1, texture_decor[1], 1, 5, pos);
    }

    // steak
    for (int j = 0; j < nb_decor[2]; j++)
    {
        pos.x = rand() % width;
        pos.y = rand() % height;
        add_decor(2, texture_decor[2], 2, 5, pos);
    }

    printf("debut_boucle\n");
    int i = 0, compte = 0;
    compte = 0;
    for (int j = 0; j < monde_pnj->nb_case_x; j++)
    {
        for (int k = 0; k < monde_pnj->nb_case_y; k++)
        {
            compte += monde_pnj->grid[j][k]->nb_pnj;
        }
    }
    printf("boucle %d ***************************** %d\n", i++, compte);
    while (program_on)
    {
        // if(SDL_PollEvent(&event)){

        // user=user_ask(event);

        etape_suivante(pnj_l, decor_l);
        affichage(window, renderer, background, user);
        compte = 0;
        for (int j = 0; j < monde_pnj->nb_case_x; j++)
        {
            for (int k = 0; k < monde_pnj->nb_case_y; k++)
            {
                compte += monde_pnj->grid[j][k]->nb_pnj;
            }
        }
        printf("boucle %d ***************************** %d\n", i++, compte);
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
        if (SDL_PollEvent(&event))
        {

            switch (event.type)
            {
            case SDL_QUIT:
                program_on = SDL_FALSE;
                break;
            }
        }

        if (i % 10 == 0)
        {
            pos.x = rand() % width;
            pos.y = rand() % height;
            add_decor(1, texture_decor[1], 1, 5, pos);
        }

        // mettre un compteur de copy dans etat ou direect pnj pour savoir quand supprimé

        /*if (i % 100 == 0) // libere les entité mort depus longtemps
        {
            int pnj_ini = pnj_l->nb_pnj;
            int j = 0;

            for (int i = 0; i < pnj_ini; i++)
            {
                if (pnj_l->list[i]->etat.id == 0 &&
                    pnj_l->list[i]->etat.step > 75)
                {
                    free(pnj_l->list[i]);
                }
                else
                {
                    pnj_l->list[j++] = pnj_l->list[i];
                }
            }

            pnj_l->nb_pnj = j;
            int dec_ini = decor_l->nb_decor;
            j = 0;

            for (int i = 0; i < dec_ini; i++)
            {
                if (decor_l->list[i]->etat.id == 0 &&
                    decor_l->list[i]->etat.step > 75)
                {
                    free(decor_l->list[i]);
                }
                else
                {
                    decor_l->list[j++] = decor_l->list[i];
                }
            }

            decor_l->nb_decor = j;
        }*/
    }
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    for (int i = 0; i < nb_texture_pnj; i++)
    {
        SDL_DestroyTexture(texture_pnj[i]);
    }

    for (int j = 0; j < nb_texture_decor; j++)
    {
        SDL_DestroyTexture(texture_decor[j]);
    }

    free_list_decor(decor_l);
    free_list_pnj(pnj_l);
    free(texture_pnj);
    free(texture_decor);

    return 0;
}
