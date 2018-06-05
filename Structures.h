#ifndef _STRUCTURE
#define _STRUCTURE

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>

typedef struct{
    int l;
    int c;
}
COUPLE;

typedef struct{
    int inf;
    int sup;
}
BORNES;

typedef struct{
    int commande_menu[3];
    int commande_jeu[6];
}/**Contient les tableaux concernant les commandes du jeu*/
COMMANDE;

typedef struct{
    /**la valeur que prend l'axe*/
    int valeur;
    /**utilise vaut 1 si l'axe l'a ete et 0 sinon*/
    int utilise;
} /**Structure decrivant un axe, sa valeur et si il a deja ete utilise*/
AXE;

typedef struct{
    /**tableau contenant toutes les touches du clavier
    la case vaut 1 si la touche a ete pressee, 0 sinon*/
	char key[SDL_NUM_SCANCODES];
	/**matrice contenant tout les axes de toutes les manettes*/
	AXE joyaxes[4][7];
	/**matrice contenant toutes les touches de toutes les manettes
    la case vaut 1 si la touche a ete pressee, 0 sinon*/
	int joybutton[4][12];
	/**matrice contenant tous les "Hat"="Chapeau" de toutes les manettes
    la case vaut 1 si la touche a ete pressee, 0 sinon*/
	int joyhat[4][10];
	/**permet de savoir si un joystick a ete rajoute*/
	int joyadded;
	/**permet de savoir si un joystick a ete enleve*/
	int joyremoved;
	/**la position suivant les 'x' de la souris*/
	int mousex;
	/**la position suivant les 'y' de la souris*/
	int mousey;
	/**la position de la souris relativement a son ancienne position suivant les 'x'*/
	int mousexrel;
	/**la position de la souris relativement a son ancienne position suivant les 'y'*/
	int mouseyrel;
	/**tableau contenant tous les boutons de la souris
	la case vaut 1 si la touche a ete pressee, 0 sinon
	utilise : SDL_BUTTON_LEFT/SDL_BUTTON_MIDDLE
	/SDL_BUTTON_RIGHT/SDL_BUTTON_WHEELUP/SDL_BUTTON_WHEELDOWN*/
	char mousebuttons[8];   //SDL_BUTTON_X1 / SDL_BUTTON_X2
	/**indique si on a appuye sur la croix rouge*/
    char quit;
} /**structure qui contient tout les evenements dont on a besoin, il suffit de regarder au bonne endroit*/
INPUT;

typedef struct{
    /**la definition de l'ecran pour le GPU*/
    SDL_Window *sdlWindow;
    /**l'equivalent de l'ecran pour le GPU, c'est sur lui qu'on colle les textures*/
    SDL_Renderer *sdlRenderer;
    /**la definition de l'ecran pour le CPU, c'est sur lui qu'on blit les surfaces*/
    SDL_Surface *ecran;
    /**perrmet de faire la transition entre le CPU et le GPU en lui collant la surface ecran*/
    SDL_Texture *sdlTexture;

    /**tableau contant toutes les surfaces associe a toutes les voitures dont on a besoin*/
    SDL_Surface **S_voiture;
    /**le nombre d'image de voiture*/
    int nbvoiture;
    /**tableau contant toutes les surfaces associe a toutes les elements du jeu dont on a besoin*/
    SDL_Surface **S_element;
    /**le nombre de surface d'elements*/
    int nbelement;
    /**tableau contant toutes les surfaces associe a toutes les menus dont on a besoin*/
    SDL_Surface **S_menu;
    /**le nombre d'image de menu*/
    int nbmenu;
    /**tableau contant toutes les surfaces associe a toutes les circuit dont on a besoin*/
    SDL_Surface **S_circuit;
    /**le nombre d'image de circuit*/
    int nbcircuit;
    /**tableau contant toutes les surfaces associe aux lettre et aux chiffres utiles pour faire le score*/
    SDL_Surface **S_score;
    /**le nombre de score**/
    int nbscore;

    /**tableau contant toutes les textures associees a toutes les voitures dont on a besoin*/
    SDL_Texture **T_voiture;
    /**tableau contant toutes les textures associees a toutes les elements du jeu dont on a besoin*/
    SDL_Texture **T_element;
    /**tableau contant toutes les textures associees a toutes les meuus dont on a besoin*/
    SDL_Texture **T_menu;
    /**tableau contant toutes les textures associees a toutes les circuits dont on a besoin*/
    SDL_Texture **T_circuit;
    /**tableau contant toutes les textures associe aux lettre et aux chiffres utiles pour faire le score*/
    SDL_Texture **T_score;

    /**tableau contenant tous les sons / bruitages dont on a besoin*/
    Mix_Chunk **sons;
    /**nombre de son a disposition*/
    int nbsons;

    /**tableau contenant toutes les musiques dont on a besoin*/
    Mix_Music **musique;
    /**nombre de musique a disposition*/
    int nbmusique;

    /**tableau contenant toutes les manettes a disposition*/
    SDL_Joystick **joystick;
    /**nombre de manettes a diposition*/
    int nbjoystick;
    /**nombre de joueur present*/
    int nbjoueur;
    /**nombre de jeu different disponible*/
    int nbjeu;
    /**la police prete a etre utilisee*/
    TTF_Font *police;
    /**la resolution en X de l'ecran*/
    int Xres;
    /**la resolution en Y de l'ecran*/
    int Yres;
    /**matrice d'entier contenant le relief*/
    int** relief;
	
    COUPLE haut_gauche;
COUPLE haut_droit;
COUPLE bas_droit;
COUPLE bas_gauche;
    COMMANDE manette[3];
} /**contient toutes les informations dont on peut avoir besoin sur le jeu en general*/
CONTEXT;

typedef struct{
    /**vaut 1 si on est en saut, 0 sinon*/
    int etat;
    /**vitesse au debut du saut*/
    double vo;
    /**altitude au depart du saut*/
    int zo;
    /**temps du debut du saut*/
    int debut_saut;
    /**temps de la fin du saut*/
    int fin_saut;
}/**contient toute les informations necessaire au bon deroulement du saut*/
SAUT;

typedef struct{
    int compteur_moteur;
    int compteur_derapage;
    int compteur_rebond;
    int compteur_crash;
}/**contient tous les compteurs evitant que les sons ne se "marchent" dessus */
SON;

typedef struct{
    /**contient la position du joueur par rapport au videoprojecteur*/
    int position_joueur;
    /**la largeur de la voiture*/
    int w;
    /**la hauteur de la voiture*/
    int h;
    /**la position en x de la voiture*/
    double x;
    /**la position en y de la voiture*/
    double y;
    /**la force du moteur commande par l'utilisateur*/
    double force_moteur;
    /**la vitesse de la voiture*/
    double vitesse;
    /**la rotation de la voiture*/
    double rotation;
    /**vaut 1 si l'utilisateur veut freiner, 0 sinon*/
    int freinage;
    /**vaut 1 si l'utilisateur a deraper au tour precedent, 0 sinon*/
    int derapage_av;
    /**vaut 1 si l'utilisateur derape, 0 sinon*/
    int derapage;
    /**contient toute les informations necessaire au bon deroulement du saut*/
    SAUT saut;
    /**angle de la pente derriere la voiture en radian*/
    double alpha;
    /**la constante permettant de converger vers l'angle voulue*/
    double virage;
    /**vaut 1 si on rebondi sur un mur, 0 sinon*/
    int rebondi;
    /**compteur pouvant aller jusqu'a 3, compte le nb de boucles depuis celle où on a rebondi*/
    int compteur_rebond;
    /**contient tous les compteurs evitant que les sons ne se "marchent" dessus */
    SON son;

    COMMANDE manette;
} /**contient toutes les information sur la voiture dont on peut avoir besoin*/
POSITION;

typedef struct{
    /**position du centre du cercle*/
    int x,y;
    /**rayon du cercle*/
    int rayon;
}/**contient les informations utiles concernant un cercle*/
CERCLE;

typedef struct{
  double x,y;
}
POINT;

typedef struct{
  double x,y;
}
VECTEUR;

typedef struct{
    /**le drapeau de qui ?*/
    int dequi;
    /**a-t-il ete pris ? 1=oui / 0=non*/
    int pris;
    /**par qui a-t-il ete pris ?*/
    int parqui;
}/**contient toutes les information sur un flag dont on peut avoir besoin*/
INFOSFLAG;


#endif // _STRUCTURE

