#include "Fonctions.h"

int main ( int argc, char *argv[] ){
    int choix=0,quitter=0,start=0,precedent=1, erreur_init;
    CONTEXT C;

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK);
	SDL_DisplayMode resolution;
	SDL_GetDesktopDisplayMode(0,&resolution);
	printf("%d %d\n",resolution.h,resolution.w);

    C.relief=initialisation_manuel_relief(resolution.h,resolution.w);


    InitAll(&C,&erreur_init); //Initialise tout
    if(gestion_erreur(C,erreur_init)==1){ //si on a une erreur
        Release(&C); //on arrete tout
        return -1;
    }
    //C.relief=creeMatricePyramideCentrale(C.Yres,C.Xres,0,400,5); //Creation de de la matrice du relief
    //CreationTextureRelief(&C,C.relief, C.Yres, C.Xres, 0, 0); //Creation de la texture du relief

    InitCommande(&(C.manette[0]));
    InitCommande(&(C.manette[1]));
    InitCommande(&(C.manette[2]));
    Mix_PlayMusic(C.musique[0], -1);
    MenuAide(&C,0,&(C.manette[0]));
    do{
        if(precedent==1){ //quand precedent vaut 1 cela se refere au menu Start
            Mix_ResumeMusic();
            MenuStart(&C,&quitter,&start,&precedent); //gestion du menu start
        }
        if(start==1){ //si l'utilisateur veut commencer
            if(precedent==2){ //quanf precedent vaut 2 cela se refere au menu joueur
                Mix_ResumeMusic();
                MenuJoueur(&C,&precedent); //gestion du menu joueur
            }
            if(precedent==3){ //quand precedent vaut 3 cela se refere au menu jeu
                Mix_ResumeMusic();
                MenuJeu(&C,&precedent,&choix); //gestion du menu jeu

                Mix_RewindMusic();
                Mix_PauseMusic();
                if(choix==1){  //gestion de tout les choix possibles
                    deroulement_cerise_joystick(&C,&precedent);
                }
                else if(choix==2){
                    deroulement_flag_joystick(&C,&precedent);
                }
                else if(choix==3){
                    deroulement_course_joystick(&C,&precedent);
                }
                else{}
                Mix_Pause(-1); //met en pause tous les canaux
            }
        }
    }while(quitter==0); //on continue tant que l'utilisateur ne veut pas quitter

    Release(&C);
    return 0;
}


//Le 09/04/2018, il y a 2968 lignes de codes dans le projet
