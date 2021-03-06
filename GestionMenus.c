#include "Fonctions.h"


void MenuStart(CONTEXT *C, int* quitter, int* start,int *precedent){
    int i=0;
    Input in; //tableau des evenements
    SDL_Rect menu[2],contour[2];

    *quitter=0; *start=0; *precedent=0;
    mise_a_zero_input(&in);
    //recuperation des tailles des textures
    SDL_QueryTexture(C->T_menu[BOUTON_START],NULL,NULL,&(menu[0].w),&(menu[0].h));
    SDL_QueryTexture(C->T_menu[BOUTON_QUITTER],NULL,NULL,&(menu[1].w),&(menu[1].h));
    SDL_QueryTexture(C->T_menu[CONTOUR_MENU], NULL, NULL, &(contour[0].w), &(contour[0].h));
    SDL_QueryTexture(C->T_menu[CONTOUR_MENU], NULL, NULL, &(contour[1].w), &(contour[1].h));

    //definitions des coordonnees des differentes textures
    menu[0].x=(int)((C->Xres-menu[0].w)/2-71);   menu[0].y=(int)((C->Yres-menu[0].h)/2-271);
    menu[1].x=(int)((C->Xres-menu[1].w)/2-71);   menu[1].y=(int)((C->Yres-menu[1].h)/2-122);
    contour[0].x=menu[0].x;                      contour[0].y=menu[0].y;
    contour[1].x=menu[1].x;                      contour[1].y=menu[1].y;

    //affichage de ses textures
    SDL_RenderClear(C->sdlRenderer);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_START], NULL, &menu[0]);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_QUITTER], NULL, &menu[1]);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[CONTOUR_MENU], NULL, &(contour[0]));
    SDL_RenderPresent(C->sdlRenderer);

    while(1){ //boucle infini
        UpdateEventsW(&in);
        if(in.joyadded){ //rajout d'une manette
            if(C->nbjoystick!=SDL_NumJoysticks()){
                ReallocationJoystick(C);
            }
            in.joyadded=0;
        }
        if(in.joyremoved){ //retrait d'une manette
            if(C->nbjoystick!=SDL_NumJoysticks()){
                ReallocationJoystick(C);
            }
            in.joyremoved=0;
        }
        if(in.joyhat[0][SDL_HAT_DOWN]){ //mouvement du hat vers le bas
            i++;
            if(i>1){
                i=0;
            }
            in.joyhat[0][SDL_HAT_DOWN]=0;
        }
        else if(in.joyhat[0][SDL_HAT_UP]){ //mouvement du hat vers le haut
            i--;
            if(i<0){
                i=1;
            }
            in.joyhat[0][SDL_HAT_UP]=0;
        }
        else if(in.joybutton[0][BX]){ //bouton X presse, on veut quitter le jeu
            in.joybutton[0][BX]=0;

            Mix_VolumeChunk(C->sons[SON_CLIC], MIX_MAX_VOLUME);
            Mix_PlayChannel(CANAL_CLIC, C->sons[SON_CLIC], 0);

            *quitter=1;
            *precedent=0;
            return;
        }
        else if(in.joybutton[0][BA]){ //Bouton A presse, on selectionne un des boutons
            in.joybutton[0][BA]=0;

            Mix_VolumeChunk(C->sons[SON_CLIC], MIX_MAX_VOLUME);
            Mix_PlayChannel(CANAL_CLIC, C->sons[SON_CLIC], 0);
            //suivant la position du contour, on fait differentes actions
            if(i==0){ //continuer au menu suivant
                *start=1;
                *precedent=2;
            }
            else{ //quitter le jeu
                *quitter=1;
                *precedent=0;
                *start=0;
            }
            return;
        }
        else if(in.key[SDL_SCANCODE_ESCAPE]){ //si on appuye sur la touche echap
            *quitter=1; //on quitte
            *start=0;
            return;
        }

        SDL_RenderClear(C->sdlRenderer);
        SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_START], NULL, &menu[0]);
        SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_QUITTER], NULL, &menu[1]);
        SDL_RenderCopy(C->sdlRenderer, C->T_menu[CONTOUR_MENU], NULL, &(contour[i]));
        SDL_RenderPresent(C->sdlRenderer);
    }
}

void MenuJoueur(CONTEXT *C, int* pre){
    int i=1;
    Input in;
    SDL_Rect menu[3],precedent,contour[4];

    *pre=0;
    mise_a_zero_input(&in);
    //recuperation de la tailles des textures
    SDL_QueryTexture(C->T_menu[BOUTON_PRECEDENT], NULL, NULL, &(precedent.w), &(precedent.h));
    SDL_QueryTexture(C->T_menu[BOUTON_1JOUEUR], NULL, NULL, &(menu[0].w), &(menu[0].h));
    SDL_QueryTexture(C->T_menu[BOUTON_2JOUEUR], NULL, NULL, &(menu[1].w), &(menu[1].h));
    SDL_QueryTexture(C->T_menu[BOUTON_3JOUEUR], NULL, NULL, &(menu[2].w), &(menu[2].h));

    SDL_QueryTexture(C->T_menu[CONTOUR_PRECEDENT], NULL, NULL, &(contour[0].w), &(contour[0].h));
    SDL_QueryTexture(C->T_menu[CONTOUR_MENU], NULL, NULL, &(contour[1].w), &(contour[1].h));
    SDL_QueryTexture(C->T_menu[CONTOUR_MENU], NULL, NULL, &(contour[2].w), &(contour[2].h));
    SDL_QueryTexture(C->T_menu[CONTOUR_MENU], NULL, NULL, &(contour[3].w), &(contour[3].h));

    //definitions des coordonnees des differentes textures
    precedent.x=C->Xres-100-precedent.w;   precedent.y=C->Yres-50-precedent.h;
    contour[0].x=C->Xres-100-contour[0].w; contour[0].y=C->Yres-50-contour[0].h;

    menu[0].x=(int)((C->Xres-menu[0].w)/2-70);      menu[0].y=(int)((C->Yres-menu[0].h)/2-300);
    menu[1].x=(int)((C->Xres-menu[1].w)/2-70);      menu[1].y=(int)((C->Yres-menu[1].h)/2-150);
    menu[2].x=(int)((C->Xres-menu[2].w)/2-70);      menu[2].y=(int)((C->Yres-menu[2].h)/2);

    contour[1].x=menu[0].x;                contour[1].y=menu[0].y;
    contour[2].x=menu[1].x;                contour[2].y=menu[1].y;
    contour[3].x=menu[2].x;                contour[3].y=menu[2].y;

    //affichage de ses textures
    SDL_RenderClear(C->sdlRenderer);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_PRECEDENT], NULL, &precedent);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_1JOUEUR], NULL, &menu[0]);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_2JOUEUR], NULL, &menu[1]);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_3JOUEUR], NULL, &menu[2]);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[CONTOUR_MENU], NULL, &(contour[1]));
    SDL_RenderPresent(C->sdlRenderer);

    while(1){ //boucle infini
        UpdateEventsW(&in);
        if(in.joyadded){ //on ajoute une manette
            if(C->nbjoystick!=SDL_NumJoysticks()){
                ReallocationJoystick(C);
            } //on reallou donc dinamiquement le tableau des manettes
            in.joyadded=0;
        }
        if(in.joyremoved){ //on enleve une manette
            if(C->nbjoystick!=SDL_NumJoysticks()){
                ReallocationJoystick(C);
            }
            in.joyremoved=0;
        }
        if(in.joyhat[0][SDL_HAT_DOWN]){ //on appuie sur la fleche directionnelle bas de la manette
            i++;
            if(i>3){
                i=0;
            }
            in.joyhat[0][SDL_HAT_DOWN]=0;
        }
        else if(in.joyhat[0][SDL_HAT_UP]){ //on appuie sur la fleche directionnelle haut de la manette
            i--;
            if(i<0){
                i=3;
            }
            in.joyhat[0][SDL_HAT_UP]=0;
        }
        else if(in.joybutton[0][BX]){ //bouton X, ie touche retour menu precedent
            in.joybutton[0][BX]=0;

            Mix_VolumeChunk(C->sons[SON_CLIC], MIX_MAX_VOLUME);
            Mix_PlayChannel(CANAL_CLIC, C->sons[SON_CLIC], 0);

            *pre=1;
            return;
        }
        else if(in.joybutton[0][BA]){ //Bouton A ie on selectionne un des boutons
            in.joybutton[0][BA]=0;

            Mix_VolumeChunk(C->sons[SON_CLIC], MIX_MAX_VOLUME);
            Mix_PlayChannel(CANAL_CLIC, C->sons[SON_CLIC], 0);
            if(i==0){ //le contour est sur le bouton precedent
                *pre=1; //on va dans le menu Start
                return;
            }
            else{ //le contour est sur un des autres boutons
                if(i<=C->nbjoystick){ //il y a suffisamment de mannettes par rapport a ce qu'il y a de marque sur le bouton
                    *pre=3; //on va dans le menu Jeu
                    C->nbjoueur=i;
                    return;
                }
                else{ //ou pas et on le dit alors
                    Avertissement(*C,menu[i-1]);
                }
            }
        }
        else if(in.key[SDL_SCANCODE_ESCAPE]||in.quit){ //on appuye sur la touche echap
            *pre=1;
            return;
        }

        SDL_RenderClear(C->sdlRenderer);
        SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_PRECEDENT], NULL, &precedent);
        SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_1JOUEUR], NULL, &menu[0]);
        SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_2JOUEUR], NULL, &menu[1]);
        SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_3JOUEUR], NULL, &menu[2]);
        if(i==0){
            SDL_RenderCopy(C->sdlRenderer, C->T_menu[CONTOUR_PRECEDENT], NULL, &(contour[0]));
        }
        else{
            SDL_RenderCopy(C->sdlRenderer, C->T_menu[CONTOUR_MENU], NULL, &(contour[i]));
        }
        SDL_RenderPresent(C->sdlRenderer);
    }
}

void MenuJeu(CONTEXT *C, int *pre, int *choix){
    int i=1;
    Input in;
    SDL_Rect menu[C->nbjeu],contour[C->nbjeu+1],precedent;

    *choix=0;
    mise_a_zero_input(&in);

    //recuperation de la tailles des textures
    SDL_QueryTexture(C->T_menu[BOUTON_PRECEDENT], NULL, NULL, &(precedent.w), &(precedent.h));
    SDL_QueryTexture(C->T_menu[BOUTON_JEUCERISE],NULL,NULL,&(menu[0].w),&(menu[0].h));
    SDL_QueryTexture(C->T_menu[BOUTON_JEUPRISEFLAG],NULL,NULL,&(menu[1].w),&(menu[1].h));
    SDL_QueryTexture(C->T_menu[BOUTON_JEUCOURSE],NULL,NULL,&(menu[2].w),&(menu[2].h));
    SDL_QueryTexture(C->T_menu[CONTOUR_PRECEDENT], NULL, NULL, &(contour[0].w), &(contour[0].h));
    SDL_QueryTexture(C->T_menu[CONTOUR_MENU], NULL, NULL, &(contour[1].w), &(contour[1].h));
    SDL_QueryTexture(C->T_menu[CONTOUR_MENU], NULL, NULL, &(contour[2].w), &(contour[2].h));
    SDL_QueryTexture(C->T_menu[CONTOUR_MENU], NULL, NULL, &(contour[3].w), &(contour[3].h));

    //definitions des coordonnees des differentes textures
    precedent.x=C->Xres-100-precedent.w;   precedent.y=C->Yres-50-precedent.h;
    menu[0].x=50;      menu[0].y=50;
    menu[1].x=50;      menu[1].y=menu[0].y+menu[0].h+50;
    menu[2].x=50;      menu[2].y=menu[1].y+menu[1].h+50;
    contour[0].x=C->Xres-100-precedent.w;  contour[0].y=C->Yres-50-precedent.h;
    contour[1].x=menu[0].x;               contour[1].y=menu[0].y;
    contour[2].x=menu[1].x;               contour[2].y=menu[1].y;
    contour[3].x=menu[2].x;               contour[3].y=menu[2].y;

    //affichage de ses textures
    SDL_RenderClear(C->sdlRenderer);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_PRECEDENT], NULL, &precedent);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_JEUCERISE], NULL, &menu[0]);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_JEUPRISEFLAG], NULL, &menu[1]);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_JEUCOURSE], NULL, &menu[2]);
    SDL_RenderCopy(C->sdlRenderer, C->T_menu[CONTOUR_MENU], NULL, &(contour[1]));
    SDL_RenderPresent(C->sdlRenderer);

    while(1){
        UpdateEventsW(&in);
        if(in.key[SDL_SCANCODE_1]){ //on appuye sur la touche 1
            *choix=1;
            *pre=1;
            return;
        }
        else if(in.key[SDL_SCANCODE_ESCAPE]||in.quit){ //on appuye sur la touche echap
            *pre=2;
            return;
        }
        else if(in.joyhat[0][SDL_HAT_DOWN]){ //on veut faire descendre le contour
            i++;
            if(i>C->nbjeu){
                i=0;
            }
            in.joyhat[0][SDL_HAT_DOWN]=0;
        }
        else if(in.joyhat[0][SDL_HAT_UP]){ //on veut faire monter le contour
            i--;
            if(i<0){
                i=C->nbjeu;
            }
            in.joyhat[0][SDL_HAT_UP]=0;
        }
        else if(in.joybutton[0][BX]){ //bouton X, on veut aller au menu precedent
            in.joybutton[0][BX]=0;

            Mix_VolumeChunk(C->sons[SON_CLIC], MIX_MAX_VOLUME);
            Mix_PlayChannel(CANAL_CLIC, C->sons[SON_CLIC], 0);

            *pre=2;
            return;
        }
        else if(in.joybutton[0][BA]){ //Bouton A, on selectionne un des boutons
            in.joybutton[0][BA]=0;

            Mix_VolumeChunk(C->sons[SON_CLIC], MIX_MAX_VOLUME);
            Mix_PlayChannel(CANAL_CLIC, C->sons[SON_CLIC], 0);
            if(i==0){ //on est sur le bouton precedent
                *pre=2;
            }
            else{ //ou sur un autre bouton
                *choix=i;
            }
            return;
        }
        else{}

        SDL_RenderClear(C->sdlRenderer);
        SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_PRECEDENT], NULL, &precedent);
        SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_JEUCERISE], NULL, &menu[0]);
        SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_JEUPRISEFLAG], NULL, &menu[1]);
        SDL_RenderCopy(C->sdlRenderer, C->T_menu[BOUTON_JEUCOURSE], NULL, &menu[2]);
        if(i==0){
            SDL_RenderCopy(C->sdlRenderer, C->T_menu[CONTOUR_PRECEDENT], NULL, &(contour[0]));
        }
        else{
            SDL_RenderCopy(C->sdlRenderer, C->T_menu[CONTOUR_MENU], NULL, &(contour[i]));
        }
        SDL_RenderPresent(C->sdlRenderer);
    }
}

void Avertissement(CONTEXT C, SDL_Rect menu){
    SDL_Rect avertissement;
    SDL_QueryTexture(C.T_menu[BOUTON_AVERTISSEMENT],NULL,NULL,&(avertissement.w),&(avertissement.h));

    //positionnement du bouton avertissement suivant l'endroit du contour
    avertissement.x=menu.x+400;
    avertissement.y=menu.y;

    SDL_RenderCopy(C.sdlRenderer, C.T_menu[BOUTON_AVERTISSEMENT], NULL, &avertissement);
    SDL_RenderPresent(C.sdlRenderer);
    SDL_Delay(1000); //attente de 1s
}

void MenuPause(CONTEXT C,int* precedent,int *option,int joueur){
    int i=0;
    Input in;
    SDL_Rect menu,contour[5];

    *precedent=0;
    mise_a_zero_input(&in);

    //recuperation de la tailles des textures
    SDL_QueryTexture(C.T_menu[MENU_PAUSE],NULL,NULL,&(menu.w),&(menu.h));
    SDL_QueryTexture(C.T_menu[CONTOUR_PAUSE], NULL, NULL, &(contour[0].w), &(contour[0].h));
    SDL_QueryTexture(C.T_menu[CONTOUR_PAUSE], NULL, NULL, &(contour[1].w), &(contour[1].h));
    SDL_QueryTexture(C.T_menu[CONTOUR_PAUSE], NULL, NULL, &(contour[2].w), &(contour[2].h));
    SDL_QueryTexture(C.T_menu[CONTOUR_PAUSE], NULL, NULL, &(contour[3].w), &(contour[3].h));
    SDL_QueryTexture(C.T_menu[CONTOUR_PAUSE], NULL, NULL, &(contour[4].w), &(contour[4].h));

    //definitions des coordonnees des differentes textures
    menu.x=(int)((C.Xres-menu.w)/2);      menu.y=(int)((C.Yres-menu.h)/2);
    contour[0].x=menu.x+25;               contour[0].y=menu.y+34;
    contour[1].x=menu.x+25;               contour[1].y=menu.y+89;
    contour[2].x=menu.x+24;               contour[2].y=menu.y+144;
    contour[3].x=menu.x+24;               contour[3].y=menu.y+199;
    contour[4].x=menu.x+24;               contour[4].y=menu.y+254;

    //affichage de ses textures
    SDL_RenderCopy(C.sdlRenderer, C.T_menu[MENU_PAUSE], NULL, &menu);
    SDL_RenderCopy(C.sdlRenderer, C.T_menu[CONTOUR_PAUSE], NULL, &(contour[0]));
    SDL_RenderPresent(C.sdlRenderer);

    while(1){
        UpdateEventsW(&in);
        if(in.joyhat[joueur][SDL_HAT_DOWN]){ //on fait descendre le contour
            i++;
            if(i>4){
                i=0;
            }
            in.joyhat[joueur][SDL_HAT_DOWN]=0;
        }
        else if(in.joyhat[joueur][SDL_HAT_UP]){ //on fait monter le contour
            i--;
            if(i<0){
                i=4;
            }
            in.joyhat[joueur][SDL_HAT_UP]=0;
        }
        else if(in.joybutton[joueur][START]){ //on a appuyer sur la touche start, on quitte le menu Pause
            in.joybutton[joueur][START]=0;
            return;
        }
        else if(in.joybutton[joueur][BX]){ //bouton X, on quitte le menu Pause
            in.joybutton[joueur][BX]=0;
            return;
        }
        else if(in.joybutton[joueur][BA]){ //Bouton A, on selectionne un des boutons
            in.joybutton[joueur][BA]=0;
            if(i==0){ //on veut quitter le menu Pause
                *option=0;
                return;
            }
            else if(i==1){ //on veut rentrer dans les options
                *option=1;
                MenuOption(C, joueur);
                return;
            }
            else{ //on veut aller dans un des menus precedents
                *option=0;
                *precedent=5-i;
                return;
            }
        }
        else{}
        SDL_RenderCopy(C.sdlRenderer, C.T_menu[MENU_PAUSE], NULL, &menu);
        SDL_RenderCopy(C.sdlRenderer, C.T_menu[CONTOUR_PAUSE], NULL, &(contour[i]));
        SDL_RenderPresent(C.sdlRenderer);
    }
}

void MenuOption(CONTEXT C, int joueur){
    SDL_RenderClear(C.sdlRenderer);

    int i=0;
    Input in;
    mise_a_zero_input(&in);
    SDL_Rect menu[3],contour[3];

    SDL_QueryTexture(C.T_menu[BOUTON_AIDE],NULL,NULL,&(menu[0].w),&(menu[0].h));
    SDL_QueryTexture(C.T_menu[BOUTON_COMMANDES],NULL,NULL,&(menu[1].w),&(menu[1].h));
    SDL_QueryTexture(C.T_menu[BOUTON_QUITTER],NULL,NULL,&(menu[2].w),&(menu[2].h));

    SDL_QueryTexture(C.T_menu[CONTOUR_MENU], NULL, NULL, &(contour[0].w), &(contour[0].h));
    SDL_QueryTexture(C.T_menu[CONTOUR_MENU], NULL, NULL, &(contour[1].w), &(contour[1].h));
    SDL_QueryTexture(C.T_menu[CONTOUR_MENU], NULL, NULL, &(contour[2].w), &(contour[2].h));

    menu[0].x=(int)((C.Xres-menu[0].w)/2-70);      menu[0].y=(int)((C.Yres-menu[0].h)/2-300);
    menu[1].x=(int)((C.Xres-menu[1].w)/2-70);      menu[1].y=(int)((C.Yres-menu[1].h)/2-150);
    menu[2].x=(int)((C.Xres-menu[2].w)/2-70);      menu[2].y=(int)((C.Yres-menu[2].h)/2);

    contour[0].x=menu[0].x;                contour[0].y=menu[0].y;
    contour[1].x=menu[1].x;                contour[1].y=menu[1].y;
    contour[2].x=menu[2].x;                contour[2].y=menu[2].y;

    SDL_RenderCopy(C.sdlRenderer, C.T_menu[BOUTON_AIDE], NULL, &menu[0]);
    SDL_RenderCopy(C.sdlRenderer, C.T_menu[BOUTON_COMMANDES], NULL, &menu[1]);
    SDL_RenderCopy(C.sdlRenderer, C.T_menu[BOUTON_QUITTER], NULL, &menu[2]);
    SDL_RenderCopy(C.sdlRenderer, C.T_menu[CONTOUR_MENU], NULL, &(contour[0]));
    SDL_RenderPresent(C.sdlRenderer);

    while(1){
        UpdateEventsW(&in);
        if(in.joyhat[joueur][SDL_HAT_DOWN]){ //on fait descendre le contour
            i++;
            if(i>2){
                i=0;
            }
            in.joyhat[joueur][SDL_HAT_DOWN]=0;
        }
        else if(in.joyhat[joueur][SDL_HAT_UP]){ //on fait monter le contour
            i--;
            if(i<0){
                i=2;
            }
            in.joyhat[joueur][SDL_HAT_UP]=0;
        }
        else if(in.joybutton[joueur][BA]){ //Bouton A, on selectionne un des boutons
            in.joybutton[joueur][BA]=0;
            if(i==0){ //on veut aller dans le menu aide
                MenuAide(C,joueur);
                SDL_RenderClear(C.sdlRenderer);
            }
            else if(i==1){ //on veut rentrer dans le menu des commandes
            }
            else{ //on veut quitter le menu option
                return;
            }
        }
        else{}
        SDL_RenderCopy(C.sdlRenderer, C.T_menu[BOUTON_AIDE], NULL, &menu[0]);
        SDL_RenderCopy(C.sdlRenderer, C.T_menu[BOUTON_COMMANDES], NULL, &menu[1]);
        SDL_RenderCopy(C.sdlRenderer, C.T_menu[BOUTON_QUITTER], NULL, &menu[2]);
        SDL_RenderCopy(C.sdlRenderer, C.T_menu[CONTOUR_MENU], NULL, &(contour[i]));
        SDL_RenderPresent(C.sdlRenderer);

    }
}

void MenuAide(CONTEXT C, int joueur){
    int NB_LIGNE=15;

    SDL_Color couleurBlanche = {255, 255, 255, 255};
    SDL_Surface *texte[NB_LIGNE];
    SDL_Rect position[NB_LIGNE];

    int i;
    Input in;
    mise_a_zero_input(&in);
    char aide[NB_LIGNE][100];

    sprintf(aide[0],"Commandes pour les menus :");
    sprintf(aide[1],"Faire defiler: croix directionnelle");
    sprintf(aide[2],"Choisir : bouton A");
    sprintf(aide[3],"Revenir en arriere : bouton X");
    sprintf(aide[4]," ");
    sprintf(aide[5],"Commandes pour le jeu :");
    sprintf(aide[6],"Accelerer : bouton RT");
    sprintf(aide[7],"Reculer : bouton LT");
    sprintf(aide[8],"Tourner : joystick GAUCHE (appuyer sur Y pour plus de details)");
    sprintf(aide[9],"Freiner : bouton A");
    sprintf(aide[10],"Deraper : bouton RB");
    sprintf(aide[11],"Menu pause : bouton START");
    sprintf(aide[12],"Changer la perspective du jeu : croix directionnelle (appuyer sur B pour plus de details)");
    sprintf(aide[13]," ");
    sprintf(aide[14],"Appuyez sur X pour quitter cette fenetre.");

    for(i=0;i<NB_LIGNE-1;i++){
        texte[i]=TTF_RenderText_Blended(C.police,aide[i],couleurBlanche);
        position[i].x=10;
        position[i].y=20*i+10;
    }

    texte[NB_LIGNE-1]=TTF_RenderText_Blended(C.police,aide[NB_LIGNE-1],couleurBlanche);
    position[NB_LIGNE-1].x=10;
    position[NB_LIGNE-1].y=20*NB_LIGNE+10;

    SDL_FillRect(C.ecran, NULL, SDL_MapRGB(C.ecran->format, 0, 0, 0));
    for(i=0;i<NB_LIGNE;i++){
        SDL_BlitSurface(texte[i],NULL,C.ecran,position+i);
    }
    SDL_UpdateTexture(C.sdlTexture, NULL, C.ecran->pixels, C.ecran->pitch);

    SDL_RenderClear(C.sdlRenderer);
    SDL_RenderCopy(C.sdlRenderer, C.sdlTexture, NULL, NULL);
    SDL_RenderPresent(C.sdlRenderer);

    while(1){
        UpdateEventsW(&in);
        if(in.joyadded){ //rajout d'une manette
            if(C.nbjoystick!=SDL_NumJoysticks()){
                ReallocationJoystick(&C);
            }
            in.joyadded=0;
        }
        if(in.joyremoved){ //retrait d'une manette
            if(C.nbjoystick!=SDL_NumJoysticks()){
                ReallocationJoystick(&C);
            }
            in.joyremoved=0;
        }
        if(in.joybutton[joueur][BX]){ //bouton X, on quitte le menu Pause
            in.joybutton[joueur][BX]=0;
            return;
        }
        if(in.joybutton[joueur][BB]){ //bouton B, on veut plus de details
            in.joybutton[joueur][BB]=0;

            DetailPerspective(C,joueur);
            SDL_RenderClear(C.sdlRenderer);
            SDL_RenderCopy(C.sdlRenderer, C.sdlTexture, NULL, NULL);
            SDL_RenderPresent(C.sdlRenderer);
        }
        if(in.joybutton[joueur][BY]){ //bouton Y, on veut plus de details
            in.joybutton[joueur][BY]=0;

            ExplicationControle(C,joueur);
            SDL_RenderClear(C.sdlRenderer);
            SDL_RenderCopy(C.sdlRenderer, C.sdlTexture, NULL, NULL);
            SDL_RenderPresent(C.sdlRenderer);
        }
    }
}

void DetailPerspective(CONTEXT C, int joueur){
    Input in;
    mise_a_zero_input(&in);

    SDL_RenderClear(C.sdlRenderer);
    SDL_Rect position;
    SDL_QueryTexture(C.T_menu[MENU_PERSPECTIVE],NULL,NULL,&(position.w),&(position.h));

    position.x=(int)(C.Xres-position.w)/2;
    position.y=(int)(C.Yres-position.h)/2;

    SDL_RenderCopy(C.sdlRenderer,C.T_menu[MENU_PERSPECTIVE], NULL, &position);
    SDL_RenderPresent(C.sdlRenderer);

    while(1){
        UpdateEventsW(&in);
        if(in.joybutton[joueur][BX]){ //bouton X, on quitte le menu Pause
            in.joybutton[joueur][BX]=0;
            return;
        }
    }
}

void ExplicationControle(CONTEXT C, int joueur){
    int i;
    Input in;
    mise_a_zero_input(&in);

    SDL_Rect position[6];
    SDL_QueryTexture(C.T_menu[EXPLICATION_1], NULL, NULL, &(position[0].w), &(position[0].h));
    SDL_QueryTexture(C.T_menu[EXPLICATION_2], NULL, NULL, &(position[1].w), &(position[1].h));
    SDL_QueryTexture(C.T_menu[EXPLICATION_3], NULL, NULL, &(position[2].w), &(position[2].h));
    SDL_QueryTexture(C.T_menu[EXPLICATION_4], NULL, NULL, &(position[3].w), &(position[3].h));
    SDL_QueryTexture(C.T_menu[EXPLICATION_5], NULL, NULL, &(position[4].w), &(position[4].h));
    SDL_QueryTexture(C.T_menu[EXPLICATION_6], NULL, NULL, &(position[5].w), &(position[5].h));

    for(i=0;i<6;i++){
        position[i].x=0;
        position[i].y=0;
    }
    i=0;

    while(1){
        UpdateEventsW(&in);
        if(in.joybutton[joueur][BA]&&i!=5){
            in.joybutton[joueur][BA]=0;
            i++;
        }
        if(in.joybutton[joueur][BX]&&i==5){
            in.joybutton[joueur][BX]=0;
            return;
        }
        SDL_RenderClear(C.sdlRenderer);
        SDL_RenderCopy(C.sdlRenderer, C.T_menu[EXPLICATION_1+i], NULL, position+i);
        SDL_RenderPresent(C.sdlRenderer);
    }

}
