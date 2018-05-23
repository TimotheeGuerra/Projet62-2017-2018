#include "Fonctions.h"



int gestion_position_joueur(INPUT *in, POSITION *voiture, int numero){
    if(in->joyhat[numero][SDL_HAT_DOWN]){
        voiture->position_joueur=DERRIERE;
        in->joyhat[numero][SDL_HAT_DOWN]=0;
        return 1;
    }
    else if(in->joyhat[numero][SDL_HAT_LEFT]){
        voiture->position_joueur=GAUCHE;
        in->joyhat[numero][SDL_HAT_LEFT]=0;
        return 1;
    }
    else if(in->joyhat[numero][SDL_HAT_RIGHT]){
        voiture->position_joueur=DROITE;
        in->joyhat[numero][SDL_HAT_RIGHT]=0;
        return 1;
    }
    else if(in->joyhat[numero][SDL_HAT_UP]){
        voiture->position_joueur=DEVANT;
        in->joyhat[numero][SDL_HAT_UP]=0;
        return 1;
    }
    else{
        return 0;
    }

    return 0;
}

void gestion_derriere(POSITION *voiture, double *angle_voulu, int x, int y){
    if(abs(x)<1500&&abs(y)<1500){ //si le joystick n'est pas utilise
        *angle_voulu=voiture->rotation; //on garde la meme orientation
        return;
    }
    else if(x>0){ //un peu compliquer a expliquer sans schema
        if(y>0){
            *angle_voulu=180-(180*atan(x/y)/M_PI);
        }
        else if(y<0){
            *angle_voulu=-(180*atan(x/y)/M_PI);
        }
        else{
            *angle_voulu=90;
        }
    }
    else if(x<0){
        if(y>0){
            *angle_voulu=180-(180*atan(x/y)/M_PI);
        }
        else if(y<0){
            *angle_voulu=360-(180*atan(x/y)/M_PI);
        }
        else{
            *angle_voulu=270;
        }
    }
    else if(x==0){
        if(y>0){
            *angle_voulu=180;
        }
        else if(y<0){
            *angle_voulu=0;
        }
        else{
            *angle_voulu=voiture->rotation;
        }
    }
    else{}

    if(*angle_voulu==360){
        *angle_voulu=0;
    }
}

void gestion_devant(POSITION *voiture, double *angle_voulu, int x, int y){
    if(abs(x)<1500&&abs(y)<1500){ //si le joystick n'est pas utilise
        *angle_voulu=voiture->rotation; //on garde la meme orientation
        return;
    }
    else if(x>0){ //un peu compliquer a expliquer sans schema
        if(y>0){
            *angle_voulu=360-(180*atan(x/y)/M_PI);
        }
        else if(y<0){
            *angle_voulu=180-(180*atan(x/y)/M_PI);
        }
        else{
            *angle_voulu=270;
        }
    }
    else if(x<0){
        if(y>0){
            *angle_voulu=-(180*atan(x/y)/M_PI);
        }
        else if(y<0){
            *angle_voulu=180-(180*atan(x/y)/M_PI);
        }
        else{
            *angle_voulu=90;
        }
    }
    else if(x==0){
        if(y>0){
            *angle_voulu=0;
        }
        else if(y<0){
            *angle_voulu=180;
        }
        else{
            *angle_voulu=voiture->rotation;
        }
    }
    else{}

    if(*angle_voulu==360){
        *angle_voulu=0;
    }
}

void gestion_gauche(POSITION *voiture, double *angle_voulu, int x, int y){
    if(abs(x)<1500&&abs(y)<1500){ //si le joystick n'est pas utilise
        *angle_voulu=voiture->rotation; //on garde la meme orientation
        return;
    }
    else if(x>0){ //un peu compliquer a expliquer sans schema
        if(y>0){
            *angle_voulu=180+(180*atan(y/x)/M_PI);
        }
        else if(y<0){
            *angle_voulu=180+(180*atan(y/x)/M_PI);
        }
        else{
            *angle_voulu=180;
        }
    }
    else if(x<0){
        if(y>0){
            *angle_voulu=360+(180*atan(y/x)/M_PI);
        }
        else if(y<0){
            *angle_voulu=+(180*atan(y/x)/M_PI);
        }
        else{
            *angle_voulu=0;
        }
    }
    else if(x==0){
        if(y>0){
            *angle_voulu=270;
        }
        else if(y<0){
            *angle_voulu=90;
        }
        else{
            *angle_voulu=voiture->rotation;
        }
    }
    else{}

    if(*angle_voulu==360){
        *angle_voulu=0;
    }
}

void gestion_droite(POSITION *voiture, double *angle_voulu, int x, int y){
    if(abs(x)<1500&&abs(y)<1500){ //si le joystick n'est pas utilise
        *angle_voulu=voiture->rotation; //on garde la meme orientation
        return;
    }
    else if(x>0){ //un peu compliquer a expliquer sans schema
        if(y>0){
            *angle_voulu=+(180*atan(y/x)/M_PI);
        }
        else if(y<0){
            *angle_voulu=360+(180*atan(y/x)/M_PI);
        }
        else{
            *angle_voulu=0;
        }
    }
    else if(x<0){
        if(y>0){
            *angle_voulu=180+(180*atan(y/x)/M_PI);
        }
        else if(y<0){
            *angle_voulu=180+(180*atan(y/x)/M_PI);
        }
        else{
            *angle_voulu=180;
        }
    }
    else if(x==0){
        if(y>0){
            *angle_voulu=90;
        }
        else if(y<0){
            *angle_voulu=270;
        }
        else{
            *angle_voulu=voiture->rotation;
        }
    }
    else{}

    if(*angle_voulu==360){
        *angle_voulu=0;
    }
}

void gestion_instruction_joystick(CONTEXT *C,INPUT *in,POSITION *voiture,double* angle_voulu,int numero,int* pause){
    int x,y;
    int force;
    if(in->joyadded){ //un joystick a ete rajoute
        ReallocationJoystick(C);
        in->joyadded=0;
    }
    if(in->joyremoved){ //un joystick a ete enleve
        C->nbjoystick-=1;
        in->joyremoved=0;
    }

    if(gestion_position_joueur(in, voiture, numero)){
        Mix_VolumeChunk(C->sons[SON_CHANGEMENT_POSITION_J], MIX_MAX_VOLUME); //definie le volume sonore pour le son
        Mix_PlayChannel(CANAL_CHANGEMENT_POSITION_J, C->sons[SON_CHANGEMENT_POSITION_J], 0); //joue le son sur le canal 0, le fait 0+1 fois
    }

    if(in->joybutton[numero][START]){ //Est ce que l'utilisateur a presse le Bouton start, veut-il mettre en pause le jeu ?
        *pause=1;
        in->joybutton[numero][START]=0;
    }

    if(in->joybutton[numero][voiture->manette.commande_jeu[COMMANDE_J_FRE]]){ //Est ce que l'utilisateur a presse le Bouton A, veut-il freiner ?
        voiture->freinage=1;
        voiture->force_moteur=0;
        //in->joybutton[0][0]=0;
    }

    //recuperation de la force_moteur demande par l'utilisateur
    if(in->joyaxes[numero][voiture->manette.commande_jeu[COMMANDE_J_ACC]-100].utilise==0&&in->joyaxes[numero][voiture->manette.commande_jeu[COMMANDE_J_ACC]-100].valeur==0){//securite pour le debut du jeu ou les axes sont a 0 et pas a -32768
        voiture->force_moteur=0;
    }
    else{
        force=in->joyaxes[numero][voiture->manette.commande_jeu[COMMANDE_J_ACC]-100].valeur; //on recupere la valeur de l'axe 5 de la manette de l'utilisateur
        if(force==VALEUR_J_MIN){ //l'utilisateur n'accelere pas
            voiture->force_moteur=0;
        }
        else{ //l'utilisateur accelere
            voiture->force_moteur=FORCE_MOTEUR_MAX*(force-VALEUR_J_MIN)/(VALEUR_J_MAX-VALEUR_J_MIN); //sans le facteur, va de 0 a 1
            voiture->freinage=0;
        }
    }

    if(voiture->force_moteur==0){ //si la voiture n'avance pas, peut-etre que l'utilisateur veut reculer
        if(in->joyaxes[numero][voiture->manette.commande_jeu[COMMANDE_J_REC]-100].utilise==0&&in->joyaxes[numero][voiture->manette.commande_jeu[COMMANDE_J_REC]-100].valeur==0){ //securite
            voiture->force_moteur=0;
        }
        else{
            force=in->joyaxes[numero][voiture->manette.commande_jeu[COMMANDE_J_REC]-100].valeur;
            if(force==VALEUR_J_MIN){
                voiture->force_moteur=0;
            }
            else{
                voiture->force_moteur=(FORCE_MOTEUR_MAX/20)*(-1*force+VALEUR_J_MIN)/(VALEUR_J_MAX-VALEUR_J_MIN); //sans le facteur, va de 0 a -1
                voiture->freinage=0;
            }
        }
    }


    if(!in->joybutton[numero][voiture->manette.commande_jeu[COMMANDE_J_DER]]){ //on ne veut pas deraper
        if(voiture->derapage==0){
            voiture->derapage_av=0;
        }
        voiture->derapage=0;
    }
    else if(in->joybutton[numero][voiture->manette.commande_jeu[COMMANDE_J_DER]]){ //l'utilisateur veut deraper
        if(voiture->derapage==1){ //si on etait deja entrain de deraper
            voiture->derapage_av=1; //on le signale
        }
        else{
            voiture->derapage_av=0;
        }
        voiture->derapage=1;
    }

    //on recupere les informations du joystick fait pour tourner de la manette de l'utilisateur
    x=in->joyaxes[numero][voiture->manette.commande_jeu[COMMANDE_J_TOU]-101].valeur; //axe horizontale
    y=in->joyaxes[numero][voiture->manette.commande_jeu[COMMANDE_J_TOU]-100].valeur; //axe verticale

    //on les traduit en un angle comprehensible par l'ordinateur
    if(voiture->vitesse!=0){
        if(voiture->position_joueur==DERRIERE){
            gestion_derriere(voiture,angle_voulu,x,y);
        }
        else if(voiture->position_joueur==GAUCHE){
            gestion_gauche(voiture,angle_voulu,x,y);
        }
        else if(voiture->position_joueur==DROITE){
            gestion_droite(voiture,angle_voulu,x,y);
        }
        else if(voiture->position_joueur==DEVANT){
            gestion_devant(voiture,angle_voulu,x,y);
        }
        else{}
    }
    else{ //si la voiture n'a pas de vitesse
        *angle_voulu=voiture->rotation; //on l'empeche de tourner
    }
}
