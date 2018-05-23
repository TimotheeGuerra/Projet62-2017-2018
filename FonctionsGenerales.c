#include "Fonctions.h"


int rand_a_b(int a,int b){
    return rand()%(b-a)+a; //renvoi une valeur aleatoire entre a inclus et b exclus
}

int signe(double vitesse){
    //renvoi le signe d'un double
    if(vitesse!=0){
        return (int)(abs(vitesse)/vitesse);
    }
    return 1;
}

int PGCD(int a,int b){
    int r=a%b;
    if(r==0) return b;
    else return PGCD(b,r);
}

void PassagePositionRect(POSITION voiture, SDL_Rect* position){
    position->x=(int)(voiture.x);
    position->y=(int)(voiture.y);
}

void SDL_AffichageCaractere(SDL_Renderer *Renderer, SDL_Texture *Lettre, int x, int y){
    SDL_Rect position;
    position.x=x; position.y=y;
    SDL_QueryTexture(Lettre, NULL, NULL, &(position.w), &(position.h));

    SDL_RenderCopy(Renderer, Lettre, NULL, &position);
}

void SDL_AffichageScore(CONTEXT C, int *score){
    int i;
    for(i=0;i<C.nbjoueur;i++){
        SDL_AffichageCaractere(C.sdlRenderer,C.T_score[LETTRE_J],10,H_SCORE*i+10); //affichage du J de joueur
        SDL_AffichageCaractere(C.sdlRenderer,C.T_score[DEBUT_NOMBRE+i],W_SCORE+8,H_SCORE*i+10); //affichage du numero du joueur

        SDL_AffichageCaractere(C.sdlRenderer,C.T_score[DEBUT_NOMBRE+score[i]],2*W_SCORE+16,H_SCORE*i+10); //affichage du score
    }
}

double abs_double(double x){
    if(x<0) return -x;
    return x;
}

void securite_overflowX(CONTEXT C,int* position){
    if(*position<0) *position=0;
    else if(*position>C.Xres-1) *position=C.Xres-1;
    else{}
}

void securite_overflowY(CONTEXT C,int* position){
    if(*position<0) *position=0;
    else if(*position>C.Yres-1) *position=C.Yres-1;
    else{}
}

char* AffichageCommande(int commande){
    switch(commande){
    case BA:
        return "bouton A";
    case BB:
        return "bouton B";
    case BY:
        return "bouton Y";
    case BX:
        return "bouton X";
    case L1:
        return "bouton LB";
    case R1:
        return "bouton RB";
    case LT:
        return "bouton LT";
    case RT:
        return "bouton RT";
    case L3:
        return "bouton L3";
    case R3:
        return "bouton R3";
    case BACK:
        return "bouton back";
    case START:
        return "bouton start";
    case CROIX:
        return "croix directionnel";
    case J_DROIT:
        return "joystick droit";
    case J_GAUCHE:
        return "joystick gauche";
    case -1:
        return "commande a definir";
    default:
        return "erreur";
    }
}

int ChoixNouvelleCommande(COMMANDE *manette, int joueur, int numero_surlignage){

    if(numero_surlignage==2){ //on veut changer l'attribution de l'acceleration
        if(manette->commande_jeu[COMMANDE_J_ACC]==RT){
            manette->commande_jeu[COMMANDE_J_ACC]=LT;
            manette->commande_jeu[COMMANDE_J_REC]=RT;
            return -1;
        }
        else{
            manette->commande_jeu[COMMANDE_J_ACC]=RT;
            manette->commande_jeu[COMMANDE_J_REC]=LT;
            return -1;
        }
    }
    else if(numero_surlignage==3){ //on veut changer l'attribution de l'acceleration
        if(manette->commande_jeu[COMMANDE_J_REC]==RT){
            manette->commande_jeu[COMMANDE_J_REC]=LT;
            manette->commande_jeu[COMMANDE_J_ACC]=RT;
            return -1;
        }
        else{
            manette->commande_jeu[COMMANDE_J_REC]=RT;
            manette->commande_jeu[COMMANDE_J_ACC]=LT;
            return -1;
        }
    }
    else if(numero_surlignage==4){
        if(manette->commande_jeu[COMMANDE_J_TOU]==J_GAUCHE){
            manette->commande_jeu[COMMANDE_J_TOU]=J_DROIT;
            return -1;
        }
        else{
            manette->commande_jeu[COMMANDE_J_TOU]=J_GAUCHE;
            return -1;
        }
    }

    INPUT in;
    mise_a_zero_input(&in);

    while(!in.key[SDL_SCANCODE_ESCAPE]){
        UpdateEventsW(&in);
        if(numero_surlignage==0||numero_surlignage==1){ //ligne pour commande des menu : choisir et precedent
            if(ChoixNouveauBoutonMenu(&in, manette, joueur, numero_surlignage+1)){
                return numero_surlignage+1;
            }
        }
        if(numero_surlignage==5||numero_surlignage==6){
            if(ChoixNouveauBoutonJeu(&in,manette,joueur,numero_surlignage-2)){
                return numero_surlignage-2;
            }
        }

    }
    return 0;
}

int ChoixNouveauBoutonMenu(INPUT *in, COMMANDE *manette, int joueur, int indice_touche){
    if(in->joybutton[joueur][BA]){
        in->joybutton[joueur][BA]=0;
        manette->commande_menu[indice_touche]=BA;
        return 1;
    }
    else if(in->joybutton[joueur][BX]){
        in->joybutton[joueur][BX]=0;
        manette->commande_menu[indice_touche]=BX;
        return 1;
    }
    else if(in->joybutton[joueur][BB]){
        in->joybutton[joueur][BB]=0;
        manette->commande_menu[indice_touche]=BB;
        return 1;
    }
    else if(in->joybutton[joueur][BY]){
        in->joybutton[joueur][BY]=0;
        manette->commande_menu[indice_touche]=BY;
        return 1;
    }
    else if(in->joybutton[joueur][L1]){
        in->joybutton[joueur][L1]=0;
        manette->commande_menu[indice_touche]=L1;
        return 1;
    }
    else if(in->joybutton[joueur][R1]){
        in->joybutton[joueur][R1]=0;
        manette->commande_menu[indice_touche]=R1;
        return 1;
    }
    else{
        return 0;
    }
    return 0;
}

int ChoixNouveauBoutonJeu(INPUT *in, COMMANDE *manette, int joueur, int indice_touche){
    if(in->joybutton[joueur][BA]){
        in->joybutton[joueur][BA]=0;
        manette->commande_jeu[indice_touche]=BA;
        return 1;
    }
    else if(in->joybutton[joueur][BX]){
        in->joybutton[joueur][BX]=0;
        manette->commande_jeu[indice_touche]=BX;
        return 1;
    }
    else if(in->joybutton[joueur][BB]){
        in->joybutton[joueur][BB]=0;
        manette->commande_jeu[indice_touche]=BB;
        return 1;
    }
    else if(in->joybutton[joueur][BY]){
        in->joybutton[joueur][BY]=0;
        manette->commande_jeu[indice_touche]=BY;
        return 1;
    }
    else if(in->joybutton[joueur][L1]){
        in->joybutton[joueur][L1]=0;
        manette->commande_jeu[indice_touche]=L1;
        return 1;
    }
    else if(in->joybutton[joueur][R1]){
        in->joybutton[joueur][R1]=0;
        manette->commande_jeu[indice_touche]=R1;
        return 1;
    }
    else{
        return 0;
    }
    return 0;
}

int VerificationDouble(COMMANDE *manette, int tableau, int indice_touche){
    int i;

    if(indice_touche==-1){ //pas besoin de verification
        return 0;
    }

    if(tableau==TABLEAU_MENU){
        for(i=1;i<3;i++){
            if(i!=indice_touche){
                if(manette->commande_menu[i]==manette->commande_menu[indice_touche]){
                    manette->commande_menu[i]=-1;
                    return 1;
                }
            }
        }
    }
    if(tableau==TABLEAU_JEU){
        for(i=3;i<5;i++){
            if(i!=indice_touche){
                if(manette->commande_jeu[i]==manette->commande_jeu[indice_touche]){
                    manette->commande_jeu[i]=-1;
                    return 1;
                }
            }
        }
    }
    return 0;
}

void EgalisationManette(COMMANDE *change, COMMANDE manette){
    printf("%d\n",change->commande_menu[COMMANDE_M_CHOISIR]);
    change->commande_menu[COMMANDE_M_CHOISIR]=manette.commande_menu[COMMANDE_M_CHOISIR];
    printf("%d\n",change->commande_menu[COMMANDE_M_CHOISIR]);

    change->commande_menu[COMMANDE_M_DEFFI]=manette.commande_menu[COMMANDE_M_DEFFI];
    change->commande_menu[COMMANDE_M_PRE]=manette.commande_menu[COMMANDE_M_PRE];

    change->commande_jeu[COMMANDE_J_ACC]=manette.commande_jeu[COMMANDE_J_ACC];
    change->commande_jeu[COMMANDE_J_DER]=manette.commande_jeu[COMMANDE_J_DER];
    change->commande_jeu[COMMANDE_J_FRE]=manette.commande_jeu[COMMANDE_J_FRE];
    change->commande_jeu[COMMANDE_J_PAU]=manette.commande_jeu[COMMANDE_J_PAU];
    change->commande_jeu[COMMANDE_J_PERS]=manette.commande_jeu[COMMANDE_J_PERS];
    change->commande_jeu[COMMANDE_J_REC]=manette.commande_jeu[COMMANDE_J_REC];
    change->commande_jeu[COMMANDE_J_TOU]=manette.commande_jeu[COMMANDE_J_TOU];
}
