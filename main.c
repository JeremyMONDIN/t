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
#include "reinforce.h"

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
    height = 700;

pnj_list_t *pnj_l = NULL;
decor_list_t *decor_l = NULL;

// MJ
pnj_grid_t *monde_pnj;
decor_grid_t *monde_decor;

SDL_Rect taille_monde = {1000, 700, 50, 50};

int duree_action = 10;
float coef_ennemi_proie = 1.5;


int perte_faim = 5;
int degat_faim = 15;


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
int degats_attaque = 300,
    coef_bonus_force = 20,
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
    distance_interaction = 30;

// variable de menu
int print_var = 0;
int print_texte = 0;
int affichage_graphique = 1;

//parametre global
int evolution = 0;
int perception_discrete = 1;


char buffer[3];

int main()
{
    int loop = 1;
    while (loop)
    {
        printf("Choisir un mode :\n");
        printf("1 - Graphique on/off \n");
        printf("2 - Textuel on/off \n");
        printf("3 - fonction traceback on/off \n");
        printf("4 - lancer \n");
        printf("5 - quitter \n");

        printf("\n\nGraphique ");
        if (affichage_graphique)
            printf("on\n");
        else
            printf("off\n");
        printf("Textuel ");
        if (print_texte)
            printf("on\n");
        else
            printf("off\n");
        printf("fonction traceback ");
        if (print_var)
            printf("on\n\n");
        else
            printf("off\n\n");

        fgets(buffer, sizeof(buffer), stdin);
        switch (buffer[0])
        {
        case '1':
            affichage_graphique = !affichage_graphique;
            break;
        case '2':
            print_texte = !print_texte;
            break;
        case '3':
            print_var = !print_var;
            break;
        case '4':
            loop = 0;
            break;
        case '5':
            return 0;
            break;
        default:
            printf("erreur commande invalide\n");
        }
    }

    //srand(time(NULL));

    SDL_Rect rect = {0, 0, 60, 0},
             user = {0, 0, 1000, 700};

    rect.w = width;
    rect.h = height;

    int program_on = 1,
        nb_texture_pnj = 0,
        nb_texture_decor = 0;

    int nb_pnj[4] = {30, 0, 30, 30},
        nb_decor[3] = {50, 10, 0};
    // Vitesse, Force, Vision
    caract_t stat_panda = {5, 40, 20};
    caract_t stat_pinguin = {100, 100, 100};
    caract_t stat_lapin = {15, 1, 50};
    caract_t stat_snake = {20, 20, 50};

    pnj_grid_t *check_monde_pnj = monde_pnj;
    decor_grid_t *check_monde_decor = monde_decor;

    SDL_Texture **texture_pnj = malloc(10 * sizeof(SDL_Texture *));
    SDL_Texture **texture_decor = malloc(10 * sizeof(SDL_Texture *));

    SDL_Event event;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Texture *background = NULL;

    pnj_l = init_pnj_list();
    decor_l = init_decor_list();

    if (affichage_graphique)
    {
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
    }

    // initialisation du monde
    printf("init monde\n");
    init_monde();
    SDL_Rect pos = {0, 0, 30, 30};

    espece_t tab_espece[4] = {
        create_espece("panda", 2, texture_pnj[0], stat_panda),
        create_espece("pinguin", 2, texture_pnj[1], stat_pinguin),
        create_espece("lapin", 0, texture_pnj[2], stat_lapin),
        create_espece("snake", 1, texture_pnj[3], stat_snake)};

    for (int i = 0; i < 4; i++) {
        tab_espece[i].theta = calloc(7 * 5, sizeof(float));
    }

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

    int i = 0, compte = 0;

    
    if (print_texte)
    {
        
        compte = 0;
        for (int j = 0; j < monde_pnj->nb_case_x; j++)
        {
            for (int k = 0; k < monde_pnj->nb_case_y; k++)
            {
                compte += monde_pnj->grid[j][k]->nb_pnj;
            }
        }
        printf("boucle %d ***************************** %d\n", i++, compte);
    }

    printf("debut_boucle\n");

    while (program_on)
    {
        // if(SDL_PollEvent(&event)){

        // user=user_ask(event);

        etape_suivante(pnj_l, decor_l, i % 30);
        i++;

        if (print_texte)
        {
            compte = 0;
            for (int j = 0; j < monde_pnj->nb_case_x; j++)
            {
                for (int k = 0; k < monde_pnj->nb_case_y; k++)
                {
                    compte += monde_pnj->grid[j][k]->nb_pnj;
                }
            }
            printf("boucle %d ***************************** %d\n", i++, compte);
        }

                // main.c - Entraîner chaque espèce séparément (chaque 200 pas)
        if (i % 200 == 0) {
            // Entraîner CHAQUE espèce avec SES PROPRES trajectoires
            for (int e = 0; e < 3; e++) {
                // Collecter les trajectoires de cette espèce uniquement
                int N_espece = 0;
                for (int j = 0; j < pnj_l->nb_pnj; j++) {
                    if (pnj_l->list[j]->espece->alim == e) N_espece++;
                }
                
                if (N_espece > 0) {
                    traj_t **traj_espece = malloc(N_espece * sizeof(traj_t*));
                    int *tailles = malloc(N_espece * sizeof(int));
                    
                    int k = 0;
                    for (int j = 0; j < pnj_l->nb_pnj; j++) {
                        if (pnj_l->list[j]->espece->alim == e) {
                            traj_espece[k] = &pnj_l->list[j]->trajectoire;
                            tailles[k] = pnj_l->list[j]->trajectoire.taille;
                            k++;
                        }
                    }
                    
                    // Entraîner avec theta de CETTE espèce
                    algo_REINFORCE(0.99, N_espece, 0.01, 0.2, 5, 7,
                                traj_espece, tab_espece[e].theta, tailles);
                    
                    free(traj_espece);
                    free(tailles);
                }
            }
        }

        if (affichage_graphique)
        {
            affichage(window, renderer, background, user);
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
        }

        if (i % 1000 == 0)
        {
            compte = 0;
            for (int j = 0; j < monde_decor->nb_case_x; j++)
            {
                for (int k = 0; k < monde_decor->nb_case_y; k++)
                {
                    compte += monde_decor->grid[j][k]->nb_decor;
                }
            }
            if (compte<100){
                pos.x = rand() % width;
                pos.y = rand() % height;
                add_decor(1, texture_decor[1], 1, 5, pos);
            }
            
        }

        // mettre un compteur de copy dans etat ou direect pnj pour savoir quand supprimé

        /*if (i % 100 == 0) // libere les entité mort depus longtemps
        {
            int pnj_ini = pnj_l->nb_pnj;liste = malloc(1000 * sizeof(maillon_traj_t*));
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


    for (int i = 0; i < pnj_l->nb_pnj; i++) {
        free(pnj_l->list[i]);
    }
    free_list_pnj(pnj_l);

    for (int j = 0; j < decor_l->nb_decor; j++) {
        free(decor_l->list[j]);
    }
    free_list_decor(decor_l);
    
    free(texture_pnj);
    free(texture_decor);


    return 0;
}
