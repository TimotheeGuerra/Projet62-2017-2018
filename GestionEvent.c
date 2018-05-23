#include "Fonctions.h"

void mise_a_zero_input(INPUT* in){
    //met a 0 (ie non utilise) toutes les cases du tableau des events
    int i,j;
    for(i=0;i<SDL_NUM_SCANCODES;i++){
        in->key[i]=0;
    }
    in->joyadded=0;
    in->joyremoved=0;
    in->mousex=0;
    in->mousey=0;
    in->mousexrel=0;
    in->mouseyrel=0;
    for(i=0;i<8;i++){
        in->mousebuttons[i]=0;
    }
    for(i=0;i<4;i++){
        for(j=0;j<7;j++){
            in->joyaxes[i][j].valeur=0;
            in->joyaxes[i][j].utilise=0;
        }
        for(j=0;j<12;j++){
            in->joybutton[i][j]=0;
        }
        for(j=0;j<10;j++){
            in->joyhat[i][j]=0;
        }
    }
    in->quit=0;
}

void UpdateEventsP(INPUT* in){
	in->mousexrel=0; //pour eviter des mouvements fictifs de la souris
	in->mouseyrel=0;

	SDL_Event event;
	while(SDL_PollEvent(&event)){ //tant qu'il y a des evenements dans la fil
        GestionEvents(in,event);
	}
}

void UpdateEventsW(INPUT* in){
    in->mousexrel=0; //pour eviter des mouvements fictifs de la souris
	in->mouseyrel=0;

	SDL_Event event;
	SDL_WaitEvent(&event); //attente d'un evenement
    GestionEvents(in,event);
}

void GestionEvents(INPUT *in, SDL_Event event){
    switch (event.type){ //de quel type est l'evenement regarde
    case SDL_KEYDOWN: //on a presse sur une touche du clavier
        in->key[event.key.keysym.scancode]=1; //on recupere laquelle c'est et on met a 1 sa case
        break;
    case SDL_KEYUP: //on a relache une touche du clavier
        in->key[event.key.keysym.scancode]=0; //on recupere laquelle c'est et on met a 0 sa case
        break;
    case SDL_MOUSEMOTION: //on a bouge la souris
        //recuperation de ses nouvelles coordonnees
        in->mousex=event.motion.x;
        in->mousey=event.motion.y;
        in->mousexrel=event.motion.xrel;
        in->mouseyrel=event.motion.yrel;
        break;
    case SDL_MOUSEBUTTONDOWN: //on a presse sur une touche de la souris
        in->mousebuttons[event.button.button]=1;
        break;
    case SDL_MOUSEBUTTONUP: //on a relache sur une touche de la souris
        in->mousebuttons[event.button.button]=0;
        break;
    case SDL_WINDOWEVENT_CLOSE: //on a appuye sur la croix rouge de fermeture d'une fenetre
        in->quit=1;
        break;
    case SDL_JOYBUTTONDOWN : //on a presse sur une touche d'une manette
        in->joybutton[event.jbutton.which][event.jbutton.button]=1;
        break;
    case SDL_JOYBUTTONUP : //on a relache une touche d'une manette
        in->joybutton[event.jbutton.which][event.jbutton.button]=0;
        break;
    case SDL_JOYAXISMOTION : //on a bouge un axe d'une manette
        in->joyaxes[event.jaxis.which][event.jaxis.axis].valeur=event.jaxis.value;
        in->joyaxes[event.jaxis.which][event.jaxis.axis].utilise=1;
        break;
    case SDL_JOYHATMOTION :  //on a bouge un "hat"/une croix directionnelle d'une manette
        in->joyhat[event.jhat.which][event.jhat.value]=1;
        break;
    case SDL_JOYDEVICEADDED : //on a rajoute une manette
        in->joyadded=1;
        break;
    case SDL_JOYDEVICEREMOVED : //on a enleve une manette
        in->joyremoved=1;
        break;
    default:
        break;
    }
}
