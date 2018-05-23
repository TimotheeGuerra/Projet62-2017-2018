#include "Fonctions.h"

int CollisionCercles(CERCLE C1,CERCLE C2){
    int d2=(C1.x-C2.x)*(C1.x-C2.x)+(C1.y-C2.y)*(C1.y-C2.y); //distance au carre entre les centres des deux cercles

    if(d2>(C1.rayon+C2.rayon)*(C1.rayon+C2.rayon)){ //si la distance est plus grande que la somme des rayons au carre
        return 0; //pas de collision
    }
    else{
        return 1; //collision
    }
}

void PointApresRotation(POINT *point,POINT centre,double angle){
    double C=cos(M_PI*angle/180), S=sin(M_PI*angle/180);

    (*point).x=((*point).x-centre.x)*C-((*point).y-centre.y)*S+centre.x;
    (*point).y=((*point).x-centre.x)*S+((*point).y-centre.y)*C+centre.y;
}

int CollisionPointCercle(POINT point,CERCLE C){
    //distance au carre entre le point et le centre du cercle
    double d =(point.x-C.x)*(point.x-C.x)+(point.y-C.y)*(point.y-C.y);
    if (d>C.rayon*C.rayon){ //si il est a l'ext du cercle
        return 0;
    }
    else{ //si il est a l'int du cercle
        return 1;
    }
}

int CollisionDroiteCercle(POINT A,POINT B,CERCLE C){
    VECTEUR u;
    u.x=B.x-A.x;
    u.y=B.y-A.y;

    VECTEUR AC;
    AC.x=C.x-A.x;
    AC.y=C.y-A.y;

    double numerateur=u.x*AC.y-u.y*AC.x;   // norme du vecteur v
    if (numerateur<0){
        numerateur=-numerateur;   // valeur absolue ; si c'est negatif, on prend l'oppose.
    }

    double denominateur=sqrt(u.x*u.x+u.y*u.y);  // norme de u
    double CI=numerateur/denominateur;
    if(CI<C.rayon){
        return 1;
    }
    else{
        return 0;
    }
}

int CollisionSegmentCercle(POINT A,POINT B,CERCLE C){
    if(CollisionDroiteCercle(A,B,C)==0){
        return 0;  // si on ne touche pas la droite, on ne touchera jamais le segment
    }

    VECTEUR AB,AC,BC;
    AB.x=B.x-A.x;
    AB.y=B.y-A.y;
    AC.x=C.x-A.x;
    AC.y=C.y-A.y;
    BC.x=C.x-B.x;
    BC.y=C.y-B.y;

    double pscal1=AB.x*AC.x+AB.y*AC.y;  // produit scalaire
    double pscal2=(-AB.x)*BC.x+(-AB.y)*BC.y;  // produit scalaire
    if (pscal1>=0&&pscal2>=0){
        return 1;   // I entre A et B, ok.
    }

    // derniere possibilite, A ou B dans le cercle
    if (CollisionPointCercle(A,C)==1){
        return 1;
    }
    if (CollisionPointCercle(B,C)==1){
        return 1;
    }

    return 0;
}

int CollisionRectangleCercle(POSITION rectangle,CERCLE cercle){
    CERCLE R;
    R.x=rectangle.x+rectangle.w/2; R.y=rectangle.y+rectangle.h/2; R.rayon=rectangle.h/2;
    if(CollisionCercles(R,cercle)==0){ //si meme les cercles ne se touchent pas
        return 0; //pas besoin d'essayer d'etre plus precis
    }

    POINT HG,HD,BG,BD,centre; //les sommets du rectangle avant rotation, sommet Haut Gauche, Haut Droit ...
    DefinitionRectangle(&HG,&HD,&BD,&BG,&centre,rectangle);

    if(CollisionSegmentCercle(HG,HD,cercle)==1) return 1;
    if(CollisionSegmentCercle(HD,BD,cercle)==1) return 2;
    if(CollisionSegmentCercle(BD,BG,cercle)==1) return 3;
    if(CollisionSegmentCercle(BG,HG,cercle)==1) return 4;
    return 0;
}

void DefinitionRectangle(POINT *HG,POINT *HD, POINT *BD, POINT *BG,POINT* centre,POSITION rectangle){
    (*HG).x=rectangle.x;                   (*HG).y=rectangle.y;
    (*HD).x=rectangle.x+rectangle.w;       (*HD).y=rectangle.y;
    (*BG).x=rectangle.x;                   (*BG).y=rectangle.y+rectangle.h;
    (*BD).x=rectangle.x+rectangle.w;       (*BD).y=rectangle.y+rectangle.h;
    (*centre).x=rectangle.x+rectangle.w/2; (*centre).y=rectangle.y+rectangle.h/2;

    PointApresRotation(HG,*centre,rectangle.rotation);
    PointApresRotation(HD,*centre,rectangle.rotation);
    PointApresRotation(BG,*centre,rectangle.rotation);
    PointApresRotation(BD,*centre,rectangle.rotation);
}

void DefinitionCercle(CERCLE *C, POSITION cercle){
    C->x=(int)(cercle.x+cercle.w/2);
    C->y=(int)(cercle.y+cercle.h/2);
    C->rayon=(int)(cercle.h/2);
}
