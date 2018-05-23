#include "Fonctions.h"


#define SCORE_MAX_FLAG 3


void deroulement_flag_joystick(CONTEXT *C,int *precedent){
    SDL_Rect positionV[C->nbjoueur], positionFlag[C->nbjoueur],initialFlag[C->nbjoueur], positionBalise[C->nbjoueur], positionCoupe,positionGagnant;  //les positions des differents objets dont la SDL a besoin pour l'affichage
    POSITION voiture[C->nbjoueur],initial[C->nbjoueur]; //contient toutes les donnees utiles sur les voitures et leurs positions initiales
    INFOSFLAG flag[C->nbjoueur]; //contient toutes les donnees concernant les flags
    //flag[0] fait reference au flag qu'a la voiture du joueur 0

    int i,pause=0,option=0,collision=0;
    int temps_actuel=0,temps_precedent=0; //tous les differents temps dont on a besoin
    int score[C->nbjoueur]; for(i=0;i<C->nbjoueur;i++) score[i]=0; //tableau des scores des joueurs initialise a 0
    double angle_voulu=0,rot_graphique[C->nbjoueur]; //les differents angles dont on a besoin

    //permet de recuperer la taille de l'image
    for(i=0;i<C->nbjoueur;i++){
        InitFlag(flag+i);
        InitVoiture(voiture+i);InitVoiture(initial+i);
        SDL_QueryTexture(C->T_voiture[i], NULL, NULL, &(positionV[i].w), &(positionV[i].h));
        SDL_QueryTexture(C->T_element[i+DEBUT_FLAG], NULL, NULL, &(positionFlag[i].w), &(positionFlag[i].h));
        SDL_QueryTexture(C->T_element[i+DEBUT_BALISE], NULL, NULL, &(positionBalise[i].w), &(positionBalise[i].h));
        voiture[i].h=positionV[i].h;
        voiture[i].w=positionV[i].w;
    }
    SDL_QueryTexture(C->T_element[ELEMENT_TROPHEE], NULL, NULL, &(positionCoupe.w), &(positionCoupe.h));
    SDL_QueryTexture(C->T_voiture[0], NULL, NULL, &(positionGagnant.w), &(positionGagnant.h));
    positionCoupe.x=(C->Xres-positionCoupe.w)/2;   positionCoupe.y=(C->Yres-positionCoupe.h)/2;
    positionGagnant.x=positionCoupe.x+MILIEU_COUPE_X-positionGagnant.w/2;
    positionGagnant.y=positionCoupe.y+MILIEU_COUPE_Y-positionGagnant.h/2;

    //initialisation de variables
    affectation_aleatoire_flag(*C,positionFlag,initialFlag,positionBalise);
    affectation_voiture(*C,voiture,positionFlag,initial,positionV);

    INPUT in; //tableau contenant tous les evenements utiles
    mise_a_zero_input(&in);

    while(!in.key[SDL_SCANCODE_ESCAPE]){ //tant que l'utilisateur n'a pas appuye sur la touche echap
        for(i=0;i<C->nbjoueur;i++){ //regarde si un joueur a atteint le score max
            if(score[i]>=SCORE_MAX_FLAG){//si oui,affiche la coupe et retourne au menu jeu
                Mix_Pause(-1); //met en pause tous les canaux

                SDL_RenderClear(C->sdlRenderer); //destruction de l'ancienne ecran
                SDL_AffichageScore(*C,score); //fonction creee permettant d'afficher les scores grâces a des images BMP
                SDL_RenderCopy(C->sdlRenderer,C->T_element[ELEMENT_TROPHEE],NULL,&positionCoupe);
                SDL_RenderCopy(C->sdlRenderer,C->T_voiture[i],NULL,&positionGagnant);
                SDL_RenderPresent(C->sdlRenderer); //rafraichit l'ecran

                Mix_VolumeChunk(C->sons[SON_VICTOIRE],MIX_MAX_VOLUME); //definie le volume sonore pour le son
                Mix_PlayChannel(CANAL_VICTOIRE, C->sons[SON_VICTOIRE], 0);
                SDL_Delay(4000); //attente de 4s
                *precedent=3;
                return;
            }
        }
        UpdateEventsP(&in);  //on recupere tous les evenements
        temps_actuel=SDL_GetTicks(); //on recupere le temps actuel
        if(temps_actuel-temps_precedent>TEMPS){ //si on a attendu suffisamment, on fait bouger la voiture

            //gestion de toutes les voitures une par une
            for(i=0;i<C->nbjoueur;i++){
                //gestion des instructions
                gestion_instruction_joystick(C,&in,&voiture[i],&angle_voulu,i,&pause); //on gere les demandes de l'utilisateur
                if(pause==1){ //si il a appuye sur start.
                    pause=0;
                    MenuPause(C,voiture+i,precedent,&option,i); //gestion du menu pause
                    if(*precedent!=0){ //il a demande a quitter le jeu
                        return;
                    }
                    *precedent=3;
                }

                //gestion de la physique et de la voiture
                gestion_globale_voiture(*C,voiture+i,positionV+i,rot_graphique+i,angle_voulu,i);

                //gestion des collisions
                if(percute_bord_joystick(*C,voiture+i)){
                    gestion_bruit_rebond(*C,voiture,i);
                }
                for(int j=0;j<C->nbjoueur;j++){
                    if(i==j){
                        if(percute_objet_joystick(voiture[i],positionBalise[i])){ //on percute sa propre balise
                            if(flag[i].dequi!=-1){ //on a un drapeau ennemi
                                flag[flag[i].dequi].pris=0;flag[flag[i].dequi].parqui=-1;
                                positionFlag[flag[i].dequi].x=initialFlag[flag[i].dequi].x;
                                positionFlag[flag[i].dequi].y=initialFlag[flag[i].dequi].y;
                                flag[i].dequi=-1;
                                score[i]++;
                            }
                        }
                    }
                    else{
                        if(percute_objet_joystick(voiture[i],positionFlag[j])){ //est ce qu'on percute un flag ennemi
                            flag[i].dequi=j;
                            flag[j].pris=1; flag[j].parqui=i;
                        }

                        if(percute_voiture_joystick(&voiture[i],&voiture[j],&collision)){ //si il y a collision entre deux voitures
                            //elles perdent les deux leur drapeau (si elles en avaient un)
                            if(flag[i].dequi!=-1){
                                positionFlag[flag[i].dequi].x=initialFlag[flag[i].dequi].x;
                                positionFlag[flag[i].dequi].y=initialFlag[flag[i].dequi].y;
                                flag[i].dequi=-1;
                            }
                            if(flag[j].dequi!=-1){
                                positionFlag[flag[j].dequi].x=initialFlag[flag[j].dequi].x;
                                positionFlag[flag[j].dequi].y=initialFlag[flag[j].dequi].y;
                                flag[j].dequi=-1;
                            }
                            gestion_bruit_crash(*C, voiture+i, i);
                        }
                    }
                }

                //nouvelle position de la voiture
                if(collision==0){ //si il ne va pas y avoir collision
                    calcul_position_joystick(&voiture[i]);
                }
                positionV[i].x=(int)voiture[i].x; positionV[i].y=(int)voiture[i].y; //nouvelle position de la voiture
                if(flag[i].dequi!=-1){
                    positionFlag[flag[i].dequi].x=positionV[i].x; positionFlag[flag[i].dequi].y=positionV[i].y;
                }
            }
            //rafraichissement du temps ecoule
            temps_precedent=temps_actuel;
        }
        else{ //si on a pas attendu suffisament, on endort le programme pour eviter qu'il ne consomme 100% du CPU
            SDL_Delay(TEMPS-(temps_actuel-temps_precedent));
        }

        //affichage de certaines valeurs

        //affichage a l'ecran de toutes les images
        SDL_RenderClear(C->sdlRenderer); //nettoye l'ecran des textures precedentes

        SDL_AffichageScore(*C,score);
        SDL_RenderCopy(C->sdlRenderer, C->T_circuit[0], NULL, NULL); //affiche le decor
        for(i=0;i<C->nbjoueur;i++){ //affiche toutes les balises
            SDL_RenderCopy(C->sdlRenderer, C->T_element[i+DEBUT_BALISE], NULL, &(positionBalise[i])); //affiche les flags
        }
        for(i=0;i<C->nbjoueur;i++){ //affiche toutes les voitures
            SDL_RenderCopyEx(C->sdlRenderer,C->T_voiture[i],NULL,&positionV[i],rot_graphique[i],NULL,SDL_FLIP_NONE); //fait tourner la texture dans le sens horaire.
        }
        for(i=0;i<C->nbjoueur;i++){ //affiche tous les drapeaux
            SDL_RenderCopy(C->sdlRenderer, C->T_element[i+DEBUT_FLAG], NULL, &(positionFlag[i])); //affiche les flags
        }
        SDL_RenderPresent(C->sdlRenderer); //rafraichit l'ecran
    }
}

void InitFlag(INFOSFLAG *flag){
    flag->dequi=-1;
    flag->pris=0;
    flag->parqui=-1;
}

void affectation_aleatoire_flag(CONTEXT C, SDL_Rect* positionFlag, SDL_Rect* initialFlag, SDL_Rect* positionBalise){
    if(C.nbjoueur==1){ //il n'y a qu'un joueur, le flag peut etre n'importe où sur l'ecran
        positionFlag->x=rand_a_b(50,C.Xres-50);
        positionFlag->y=rand_a_b(50,C.Yres-50);
        *initialFlag=*positionFlag;
        *positionBalise=*positionFlag;

        return;
    }

    if(C.nbjoueur==2){ //il y a deux joueurs, on divise l'ecran en 2 rectangles egaux, les flags
    //peuvent etre n'importe où dans ce rectangle.
        positionFlag->x=rand_a_b(50,250);
        positionFlag->y=rand_a_b(50,C.Yres-50);
        *initialFlag=*positionFlag;
        *positionBalise=*positionFlag;

        (positionFlag+1)->x=rand_a_b(C.Xres-250,C.Xres-50);
        (positionFlag+1)->y=rand_a_b(50,C.Yres-50);
        *(initialFlag+1)=*(positionFlag+1);
        *(positionBalise+1)=*(positionFlag+1);

        return;
    }
    if(C.nbjoueur==3){ //il y a trois joueurs, on divise l'ecran en 3 rectangles egaux, les flags
    //peuvent etre n'importe où dans ce rectangle.
        positionFlag->x=rand_a_b(50,200);
        positionFlag->y=rand_a_b(50,200);
        *initialFlag=*positionFlag;
        *positionBalise=*positionFlag;

        (positionFlag+1)->x=rand_a_b(C.Xres-250,C.Xres-50);
        (positionFlag+1)->y=rand_a_b(50,200);
        *(initialFlag+1)=*(positionFlag+1);
        *(positionBalise+1)=*(positionFlag+1);

        (positionFlag+2)->x=rand_a_b((C.Xres/2)-100,(C.Xres/2)+100);
        (positionFlag+2)->y=rand_a_b(C.Yres-250,C.Yres-50);
        *(initialFlag+2)=*(positionFlag+2);
        *(positionBalise+2)=*(positionFlag+2);

        return;
    }
}

void affectation_voiture(CONTEXT C, POSITION* voiture, SDL_Rect *positionFlag, POSITION *initial, SDL_Rect *positionV){
    if(C.nbjoueur==1){
        //la voiture se trouve sur le drapeau
        voiture->x=positionFlag->x+positionFlag->w/2-voiture->w/2;
        voiture->y=positionFlag->y+positionFlag->h/2-voiture->h/2;
        voiture->rotation=rand_a_b(0,360);
        *(initial)=*(voiture);
        PassagePositionRect(*voiture,positionV);
        return;
    }

    if(C.nbjoueur==2){
        //la voiture se trouve a droite du drapeau
        voiture->x=positionFlag->x+positionFlag->w/2-voiture->w/2+50;
        voiture->y=positionFlag->y+positionFlag->h/2-voiture->h/2;
        voiture->rotation=90;
        *(initial)=*(voiture);
        PassagePositionRect(*voiture,positionV);

        //la voiture se trouve a gauche du drapeau
        (voiture+1)->x=(positionFlag+1)->x+(positionFlag+1)->w/2-(voiture+1)->w/2-50;
        (voiture+1)->y=(positionFlag+1)->y+(positionFlag+1)->h/2-(voiture+1)->h/2;
        (voiture+1)->rotation=270;
        *(initial+1)=*(voiture+1);
        PassagePositionRect(*(voiture+1),positionV+1);
        return;
    }
    if(C.nbjoueur==3){
        //la voiture se trouve a droite du drapeau
        voiture->x=positionFlag->x+positionFlag->w/2-voiture->w/2+50;
        voiture->y=positionFlag->y+positionFlag->h/2-voiture->h/2;
        voiture->rotation=90;
        *(initial)=*(voiture);
        PassagePositionRect(*voiture,positionV);

        //la voiture se trouve a gauche du drapeau
        (voiture+1)->x=(positionFlag+1)->x+(positionFlag+1)->w/2-(voiture+1)->w/2-50;
        (voiture+1)->y=(positionFlag+1)->y+(positionFlag+1)->h/2-(voiture+1)->h/2;
        (voiture+1)->rotation=270;
        *(initial+1)=*(voiture+1);
        PassagePositionRect(*(voiture+1),positionV+1);

        //la voiture se trouve au dessus du drapeau
        (voiture+2)->x=(positionFlag+2)->x+(positionFlag+2)->w/2-(voiture+2)->w/2;
        (voiture+2)->y=(positionFlag+2)->y+(positionFlag+2)->h/2-(voiture+2)->h/2-50;
        (voiture+2)->rotation=0;
        *(initial+2)=*(voiture+2);
        PassagePositionRect(*(voiture+2),positionV+2);
        return;
    }
}
