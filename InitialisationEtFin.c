#include "Fonctions.h"


void InitAll(CONTEXT* C, int *erreur){
    SDL_DisplayMode resolution;

    Init_rand(); //peremt d'initialiser la fontion random

    //initialisation de la video et des joysticks
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)<0){
        *erreur=-1;
        return;
    }

    //initialisation de l'ecriture
    if(TTF_Init()!=0){
        *erreur=-2;
        return;
    }

    //recuperation de la resolution de l'ecran
    if (SDL_GetDesktopDisplayMode(0,&resolution)!=0) {
        *erreur=-4;
        return;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){
        *erreur=-5;
        return;
    }
    Mix_AllocateChannels(32); //alloue 32 canaux pour les sons, de 0 a 31

    //creation de l'ecran et de la fenetre de rendu pour le GPU
    C->sdlWindow=SDL_CreateWindow("Ma fenetre de jeu", SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , resolution.w, resolution.h, SDL_WINDOW_FULLSCREEN_DESKTOP);
    C->sdlRenderer=SDL_CreateRenderer(C->sdlWindow, -1,  SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(C->sdlRenderer, 0, 0, 0, 255); //ajuste la couleur du fond du rendu

    //resolution de l'ecran
    C->Xres=resolution.w;
    C->Yres=resolution.h;

    //creation d'un ecran fictif pour le CPU
    C->ecran=SDL_CreateRGBSurface(0, C->Xres, C->Yres, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if(C->ecran==NULL){
        *erreur=-3;
        return;
    }

    //creation de la texture, dont le GPU a besoin, associee a l'ecran du CPU
    C->sdlTexture=SDL_CreateTexture(C->sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 640, 480);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // permet d'obtenir les redimensionnements plus doux.

    SDL_JoystickEventState(SDL_ENABLE); //permet la recuperation des evenements lies au joystick

    //charge toutes les images, les joysticks ...
    LoadAll(C,erreur);
    return;
}

void LoadAll(CONTEXT *C,int *erreur){
    int i;

    //different tableau contenant le chemin pour trouver les fichiers images
    char* voiture[]={"Voiture/BlueRaceCar.bmp","Voiture/GreenRaceCar.bmp","Voiture/RedRaceCar.bmp"};
    char* element_jeu[]={"Element/cerise.bmp","Element/cerise_doree.bmp","Element/coupe.bmp",
    "Element/FlagBlue.bmp","Element/FlagGreen.bmp","Element/FlagRed.bmp",
    "Element/BaliseBlue.bmp","Element/BaliseGreen.bmp","Element/BaliseRed.bmp"};
    char* menu[]={"Menu/contour100_34.bmp","Menu/contour310_86.bmp","Menu/contour174_40.bmp",
    "Menu/precedent.bmp","Menu/start.bmp","Menu/quitter.bmp",
    "Menu/joueur1.bmp","Menu/joueur2.bmp","Menu/joueur3.bmp","Menu/avertissement.bmp",
    "Menu/jeucerise.bmp","Menu/jeuflag.bmp","Menu/Pause_227_399.bmp","Menu/jeucourse.bmp",
    "Menu/aide.bmp","Menu/commandes.bmp", "Menu/detailPerspective.bmp",
    "Menu/Explication/1.jpg","Menu/Explication/2.jpg","Menu/Explication/3.jpg",
    "Menu/Explication/4.jpg","Menu/Explication/5.jpg","Menu/Explication/6.jpg"};
    char* circuit[]={"Circuit/relief.bmp","Circuit/circuit_coeur.bmp"};
    char* score[]={"Score/J.bmp","Score/0.bmp","Score/1.bmp","Score/2.bmp","Score/3.bmp",
    "Score/4.bmp","Score/5.bmp","Score/6.bmp","Score/7.bmp","Score/8.bmp","Score/9.bmp",};
    char* son[]={"Bruitage/changement_position_joueur.wav", "Bruitage/racecar.wav",
    "Bruitage/boutton.wav","Bruitage/derapage.wav","Bruitage/crash.WAV","Bruitage/rebond.wav"};
    char* musique[]={"Musique/musique1.mp3","Musique/musique2.mp3"};

    //calcul de differents nombres tres utiles pour la suite
    C->nbvoiture=sizeof(voiture)/sizeof(char*);
    C->nbelement=sizeof(element_jeu)/sizeof(char*);
    C->nbmenu=sizeof(menu)/sizeof(char*);
    C->nbcircuit=sizeof(circuit)/sizeof(char*);
    C->nbscore=sizeof(score)/sizeof(char*);
    C->nbsons=sizeof(son)/sizeof(char*);
    C->nbmusique=sizeof(musique)/sizeof(char*);
    C->nbjoystick=SDL_NumJoysticks();
    C->nbjeu=3;

    //allocation dynamique des tableau contenant les surface (pour le CPU) et les textures associe pour le GPU
    C->S_voiture=malloc(C->nbvoiture*sizeof(SDL_Surface*));
    C->T_voiture=malloc(C->nbvoiture*sizeof(SDL_Texture*));

    C->S_element=malloc(C->nbelement*sizeof(SDL_Surface*));
    C->T_element=malloc(C->nbelement*sizeof(SDL_Texture*));

    C->S_menu=malloc(C->nbmenu*sizeof(SDL_Surface*));
    C->T_menu=malloc(C->nbmenu*sizeof(SDL_Texture*));

    C->S_circuit=malloc(C->nbcircuit*sizeof(SDL_Surface*));
    C->T_circuit=malloc(C->nbcircuit*sizeof(SDL_Texture*));

    C->S_score=malloc(C->nbscore*sizeof(SDL_Surface*));
    C->T_score=malloc(C->nbscore*sizeof(SDL_Texture*));

    C->joystick=malloc(C->nbjoystick*sizeof(SDL_Joystick*));

    C->sons=malloc(C->nbsons*sizeof(Mix_Chunk*));

    C->musique=malloc(C->nbmusique*sizeof(Mix_Music*));

    //gestion des erreurs liees aux allocation dynamiques
    if(C->S_voiture==NULL||C->S_menu==NULL||C->S_circuit==NULL||C->S_element==NULL||C->S_score==NULL){
        *erreur=-10;
        return;
    }
    if(C->T_voiture==NULL||C->T_menu==NULL||C->T_circuit==NULL||C->T_element==NULL||C->T_score==NULL){
        *erreur=-11;
        return;
    }
    if(C->joystick==NULL){
        *erreur=-12;
        return;
    }
    if(C->sons==NULL){
        *erreur=-14;
        return;
    }

    if(C->musique==NULL){
        *erreur=-15;
        return;
    }

    //chargements des images de voitures dans les surfaces et creation des textures associees
    for(i=0;i<C->nbvoiture;i++){
        C->S_voiture[i]=IMG_Load(voiture[i]);
        if(C->S_voiture[i]==NULL){ //gestion des erreurs liees au chargement d'image
            *erreur=-100-i;
            return;
        }
        SDL_SetColorKey(C->S_voiture[i],SDL_TRUE,SDL_MapRGB(C->S_voiture[i]->format,255,0,0)); //defini une couleur transparente (celle du fond des images)
        C->T_voiture[i]=SDL_CreateTextureFromSurface(C->sdlRenderer,C->S_voiture[i]);
        if(C->T_voiture[i]==NULL){ //gestion des erreurs liees a la creation des textures
            *erreur=-100-i;
            return;
        }
    }

    //chargements des images des elements de jeu dans les surfaces et creation des textures associees
    for(i=0;i<C->nbelement;i++){
        C->S_element[i]=IMG_Load(element_jeu[i]);
        if(C->S_element[i]==NULL){
            *erreur=-500-i;
            return;
        }
        SDL_SetColorKey(C->S_element[i],SDL_TRUE,SDL_MapRGB(C->S_element[i]->format,255,255,255));
        C->T_element[i]=SDL_CreateTextureFromSurface(C->sdlRenderer,C->S_element[i]);
        if(C->T_element[i]==NULL){
            *erreur=-500-i;
            return;
        }
    }

    //chargements des images des menus dans les surfaces et creation des textures associees
    for(i=0;i<C->nbmenu;i++){
        C->S_menu[i]=IMG_Load(menu[i]);
        if(C->S_menu[i]==NULL){
            *erreur=-200-i;
            return;
        }
        SDL_SetColorKey(C->S_menu[i],SDL_TRUE,SDL_MapRGB(C->S_menu[i]->format,255,255,255));
        C->T_menu[i]=SDL_CreateTextureFromSurface(C->sdlRenderer,C->S_menu[i]);
        if(C->T_menu[i]==NULL){
            *erreur=-200-i;
            return;
        }
    }

    //chargements des images de circuits dans les surfaces et creation des textures associees
    for(i=0;i<C->nbcircuit;i++){
        C->S_circuit[i]=IMG_Load(circuit[i]);
        if(C->S_circuit[i]==NULL){
            *erreur=-300-i;
            return;
        }
        SDL_SetColorKey(C->S_circuit[i],SDL_TRUE,SDL_MapRGB(C->S_circuit[i]->format,255,0,0));
        C->T_circuit[i]=SDL_CreateTextureFromSurface(C->sdlRenderer,C->S_circuit[i]);
        if(C->T_circuit[i]==NULL){
            *erreur=-300-i;
            return;
        }
    }

    //chargements des images de score dans les surfaces et creation des textures associees
    for(i=0;i<C->nbscore;i++){
        C->S_score[i]=IMG_Load(score[i]);
        if(C->S_score[i]==NULL){
            *erreur=-600-i;
            return;
        }
        SDL_SetColorKey(C->S_score[i],SDL_TRUE,SDL_MapRGB(C->S_score[i]->format,255,255,255));
        C->T_score[i]=SDL_CreateTextureFromSurface(C->sdlRenderer,C->S_score[i]);
        if(C->T_score[i]==NULL){
            *erreur=-600-i;
            return;
        }
    }

    //chargement des joysticks
    for(i=0;i<C->nbjoystick;i++){
        C->joystick[i]=SDL_JoystickOpen(i);
        if(C->joystick[i]==NULL){ //gestion des erreurs liees a l'ouverture des manettes
            *erreur=-400-i;
            return;
        }
    }

    //chargement des sons
    for(i=0;i<C->nbsons;i++){
        C->sons[i]=Mix_LoadWAV(son[i]);
        if(C->sons[i]==NULL){ //gestion des erreurs liees au chargement des sons
            *erreur=-700-i;
            return;
        }
    }

    //chargement des musiques
    for(i=0;i<C->nbmusique;i++){
        C->musique[i]=Mix_LoadMUS(musique[i]);
        if(C->musique[i]==NULL){ //gestion des erreurs liees au chargement des sons
            *erreur=-800-i;
            return;
        }
    }

    //chargement de la police d'ecriture
    C->police=TTF_OpenFont("Timeless.ttf",15);
    if(C->police==NULL){ //gestion des erreurs liees au chargement de la police
        fprintf(stderr,"Impossible de charger \"Timeless.ttf\"");
        *erreur=-13;
        return;
    }
    return;
}

int Release(CONTEXT *C){
    int i;

    //lieration de toutes les surfaces et de toutes les textures
    for(i=0;i<C->nbvoiture;i++){
        SDL_FreeSurface(C->S_voiture[i]);
        SDL_DestroyTexture(C->T_voiture[i]);
    }
    for(i=0;i<C->nbelement;i++){
        SDL_FreeSurface(C->S_element[i]);
        SDL_DestroyTexture(C->T_element[i]);
    }
    for(i=0;i<C->nbmenu;i++){
        SDL_FreeSurface(C->S_menu[i]);
        SDL_DestroyTexture(C->T_menu[i]);
    }
    for(i=0;i<C->nbcircuit;i++){
        SDL_FreeSurface(C->S_circuit[i]);
        SDL_DestroyTexture(C->T_circuit[i]);
    }

    //fermeture des manettes
    for(i=0;i<C->nbjoystick;i++){
        SDL_JoystickClose(C->joystick[i]);
    }
    //liberation de tous les sons charges
    for(i=0;i<C->nbsons;i++){
        Mix_FreeChunk(C->sons[i]);
    }
    //liberation de toutes les musiques charges
    for(i=0;i<C->nbmusique;i++){
        Mix_FreeMusic(C->musique[i]);
    }

    //liberation de toutes les allocations dynamiques
    free(C->S_voiture);free(C->S_menu);free(C->S_circuit);free(C->S_element);
    free(C->T_voiture);free(C->T_menu);free(C->T_circuit);free(C->T_element);
    free(C->joystick);
    free(C->sons);
    free(C->musique);

    //fermeture de la police d'ecriture
    TTF_CloseFont(C->police);

    //liberation de la matrice de relief
    libereMatriceInt(C->relief);


    //destruction des ecrans
    SDL_DestroyTexture(C->sdlTexture);
    SDL_DestroyRenderer(C->sdlRenderer);
    SDL_FreeSurface(C->ecran);
    SDL_DestroyWindow(C->sdlWindow);

    //fermeture de l'audio, de l'ecriture et de la video
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    return 0;
}

int gestion_erreur(CONTEXT C,int erreur){
    FILE *E=fopen("CompteRendu_erreurs.txt","wt");

    int i;
    if(erreur==-1){
        fprintf(E,"erreur d'initialisation video ou joystick!!!\n");
        fclose(E);
        return 1;
    }
    else if(erreur==-2){
        fprintf(E,"erreur d'initialisation TTF!!!\n");
        fclose(E);
        return 1;
    }
    else if(erreur==-3){
        fprintf(E,"erreur d'initialisation de l'ecran!!!\n");
        fclose(E);
        return 1;
    }
    else if(erreur==-4){
        fprintf(E,"Probleme a l'obtention de la resolution de l'ecran!!!\n");
        fclose(E);
        return 1;
    }
    else if(erreur==-5){
        fprintf(E,"erreur d'initialisation de la musique\n");
        fclose(E);
        return 1;
    }

    else if(erreur==-10){
        fprintf(E,"erreur de l'allocation d'un des tableaux de surfaces!!!\n");
        fclose(E);
        return 1;
    }
    else if(erreur==-11){
        fprintf(E,"erreur de l'allocation d'un des tableaux de textures!!!\n");
        fclose(E);
        return 1;
    }
    else if(erreur==-12){
        fprintf(E,"erreur de l'allocation du tableau de joysticks!!!\n");
        fclose(E);
        return 1;
    }
    else if(erreur==-13){
        fprintf(E,"erreur de chargement de la police!!!\n");
        fclose(E);
        return 1;
    }
    else if(erreur==-14){
        fprintf(E,"erreur de l'allocation du tableau de sons!!!\n");
        fclose(E);
        return 1;
    }
    else if(erreur==-15){
        fprintf(E,"erreur de l'allocation du tableau des musiques!!!\n");
        fclose(E);
        return 1;
    }
    else{
        for(i=0;i<C.nbvoiture;i++){
            if(erreur==-100-i){
                fprintf(E,"erreur de chargement de la voiture : %d!!!\n",i);
                fclose(E);
                return 1;
            }
        }
        for(i=0;i<C.nbmenu;i++){
            if(erreur==-200-i){
                fprintf(E,"erreur de chargement du menu : %d!!!\n",i);
                fclose(E);
                return 1;
            }
        }
        for(i=0;i<C.nbcircuit;i++){
            if(erreur==-300-i){
                fprintf(E,"erreur de chargement du circuit : %d!!!\n",i);
                fclose(E);
                return 1;
            }
        }
        for(i=0;i<C.nbjoystick;i++){
            if(erreur==-400-i){
                fprintf(E,"erreur de chargement du joystick : %d!!!\n",i);
                fclose(E);
                return 1;
            }
        }
        for(i=0;i<C.nbelement;i++){
            if(erreur==-500-i){
                fprintf(E,"erreur de chargement de l'element : %d!!!\n",i);
                fclose(E);
                return 1;
            }
        }
        for(i=0;i<C.nbscore;i++){
            if(erreur==-600-i){
                fprintf(E,"erreur de chargement du score : %d!!!\n",i);
                fclose(E);
                return 1;
            }
        }
        for(i=0;i<C.nbscore;i++){
            if(erreur==-700-i){
                fprintf(E,"erreur de chargement du son : %d!!!\n",i);
                fclose(E);
                return 1;
            }
        }
        for(i=0;i<C.nbmusique;i++){
            if(erreur==-800-i){
                fprintf(E,"erreur de chargement de la musique : %d!!!\n",i);
                fclose(E);
                return 1;
            }
        }
        return 0;
    }
}

void Init_rand(){
    //initialise la fonction random
    srand(time(NULL));
}

void mise_a(int valeur,Coordonnee *circuit,int taille){
    //mat des coordonnees a la valeur souhaitee
    int i;
    for(i=0;i<taille;i++){
        circuit[i].X=valeur;
        circuit[i].Y=valeur;
    }
}

int ReallocationJoystick(CONTEXT *C){
    //realoue dynamiquement le tableau des joysticks lors d'un changement
    int i;

    //destruction de l'ancienne matrice des manettes
    for(i=0;i<C->nbjoystick;i++){
        SDL_JoystickClose(C->joystick[i]);
    }
    free(C->joystick);

    //allocation de la nouvelle matrice des manettes
    C->nbjoystick=SDL_NumJoysticks();
    if(C->nbjoystick==0){ //si il n'y a plus de manette, on ne fait rien.
        return 0;
    }
    else{ //si il y a des manettes connectees :
        C->joystick=malloc(C->nbjoystick*sizeof(SDL_Joystick*));
        for(i=0;i<C->nbjoystick;i++){
            C->joystick[i]=SDL_JoystickOpen(i);
            if(C->joystick[i]==NULL){ //gestion des erreurs liees a l'ouverture des manettes
                return -1;
            }
        }
    }
    return C->nbjoystick;
}
