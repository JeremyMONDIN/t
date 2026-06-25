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
#include "user.h"
#include "reinforce.h"
#include "text.h"

//"C:\Users\gaill\Documents\projet-zz1\projet global\Jeu SMA\assets\backgrounds\backgroundColorFall.png"
//"C:\Users\gaill\Documents\projet-zz1\projet global\Jeu SMA\assets\pnj\panda.png"

void init_affichage(SDL_Texture **texture_pnj,
                    SDL_Texture **texture_decor,
                    SDL_Window **window,
                    SDL_Renderer **renderer,
                    SDL_Texture **background,
                    SDL_Texture **texture_death,
                    int width,
                    int height,
                    int nb_texture_pnj,
                    int nb_texture_decor,
                    char *iamge_background,
                    char *image_pnj[5],
                    char *image_decor[4])
{
    printf("init texture\n");

    *window = SDL_CreateWindow("Grille", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                               // SDL_WINDOW_FULLSCREEN );
                               SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (*window == 0)
    {
        fprintf(stderr, "Erreur a la creation de la fenetre : %s\n", SDL_GetError());
        SDL_Delay(5000);
    }
    else
    {
        *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED); /* | SDL_RENDERER_PRESENTVSYNC */
        if (*renderer == 0)
        {
            fprintf(stderr, "Erreur de creation d'un renderer : %s\n", SDL_GetError());
        }
    }
    SDL_Surface *my_image = NULL;

    char *nom = malloc(100 * sizeof(char));
    sprintf(nom, "%s%s", "assets/backgrounds/", iamge_background);
    my_image = IMG_Load(nom);
    if (!my_image)
        printf("IMG_Load: %s\n", IMG_GetError());
    *background = SDL_CreateTextureFromSurface(*renderer, my_image);
    if (!*background)
        printf("SDL_CreateTextureFromSurface1: %s\n", IMG_GetError());
    SDL_FreeSurface(my_image);

    my_image = IMG_Load("assets/decors/Steak.png");
    if (!my_image)
        printf("IMG_Load: %s\n", IMG_GetError());
    *texture_death = SDL_CreateTextureFromSurface(*renderer, my_image);
    if (!texture_death)
        printf("SDL_CreateTextureFromSurface1: %s\n", IMG_GetError());
    SDL_FreeSurface(my_image);

    while (image_pnj[nb_texture_pnj] != NULL)
    {
        sprintf(nom, "%s%s", "assets/pnj/", image_pnj[nb_texture_pnj]);
        my_image = IMG_Load(nom);
        if (!my_image)
            printf("IMG_Load: %s\n", IMG_GetError());
        texture_pnj[nb_texture_pnj] = SDL_CreateTextureFromSurface(*renderer, my_image);
        if (!texture_pnj[nb_texture_pnj])
            printf("SDL_CreateTextureFromSurface1: %s\n", IMG_GetError());
        SDL_FreeSurface(my_image);
        nb_texture_pnj++;
    }
    while (image_decor[nb_texture_decor])
    {
        sprintf(nom, "%s%s", "assets/decors/", image_decor[nb_texture_decor]);
        my_image = IMG_Load(nom);
        if (!my_image)
            printf("IMG_Load: %s\n", IMG_GetError());
        texture_decor[nb_texture_decor] = SDL_CreateTextureFromSurface(*renderer, my_image);
        if (!texture_decor[nb_texture_decor])
            printf("SDL_CreateTextureFromSurface1: %s\n", IMG_GetError());
        SDL_FreeSurface(my_image);

        nb_texture_decor++;
    }
    free(nom);
}

int main()
{
    SDL_Texture *texture_death = NULL;

    pnj_list_t *pnj_l = NULL;
    decor_list_t *decor_l = NULL;

    pnj_grid_t *monde_pnj = NULL;
    decor_grid_t *monde_decor = NULL;

    SDL_Rect taille_monde = {1000, 700, 50, 50};

    // variable de menu
    int print_texte = 0;
    int affichage_graphique = 1;
    int temp_on = 1;
    int limite = 0;
    char buffer[3];

    int loop = 1;
    while (loop)
    {
        printf("Choisir un mode :\n");
        printf("1 - Graphique on/off \n");
        printf("2 - Textuel on/off \n");
        printf("3 - limite 10 000 itération\n");
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
        printf("Limite ");
        if (limite)
            printf("on\n");
        else
            printf("off\n");


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
            limite = !limite;
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

    srand(time(NULL));

    SDL_Rect rect = {0},
             user = {0, 0, 100, 100};

    pnj_t *select = NULL;

    int program_on = 1,
        nb_texture_pnj = 0,
        nb_texture_decor = 0;

    int width = 1000,
        height = 700;

    int nb_pnj[4] = {20, 0, 20, 30},
        nb_decor[3] = {50, 10, 0};

    
    // Vitesse, Force, Vision
    caract_t stat_panda = {5, 40, 20};
    caract_t stat_pinguin = {100, 100, 100};
    caract_t stat_lapin = {15, 1, 50};
    caract_t stat_snake = {20, 20, 50};

    //{base, vie, energie, faim, nb_nourriture, nb_allie, nb_ennemi, nb_proie, d_nourriture, d_allie, d_ennemi, d_proie, rapport_force, rapport_vitesse}

    float coef_proie[7][14]={
       // explorer
            { 4,  1,  1, -1, -1,  1, -3, -2,  1, -3,  0,  0,  0,  0},
      // manger
            {-3, -1, -2,  5,  3,  0, -4,  5,  0, -4,  0,  0,  0,  0},
      // defendre
            {-4, -2, -1,  0,  0,  1,  4,  0,  1,  4,  4,  3,  0,  0},
      // attaquer
            {-1000,0,0,0,0,0,0,0,0,0,0,0,0,0},
      // reproduire
            {-2,  3,  3, -3,  0,  4, -2,  0,  3, -2,  0,  0,  0,  0},
      // fuir
            {-4, -2, -1,  0,  0, -1,  6,  0,  0,  6, -5, -4,  0,  0},
      // repos
            { 0, -3, -5, -2,  0,  0, -1,  0,  0,  0,  0,  0,  0,  0}
    };


    float coef_carn[7][14]={
      // explorer
            { 3,  1,  1, -1, -1,  0, -1, -2,  0, -1,  0,  0,  1, -1},
      // manger
            {5, -1, -2,  5,  0,  0, -2,  0,  0, -2,  0,  0,  4,  5},
      // defendre
            {-3, -2, -1,  0,  0,  1,  3,  0,  1,  3,  4,  3,  0,  0},
      // attaquer
            { 12,  1,  1,  2,  0, -1, -2,  0,  0, -2,  4,  3,  5,  5},
      // reproduire
            {-2,  3,  3, -3,  0,  3, -2,  0,  3, -2,  0,  0,  0,  0},
      // fuir
            {-3, -2, -1,  0,  0, -1,  5,  0,  0,  5, -5, -4,  0,  0},
      // repos
            { 0, -3, -5, -2,  0,  0, -1,  0,  0,  0,  0,  0,  0,  0}
    };


    float coef_omni[7][14]={
      // explorer
            { 3,  1,  1, -1, -1,  1, -2, -2,  1, -2,  0,  0,  1, -1},
      // manger
            {-2, -1, -2,  5,  2,  0, -3,  4,  0, -3,  0,  0,  2,  3},
      // defendre
            {-3, -2, -1,  0,  0,  1,  3,  0,  1,  3,  4,  3,  0,  0},
      // attaquer
            { 0,  1,  1,  1,  0, -1, -2,  0,  0, -2,  3,  2,  4,  4},
      // reproduire
            {-2,  3,  3, -3,  0,  4, -2,  0,  3, -2,  0,  0,  0,  0},
      // fuir
            {-3, -2, -1,  0,  0, -1,  5,  0,  0,  5, -5, -4,  0,  0},
      // repos
            { 0, -3, -5, -2,  0,  0, -1,  0,  0,  0,  0,  0,  0,  0}
    }; 

    SDL_Texture **texture_pnj = malloc(10 * sizeof(SDL_Texture *));
    SDL_Texture **texture_decor = malloc(10 * sizeof(SDL_Texture *));

    SDL_Event event;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Texture *background = NULL;

    pnj_l = init_pnj_list();
    decor_l = init_decor_list();

    char *iamge_background = "Designer.png";
    char *image_pnj[5] = {"panda.png", "penguin.png", "rabbit.png", "snake.png", NULL};
    char *image_decor[4] = {"Carotte.png", "Potato.png", "Steak.png", NULL};

    if (affichage_graphique)
    {
        init_affichage(texture_pnj,
                       texture_decor,
                       &window,
                       &renderer,
                       &background,
                       &texture_death,
                       width,
                       height,
                       nb_texture_pnj,
                       nb_texture_decor,
                       iamge_background,
                       image_pnj,
                       image_decor);
    }

    // initialisation du monde
    printf("init monde\n");
    init_monde(&monde_pnj, &monde_decor, taille_monde);
    SDL_Rect pos = {0, 0, 30, 30};

    int n = 4;
    espece_t tab_espece[4] = {
        create_espece("panda", 2, texture_pnj[0], stat_panda,coef_omni),
        create_espece("pinguin", 2, texture_pnj[1], stat_pinguin,coef_omni),
        create_espece("lapin", 0, texture_pnj[2], stat_lapin,coef_proie),
        create_espece("snake", 1, texture_pnj[3], stat_snake,coef_carn)};

    /*FILE *file = fopen("data/save_espece.txt", "r");
    espece_t *tab_espece = load_all_espece(file, &n);
    fclose(file);*/

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < nb_pnj[i]; j++)
        {
            pos.x = rand() % taille_monde.x;
            pos.y = rand() % taille_monde.y;
            add_pnj(&tab_espece[i], pos, monde_pnj, pnj_l, taille_monde);
        }
    }

    // patate
    for (int j = 0; j < nb_decor[1]; j++)
    {
        pos.x = rand() % taille_monde.x;
        pos.y = rand() % taille_monde.y;
        add_decor(1, texture_decor[1], 1, 5, pos, monde_decor, decor_l, taille_monde);
    }

    // steak
    for (int j = 0; j < nb_decor[2]; j++)
    {
        pos.x = rand() % taille_monde.x;
        pos.y = rand() % taille_monde.y;
        add_decor(2, texture_decor[2], 2, 5, pos, monde_decor, decor_l, taille_monde);
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

    FILE *log = fopen("data/log.txt", "w");

    while (program_on)
    {
        if (limite && i > 10000)
            program_on = 0;
        if (temp_on)
            etape_suivante(pnj_l, decor_l, i++ % 30, monde_pnj, monde_decor, taille_monde, texture_death, affichage_graphique, print_texte, log);

        compte = 0;
        for (int j = 0; j < monde_pnj->nb_case_x; j++)
        {
            for (int k = 0; k < monde_pnj->nb_case_y; k++)
            {
                compte += monde_pnj->grid[j][k]->nb_pnj;
            }
        }
        fprintf(log, "boucle %d ***************************** %d\n", i, compte);
        if (print_texte && temp_on)
            printf("boucle %d ***************************** %d\n", i, compte);
        if (i % 200 == 0)
        {
            // Entraîner CHAQUE espèce avec SES PROPRES trajectoires
            for (int e = 0; e < 3; e++)
            {
                // Collecter les trajectoires de cette espèce uniquement
                int N_espece = 0;
                for (int j = 0; j < pnj_l->nb_pnj; j++)
                {
                    if (pnj_l->list[j]->espece->alim == e)
                        N_espece++;
                }

                if (N_espece > 0)
                {
                    traj_t **traj_espece = malloc(N_espece * sizeof(traj_t *));
                    int *tailles = malloc(N_espece * sizeof(int));

                    int k = 0;
                    for (int j = 0; j < pnj_l->nb_pnj; j++)
                    {
                        if (pnj_l->list[j]->espece->alim == e)
                        {
                            traj_espece[k] = &pnj_l->list[j]->trajectoire;
                            tailles[k] = pnj_l->list[j]->trajectoire.taille;
                            k++;
                        }
                    }

                    float maxi = tab_espece[e].theta[0][0];
                    float mini = tab_espece[e].theta[0][0];
                    for (int i = 0; i < 7; i++)
                    {
                        for (int j = 0; j < 14; j++)
                        {
                            if (tab_espece[e].theta[i][j] > maxi)
                            {
                                maxi = tab_espece[e].theta[i][j];
                            }
                            if (tab_espece[e].theta[i][j] < mini)
                            {
                                mini = tab_espece[e].theta[i][j];
                            }
                        }
                    }

                    // Entraîner avec theta de CETTE espèce
                    algo_REINFORCE(0.99, N_espece, 0.01, 0.2, 5, 7,
                                   traj_espece, tab_espece[e].theta, tailles, mini, maxi);

                    free(traj_espece);
                    free(tailles);
                }
            }
        }

        if (affichage_graphique)
        {
            while (SDL_PollEvent(&event))
            {
                rect = get_demande(event);
                user_modif(rect, &user, &program_on, &temp_on, &select, monde_pnj, taille_monde);
                rect.h = 0;
            }
            if (select)
                printf("%p %s  stat %d %d %d etat %d %d  x=%d  y=%d  point=%d\n",
                       select, select->espece->nom, select->vie, select->energie, select->faim, select->etat.id, select->etat.step, select->pos.x, select->pos.y, select->lock_anim);

            affichage(window, renderer, background, user, monde_pnj, monde_decor, taille_monde, temp_on);
            SDL_RenderPresent(renderer);
            SDL_Delay(100);
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
            if (compte < 100)
            {
                pos.x = rand() % taille_monde.x;
                pos.y = rand() % taille_monde.y;
                add_decor(1, texture_decor[1], 1, 5, pos, monde_decor, decor_l, taille_monde);
            }
        }
    }
    fclose(log);

    FILE *save_traj = fopen("data/save_traj.txt", "w");
    save_all_traj(save_traj, pnj_l);
    fclose(save_traj);
    FILE *save_esp = fopen("data/save_espece.txt", "w");
    save_all_espece(save_esp, tab_espece, n);
    fclose(save_esp);

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
    for (int i = 0; i < pnj_l->nb_pnj; i++)
    {
        // Libérer les éléments de la trajectoire
        if (pnj_l->list[i]->trajectoire.liste != NULL)
        {
            free(pnj_l->list[i]->trajectoire.liste);
        }
        // PUIS libérer le PNJ lui-même
        free(pnj_l->list[i]);
    }

    free_list_pnj(pnj_l);

    for (int j = 0; j < decor_l->nb_decor; j++)
    {
        free(decor_l->list[j]);
    }
    free_list_decor(decor_l);

    free(texture_pnj);
    free(texture_decor);

    free_monde(monde_pnj, monde_decor);

    return 0;
}
