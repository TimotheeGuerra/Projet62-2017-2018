#include "Fonctions.h"


double calcul_rotation_joystick(double angle_voulu,POSITION *voiture){
    double virage; //la constante qu'on va rajoute a l'angle de la voiture pour atteindre celui attendu
    //double virage=5-0.5*voiture->vitesse;

    if(voiture->derapage==0){ //on ne derape pas
        virage=80/(10+abs_double(voiture->vitesse));
    }
    else{ //on derape
        virage=100/(10+abs_double(voiture->vitesse));
    }

    if(voiture->rotation>=360){ //on veut un angle entre 0 et 360 pas plus ni moins
        voiture->rotation-=360;
    }
    if(voiture->rotation<0){
        voiture->rotation+=360;
    }

    if((voiture->derapage==1||voiture->derapage_av==1)&&voiture->vitesse!=0){
        voiture->rotation+=voiture->virage;
        return voiture->virage;
    }

    if(abs_double(angle_voulu-voiture->rotation)<virage){ //si la difference entre l'angle actuel et
        voiture->rotation=angle_voulu;     //celui demande est inferieur au nombre rajoute a l'angle
        return 0;                            //on force la valeur de l'angle de la voiture
    }

    if(angle_voulu==voiture->rotation){ //si l'angle actuel est celui demande, on ne fait rien
        return 0;
    }

    if(voiture->rotation<180){ //faire un schema pour comprendre la suite ^^
        if(angle_voulu>voiture->rotation&&angle_voulu<=voiture->rotation+180){
            voiture->rotation+=virage;
            voiture->virage=virage;
            return virage;
        }
        else{
            voiture->rotation-=virage;
            voiture->virage=-virage;
            return -virage;
        }
    }
    if(voiture->rotation>=180){
        if(angle_voulu>=voiture->rotation-180&&angle_voulu<voiture->rotation){
            voiture->rotation-=virage;
            voiture->virage=-virage;
            return -virage;
        }
        else{
            voiture->rotation+=virage;
            voiture->virage=virage;
            return virage;
        }
    }
    return virage;
}

void calcul_vitesse_joystick(POSITION *voiture,double F_pente, double F_frottement_solide, double virage){
    if(abs_double(voiture->vitesse)<0.01&&voiture->force_moteur==0){ //on n'accelere plus et notre vitesse est tres faible
        voiture->vitesse=0; //stop
    }

    if(voiture->derapage==1){ //si on derape, on calcul la vitesse autrement
        if(voiture->vitesse==0){
            voiture->vitesse=0;
            return;
        }
        voiture->vitesse=(DELTA/MASSE)*((2000/FORCE_MOTEUR_MAX)*voiture->force_moteur+F_pente-F_frottement_solide)+(1-FROTTEMENT_FLUIDE*DELTA/MASSE)*voiture->vitesse; //principe fondamental de la dynamique
        if(abs_double(voiture->vitesse)<0.1){
            voiture->vitesse=0;
            if(virage>0){
                voiture->rotation+=40; //on ajoute veritablement les 40° a la voiture pour quelle continue sur sa lancee
            }
            else{
                voiture->rotation-=40;
            }
        }
        return;
    }

    if(voiture->freinage==0){ //l'utilisateur ne veut pas freiner
        //on utilise donc le PFD pour calculer la nouvelle vitesse
        voiture->vitesse=(DELTA/MASSE)*(voiture->force_moteur+F_pente-F_frottement_solide)+(1-FROTTEMENT_FLUIDE*DELTA/MASSE)*voiture->vitesse; //principe fondamental de la dynamique
    }
    if(voiture->freinage==1){ //l'utilisateur veut freiner
        if(voiture->vitesse>0){
            double F_freinage=-FORCE_FREINAGE; //force constante de freinage
            voiture->vitesse=(DELTA/MASSE)*(F_freinage+F_pente-F_frottement_solide)+(1-FROTTEMENT_FLUIDE*DELTA/MASSE)*voiture->vitesse; //principe fondamental de la dynamique
            if (voiture->vitesse<0.01){  //si la vitesse est inferieur a une "petite" valeur
                voiture->vitesse=0;     //on la fixe
                voiture->force_moteur=0; //on "arrete" le moteur
                voiture->freinage=0; //on ne veut plus freiner
            }
        }
        if(voiture->vitesse<0){
            double F_freinage=FORCE_FREINAGE;
            voiture->vitesse=(DELTA/MASSE)*(F_freinage+F_pente-F_frottement_solide)+(1-FROTTEMENT_FLUIDE*DELTA/MASSE)*voiture->vitesse; //principe fondamental de la dynamique
            if (voiture->vitesse>-0.01){
                voiture->vitesse=0;
                voiture->force_moteur=0;
                voiture->freinage=0;
            }
        }
    }
}

void calcul_position_joystick(POSITION* voiture){
    //calcule de la nouvelle position de la voiture
    voiture->x+=voiture->vitesse*sin(M_PI*voiture->rotation/180); //un peu de trigonometrie
    voiture->y-=voiture->vitesse*cos(M_PI*voiture->rotation/180);
}

int calcul_saut_joystick(CONTEXT C,POSITION *voiture, double beta, SDL_Rect *positionV){
    POINT centre, avant;
    avant.x=positionV->x+voiture->w/2;  avant.y=positionV->y;
    centre.x=positionV->x+voiture->w/2; centre.y=positionV->y+voiture->h/2;
    int x,y;
    if(voiture->rebondi!=0||voiture->compteur_rebond!=0){ //si on rebondi on ne saute pas.
        voiture->saut.etat=0;
        voiture->alpha=beta;
        positionV->h=voiture->h;
        positionV->w=voiture->w;
        return 0;
    }

    if(voiture->saut.etat==0){ //on ne saute pas encore
        if((voiture->alpha-beta)*180/M_PI<20){ //si la difference de pentes entre les deux position n'est pas suffisante pour un saut
            voiture->saut.etat=0; //on ne saute toujours pas
            voiture->alpha=beta;
            positionV->h=voiture->h;
            positionV->w=voiture->w;
            return 0;
        }
        else{ //sinon on peut possiblement sauter
            int temps_actuel;
            double t_saut=(2*voiture->vitesse/G)*(sin(voiture->alpha)-tan(beta)*cos(voiture->alpha));
            if(t_saut>TEMPS_SAUT_MIN){ //la duree du saut excede 1s, on le prend donc en compte
                temps_actuel=SDL_GetTicks();
                voiture->saut.etat=1;
                voiture->saut.debut_saut=temps_actuel;
                voiture->saut.fin_saut=temps_actuel+t_saut*300;
                voiture->saut.vo=voiture->vitesse;

                PointApresRotation(&avant,centre,voiture->rotation);
                x=(int)(avant.x); y=(int)(avant.y);
                securite_overflowX(C,&x); securite_overflowY(C,&y);
                voiture->saut.zo=C.relief[y][x];

                voiture->force_moteur=0;
                positionV->h=voiture->h;
                positionV->w=voiture->w;
                return 1;
            }
            else{
                voiture->saut.etat=0;
                voiture->alpha=beta;
                positionV->h=voiture->h;
                positionV->w=voiture->w;
                return 0;
            }
        }
    }
    else{ //on est en train de sauter
        int temps_actuel=SDL_GetTicks(), delta=temps_actuel-voiture->saut.debut_saut;

        PointApresRotation(&avant,centre,voiture->rotation);
        x=(int)(avant.x); y=(int)(avant.y);
        securite_overflowX(C,&x); securite_overflowY(C,&y);
        int z_sol=C.relief[y][x]; //on recupere l'altitude du relief

        //on calcul l'altitude de la voiture
        int z_voiture=(int)(-0.5*G*(delta)*(delta)+voiture->saut.vo*sin(voiture->alpha)*(delta)+voiture->saut.zo);
        if(z_voiture==z_sol){ //si on touche le sol, on arrete de sauter
            voiture->saut.etat=0;
            voiture->alpha=beta; //on recupere l'angle de la pente
            positionV->h=voiture->h;
            positionV->w=voiture->w;
            return 0;
        }

        if(temps_actuel>=voiture->saut.fin_saut){ //le saut est fini, on arrete donc de sauter
            voiture->saut.etat=0;
            voiture->alpha=beta;
            positionV->h=voiture->h;
            positionV->w=voiture->w;
            return 0;
        }
        else{ //le saut n'est pas fini, on va donc zoomer la voiture
            double zoom=calcul_zoom(*voiture,temps_actuel);
            if(abs_double(zoom>2)) zoom=2;
            else if(zoom<1) zoom=1;
            voiture->saut.etat=1;
            voiture->force_moteur=0;
            positionV->h=(int)(zoom*voiture->h);
            positionV->w=(int)(zoom*voiture->w);
            return 1;
        }
    }
    return 0;
}

double calcul_zoom(POSITION voiture, int temps_actuel){
    //calcul le zoom suivant une parabole commencant a 1 et finissant a 1 lorsque t=fin_saut
    double a, b, c;
    int debut=voiture.saut.debut_saut, fin=(voiture.saut.fin_saut-debut);
    a=-4.0*(ZOOM_MAX-1)/(fin*fin);
    b=-a*fin;
    c=1;
    return a*(temps_actuel-debut)*(temps_actuel-debut)+b*(temps_actuel-debut)+c;
}

int percute_bord_joystick(CONTEXT C,POSITION *voiture){
    POINT HG,HD,BD,BG; //poitn Haut Gauche, Haut Droit ...
    CERCLE V;
    //attribution des coordonnees des sommets de l'ecran
    HG.x=0;        HG.y=0;
    HD.x=C.Xres-1; HD.y=0;
    BD.x=C.Xres-1; BD.y=C.Yres-1;
    BG.x=0;        BG.y=C.Yres-1;

    DefinitionCercle(&V,*voiture); //definition du cercle lie a la voiture
    if(voiture->vitesse!=0){ //si la vitesse n'est pas nulle
        if(CollisionDroiteCercle(HG,HD,V)==1){ //collision avec le cote du haut
            voiture->y=0;
            voiture->rebondi=-1;
            voiture->compteur_rebond=1;
            if(voiture->vitesse>0){
                //attribution d'une nouvelle rotation a la voiture pour qu'elle rebondisse
                if(voiture->rotation>=270&&voiture->rotation<=360){
                    voiture->vitesse*=abs_double(voiture->rotation-360)*abs_double(voiture->rotation-360)/2000;
                    voiture->rotation=540-voiture->rotation;
                }
                if(voiture->rotation>=0&&voiture->rotation<=90){
                    voiture->vitesse*=abs_double(voiture->rotation-0)*abs_double(voiture->rotation-0)/2000;
                    voiture->rotation=180-voiture->rotation;
                }
            }
            else{
                voiture->rotation=180-voiture->rotation;
            }
            if(abs_double(voiture->vitesse)>VITESSE_MAX){ //empeche la vitesse de monter trop haut grâce au mur
                voiture->vitesse=VITESSE_MAX*abs_double(voiture->vitesse)/voiture->vitesse;
            }
            return 1;
        }
        if(CollisionDroiteCercle(HD,BD,V)==1){ //collision avec le cote droit
            voiture->x=(C.Xres-1)-voiture->h;
            voiture->vitesse*=abs_double(voiture->rotation-90)*abs_double(voiture->rotation-90)/2000;
            if(abs_double(voiture->vitesse)>VITESSE_MAX){
                voiture->vitesse=VITESSE_MAX*abs_double(voiture->vitesse)/voiture->vitesse;
            }
            voiture->rotation=360-voiture->rotation; //attribution d'une nouvelle rotation a la voiture pour qu'elle rebondisse
            return 1;
        }
        if(CollisionDroiteCercle(BD,BG,V)==1){ //collision avec le cote du bas
            voiture->y=(C.Yres-1)-voiture->h;
            voiture->vitesse*=abs_double(voiture->rotation-180)*abs_double(voiture->rotation-180)/2000;
            if(abs_double(voiture->vitesse)>VITESSE_MAX){
                voiture->vitesse=VITESSE_MAX*abs_double(voiture->vitesse)/voiture->vitesse;
            }
            voiture->rotation=180-voiture->rotation; //attribution d'une nouvelle rotation a la voiture pour qu'elle rebondisse
            return 1;
        }
        if(CollisionDroiteCercle(BG,HG,V)==1){ //collision avec le cote gauche
            voiture->x=voiture->h-voiture->w;
            voiture->vitesse*=abs_double(voiture->rotation-270)*abs_double(voiture->rotation-270)/2000;
            if(abs_double(voiture->vitesse)>VITESSE_MAX){
                voiture->vitesse=VITESSE_MAX*abs_double(voiture->vitesse)/voiture->vitesse;
            }
            voiture->rotation=360-voiture->rotation; //attribution d'une nouvelle rotation a la voiture pour qu'elle rebondisse
            return 1;
        }
        return 0;
    }
    return 0;
}

int percute_mur(CONTEXT C,POSITION *voiture, double beta, int joueur){
    if(voiture->rebondi==1&&voiture->compteur_rebond==0){
        gestion_bruit_rebond(C,voiture,joueur);
        if(voiture->rotation>180){
            voiture->rotation-=180;
            voiture->vitesse/=2;
            //voiture->alpha=beta;
        }
        else{
            voiture->rotation+=180;
            voiture->vitesse/=2;
            //voiture->alpha=beta;
        }

        voiture->rebondi=-1;
        voiture->compteur_rebond=1;
        return 1;
    }

    if(voiture->compteur_rebond==3){
        voiture->rebondi=0;
        voiture->compteur_rebond=0;
    }
    else if(voiture->compteur_rebond!=0){
        voiture->compteur_rebond++;
    }
    else{
        voiture->compteur_rebond=0;
    }

    return 0;
}

int percute_voiture_joystick(POSITION *voiture1,POSITION *voiture2,int *collision){
    CERCLE V1,V2;
    DefinitionCercle(&V1,*voiture1);
    DefinitionCercle(&V2,*voiture2);

    *collision=0;
    if(CollisionCercles(V1,V2)==0){ //si meme les grands cercles ne se touchent pas
        return 0; //pas besoin de continuer a essayer d'etre plus precis
    }

    POSITION fictive1,fictive2;
    fictive1=*voiture1;
    fictive2=*voiture2;

    //position que la voiture 1 aura dans le futur si il n'y a pas collision
    fictive1.x+=voiture1->vitesse*sin(M_PI*voiture1->rotation/180);
    fictive1.y-=voiture1->vitesse*cos(M_PI*voiture1->rotation/180);

    //position que la voiture 2 aura dans le futur si il n'y a pas collision
    fictive2.x+=voiture2->vitesse*sin(M_PI*voiture2->rotation/180);
    fictive2.y-=voiture2->vitesse*cos(M_PI*voiture2->rotation/180);

    double Cos1=cos(M_PI*fictive1.rotation/180), Sin1=sin(M_PI*fictive1.rotation/180);
    double Cos2=cos(M_PI*fictive2.rotation/180), Sin2=sin(M_PI*fictive2.rotation/180);

    POINT centre1,centre2;
    centre1.x=fictive1.x+fictive1.w/2; centre1.y=fictive1.y+fictive1.h/2;
    centre2.x=fictive2.x+fictive2.w/2; centre2.y=fictive2.y+fictive2.h/2;

    int diametre=PGCD(voiture1->h,voiture1->w);
    CERCLE approx1[voiture1->h/diametre][voiture1->w/diametre];
    CERCLE approx2[voiture2->h/diametre][voiture2->w/diametre];

    //decomposition des rectangles en un certain nombre de cercles
    for(int i=0;i<voiture1->h/diametre;i++){
        for(int j=0;j<voiture1->w/diametre;j++){
            //position avant rotation de la voiture
            approx1[i][j].x=diametre*(2*j+1)/2+fictive1.x;
            approx1[i][j].y=diametre*(2*i+1)/2+fictive1.y;
            //position apres rotation de la voiture
            approx1[i][j].x=(approx1[i][j].x-centre1.x)*Cos1-(approx1[i][j].y-centre1.y)*Sin1+centre1.x;
            approx1[i][j].y=(approx1[i][j].x-centre1.x)*Sin1+(approx1[i][j].y-centre1.y)*Cos1+centre1.y;
            approx1[i][j].rayon=diametre/2;

            approx2[i][j].x=diametre*(2*j+1)/2+fictive2.x;
            approx2[i][j].y=diametre*(2*i+1)/2+fictive2.y;
            approx2[i][j].x=(approx2[i][j].x-centre2.x)*Cos2-(approx2[i][j].y-centre2.y)*Sin2+centre2.x;
            approx2[i][j].y=(approx2[i][j].x-centre2.x)*Sin2+(approx2[i][j].y-centre2.y)*Cos2+centre2.y;
            approx2[i][j].rayon=diametre/2;
        }
    }

    for(int i=0;i<voiture1->h/diametre;i++){
        for(int j=0;j<voiture1->w/diametre;j++){
            V1=approx1[i][j];
            for(int l=0;l<voiture1->h/diametre;l++){
                for(int k=0;k<voiture1->w/diametre;k++){
                    V2=approx2[l][k];
                    if(CollisionCercles(V1,V2)){
                        double rot=voiture1->rotation;
                        voiture1->rotation=voiture2->rotation;
                        voiture2->rotation=rot;

                        double vitesse;
                        vitesse=(voiture1->vitesse+voiture2->vitesse)/2;
                        vitesse/=1.1;
                        voiture1->vitesse=vitesse;
                        voiture2->vitesse=vitesse;

                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

int percute_objet_joystick(POSITION voiture,SDL_Rect Objet){
    //definition du cercle de collision de l'objet
    CERCLE C;
    C.x=(int)(Objet.x+Objet.w/2);  C.y=(int)(Objet.y+Objet.h/2);
    C.rayon=(int)(Objet.h/2);

    if(CollisionRectangleCercle(voiture,C)!=0){ //si il y a collision entre la voiture et l'objet
        return 1;
    }
    return 0;
}

int gestion_derapage(POSITION *voiture, double virage, double *rot_graphique){
    if(voiture->derapage==1){ //si la voiture est entrain de deraper
        if(voiture->derapage_av==1){ //si elle derapait deja avant
            if(virage>0){ //si on tourne vers la droite
                *rot_graphique+=40; //on ajoute 40° au graphisme de la voiture par rapport a l'angle de la voiture
            }
            else if(virage<0){ //si on tourne vers la droite
                *rot_graphique-=40; //on retranche 40°
            }
        }
        else{ //si on ne derapait pas avant
            if(virage>0){
                *rot_graphique+=20; //on n'ajoute que 20° (decomposition du virage)
            }
            else if(virage<0){
                *rot_graphique-=20;
            }
        }
        return 1;
    }
    else{ //si on ne derape pas
        if(voiture->derapage_av==1){ //si on derapait avant
            if(virage>0){
                voiture->rotation+=40; //on ajoute veritablement les 40° a la voiture pour quelle continue sur sa lancee
                *rot_graphique=voiture->rotation;
            }
            else if(virage<0){
                voiture->rotation-=40;
                *rot_graphique=voiture->rotation;
            }
            else{}
        }
        return 0;
    }

    if(*rot_graphique>=360){ //on veut un angle entre 0 et 360 pas plus ni moins
        *rot_graphique-=360;
    }
    if(*rot_graphique<0){
        *rot_graphique+=360;
    }
}

double force_pente_joystick(CONTEXT C,POSITION *voiture, double *beta){
    int xf,xi,yf,yi;
    POINT Avant, Arriere, Centre; //position de la partie avant et arrière de la voiture
    double Force;
    Avant.x=voiture->x+voiture->w/2;
    Avant.y=voiture->y;
    Arriere.x=voiture->x+voiture->w/2;
    Arriere.y=voiture->y+voiture->h;
    Centre.x=voiture->x+voiture->w/2;
    Centre.y=voiture->y+voiture->h/2;

    PointApresRotation(&Avant,Centre,voiture->rotation);
    PointApresRotation(&Arriere,Centre,voiture->rotation);

    xf=(int)Avant.x;   yf=(int)Avant.y;
    xi=(int)Arriere.x; yi=(int)Arriere.y;
    securite_overflowX(C,&xi); securite_overflowY(C,&yi);
    securite_overflowX(C,&xf); securite_overflowY(C,&yf);

    //Calcul de beta angle de ma pente juste devant la voiture
    *beta= atan2((C.relief[yf][xf]-C.relief[yi][xi]),sqrt((xf-xi)*(xf-xi)+(yf-yi)*(yf-yi)));

    if(*beta-voiture->alpha>RUPTURE_PENTE_MAX){
        voiture->rebondi=1;
    }

    Force=-MASSE*G*sin(*beta)*cos(*beta);
    return Force*3; //le coef 3 est là pour rendre la force non negligeable
}

double force_frottement_solide(POSITION voiture){
    if(abs(voiture.vitesse)>0.01){
        double Cos=cos(voiture.alpha);
        return signe(voiture.vitesse)*FROTTEMENT_SOLIDE*MASSE*G*Cos*Cos;
    }
    return 0;
}

void gestion_globale_voiture(CONTEXT C,POSITION *voiture,SDL_Rect *positionV,double *rot_graphique,double angle_voulu, int joueur){
    double F_pente=0,F_frottement_solide=0,virage=0,beta=0;
    //virage=constante qu'on rajoute a l'angle actuel de la voiture pour quelle atteigne l'angle_voulue
    //beta=angle de la pente devant la voiture

    *rot_graphique=voiture->rotation;
    F_pente=force_pente_joystick(C,voiture,&beta); //beta=angle de la pente
    percute_mur(C,voiture,beta,joueur);
    if(!calcul_saut_joystick(C,voiture,beta,positionV)){ //on ne saute pas
        F_frottement_solide=force_frottement_solide(*voiture);
        virage=calcul_rotation_joystick(angle_voulu,voiture);
        if(gestion_derapage(voiture,virage,rot_graphique)){
            gestion_bruit_derapage(C,voiture,joueur);
        }
        if(voiture->derapage==0&&voiture->derapage_av==1){
            Mix_Pause(joueur+CANAL_DERAPAGE);
            Mix_Resume(CANAL_MOTEUR+joueur); //reprend le canal 1; -1 pour tous
        }
    }
    else{ //on saute, on annule donc les forces liees aux frottements solides et a la pente
        F_frottement_solide=0;
        F_pente=0;
    }
    //on calcul la nouvelle vitesse
    calcul_vitesse_joystick(voiture,F_pente,F_frottement_solide, virage);
    gestion_bruit_moteur(C,voiture,joueur);
}
