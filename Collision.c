#include "Fonctions.h" ok

int CollisionCercles(Cercle C1,Cercle C2){
   int d2=(C1.x-C2.x)*(C1.x-C2.x)+(C1.y-C2.y)*(C1.y-C2.y); //distance au carre entre les centres des deux cercles

   if (d2>(C1.rayon+C2.rayon)*(C1.rayon+C2.rayon))
        return 0; //pas de collision
   else
        return 1; //collision
}

void PointApresRotation(Point *point,Point centre,double angle){
    double C=cos(M_PI*angle/180), S=sin(M_PI*angle/180);

    (*point).x=((*point).x-centre.x)*C-((*point).y-centre.y)*S+centre.x;
    (*point).y=((*point).x-centre.x)*S+((*point).y-centre.y)*C+centre.y;
}

int CollisionPointCercle(Point point,Cercle C){
    //distance au carre entre le poin et le centre du cercle
   double d2 = (point.x-C.x)*(point.x-C.x) + (point.y-C.y)*(point.y-C.y);
   if (d2>C.rayon*C.rayon) //si il est a l'ext du cercle
        return 0;
   else //si il est a l'int du cercle
        return 1;
}

int CollisionDroiteCercle(Point A,Point B,Cercle C){
   Vecteur u;
   u.x = B.x - A.x;
   u.y = B.y - A.y;
   Vecteur AC;
   AC.x = C.x - A.x;
   AC.y = C.y - A.y;
   double numerateur = u.x*AC.y - u.y*AC.x;   // norme du vecteur v
   if (numerateur <0)
        numerateur = -numerateur;   // valeur absolue ; si c'est negatif, on prend l'oppose.
   double denominateur = sqrt(u.x*u.x + u.y*u.y);  // norme de u
   double CI = numerateur / denominateur;
   if (CI<C.rayon)
        return 1;
   else
        return 0;
}

int CollisionSegmentCercle(Point A,Point B,Cercle C){
   if (CollisionDroiteCercle(A,B,C) == 0)
        return 0;  // si on ne touche pas la droite, on ne touchera jamais le segment
   Vecteur AB,AC,BC;
   AB.x = B.x - A.x;
   AB.y = B.y - A.y;
   AC.x = C.x - A.x;
   AC.y = C.y - A.y;
   BC.x = C.x - B.x;
   BC.y = C.y - B.y;
   double pscal1 = AB.x*AC.x + AB.y*AC.y;  // produit scalaire
   double pscal2 = (-AB.x)*BC.x + (-AB.y)*BC.y;  // produit scalaire
   if (pscal1>=0 && pscal2>=0)
        return 1;   // I entre A et B, ok.
   // derniere possibilite, A ou B dans le cercle
   if (CollisionPointCercle(A,C)==1)
        return 1;
   if (CollisionPointCercle(B,C)==1)
        return 1;
   return 0;
}

int CollisionRectangleCercle(POSITION rectangle,Cercle cercle){
    Cercle R;
    R.x=rectangle.x+rectangle.w/2; R.y=rectangle.y+rectangle.h/2; R.rayon=rectangle.h/2;
    if(CollisionCercles(R,cercle)==0){ //si meme les cercles ne se touchent pas
        return 0; //pas besoin d'essayer d'etre plus precis
    }

    Point HG,HD,BG,BD,centre; //les sommets du rectangle avant rotation, sommet Haut Gauche, Haut Droit ...
    DefinitionRectangle(&HG,&HD,&BD,&BG,&centre,rectangle);

    if(CollisionSegmentCercle(HG,HD,cercle)==1) return 1;
    if(CollisionSegmentCercle(HD,BD,cercle)==1) return 2;
    if(CollisionSegmentCercle(BD,BG,cercle)==1) return 3;
    if(CollisionSegmentCercle(BG,HG,cercle)==1) return 4;
    return 0;
}

int CollisionDroiteSeg(Point A,Point B,Point O,Point P){
  Vecteur AO,AP,AB;
  AB.x = B.x - A.x;
  AB.y = B.y - A.y;
  AP.x = P.x - A.x;
  AP.y = P.y - A.y;
  AO.x = O.x - A.x;
  AO.y = O.y - A.y;
  if ((AB.x*AP.y - AB.y*AP.x)*(AB.x*AO.y - AB.y*AO.x)<0)
     return 1;
  else
     return 0;
}

int CollisionSegSeg(Point A,Point B,Point O,Point P){
  if (CollisionDroiteSeg(A,B,O,P)==0)
     return 0;  // inutile d'aller plus loin si le segment [OP] ne touche pas la droite (AB)
  if (CollisionDroiteSeg(O,P,A,B)==0)
     return 0;
  return 1;
}

int CollisionRectangles(POSITION rectangle_av,POSITION rectangle_ap,POSITION rectangle_im){
    Point centre_av,centre_ap,centre_im;
    centre_av.x=rectangle_av.x+rectangle_av.w/2; centre_av.y=rectangle_av.y+rectangle_av.h/2;
    centre_ap.x=rectangle_ap.x+rectangle_ap.w/2; centre_ap.y=rectangle_ap.y+rectangle_ap.h/2;
    centre_im.x=rectangle_im.x+rectangle_im.w/2; centre_im.y=rectangle_im.y+rectangle_im.h/2;

    Cercle Cap,Cim;
    //Cav.x=centre_av.x; Cav.y=centre_av.y; Cav.rayon=rectangle_av.h/2;
    Cap.x=centre_ap.x; Cap.y=centre_ap.y; Cap.rayon=rectangle_ap.h/2;
    Cim.x=centre_im.x; Cim.y=centre_im.y; Cim.rayon=rectangle_im.h/2;

    if(CollisionCercles(Cap,Cim)==0){ //si meme les cercles ne se touchent pas, pas besoin de plus de precision
        return 0;
    }

    Point HGav,HDav,BGav,BDav; //les sommets du rectangle avant rotation, sommet Haut Gauche, Haut Droit ...
    DefinitionRectangle(&HGav,&HDav,&BDav,&BGav,&centre_av,rectangle_av);

    Point HGap,HDap,BGap,BDap; //les sommets du rectangle avant rotation, sommet Haut Gauche, Haut Droit ...
    DefinitionRectangle(&HGap,&HDap,&BDap,&BGap,&centre_ap,rectangle_ap);

    Point HGim,HDim,BGim,BDim; //les sommets du rectangle avant rotation, sommet Haut Gauche, Haut Droit ...
    DefinitionRectangle(&HGim,&HDim,&BDim,&BGim,&centre_im,rectangle_im);

    if(CollisionSegmentRectangle(HGav,HGap,HGim,HDim,BDim,BGim)==1) return 1;
    if(CollisionSegmentRectangle(HDav,HDap,HGim,HDim,BDim,BGim)==1) return 1;
    if(CollisionSegmentRectangle(BDav,BDap,HGim,HDim,BDim,BGim)==1) return 1;
    if(CollisionSegmentRectangle(BGav,BGap,HGim,HDim,BDim,BGim)==1) return 1;
    return 0;
}

int CollisionSegmentRectangle(Point O, Point P, Point HGim, Point HDim, Point BDim, Point BGim){
    if(CollisionSegSeg(HGim,HDim,O,P)==1) return 1;
    if(CollisionSegSeg(HDim,BDim,O,P)==1) return 1;
    if(CollisionSegSeg(BDim,BGim,O,P)==1) return 1;
    if(CollisionSegSeg(BGim,HGim,O,P)==1) return 1;
    return 0;
}

int IntersectionSegments(Point A,Point B,Point I,Point P){
   Vecteur D,E;
   D.x = B.x - A.x;
   D.y = B.y - A.y;
   E.x = P.x - I.x;
   E.y = P.y - I.y;
   double denom = D.x*E.y - D.y*E.x;
   if (denom==0)
       return -1;   // erreur, cas limite
   double t = - (A.x*E.y-I.x*E.y-E.x*A.y+E.x*I.y) / denom;
   if (t<0 || t>=1)
      return 0;
   double u = - (-D.x*A.y+D.x*I.y+D.y*A.x-D.y*I.x) / denom;
   if (u<0 || u>=1)
      return 0;
   return 1;
}

int IntersectionSegmentRectangle(Point A, Point B, Point HG, Point HD, Point BD, Point BG){
    if(IntersectionSegments(A,B,HG,HD)==1) return 1;
    if(IntersectionSegments(A,B,HD,BD)==1) return 1;
    if(IntersectionSegments(A,B,BD,BG)==1) return 1;
    if(IntersectionSegments(A,B,BG,HG)==1) return 1;
    return 0;
}

int IntersectionRectangles(POSITION rectangle1,POSITION rectangle2){
    Point centre1,centre2;
    Cercle C1,C2;
    centre1.x=rectangle1.x+rectangle1.w/2; centre1.y=rectangle1.y+rectangle1.h/2;
    centre2.x=rectangle2.x+rectangle2.w/2; centre2.y=rectangle2.y+rectangle2.h/2;
    C1.x=centre1.x; C1.y=centre1.y; C1.rayon=rectangle1.h/2;
    C2.x=centre2.x; C2.y=centre2.y; C2.rayon=rectangle2.h/2;

    if(CollisionCercles(C1,C2)==0){ //si meme les cercles ne se touchent pas, pas besoin de plus de precision
        return 0;
    }

    Point HG1,HD1,BG1,BD1; //les sommets du rectangle avant rotation, sommet Haut Gauche, Haut Droit ...
    DefinitionRectangle(&HG1,&HD1,&BD1,&BG1,&centre1,rectangle1);

    Point HG2,HD2,BG2,BD2; //les sommets du rectangle avant rotation, sommet Haut Gauche, Haut Droit ...
    DefinitionRectangle(&HG2,&HD2,&BD2,&BG2,&centre2,rectangle2);

    if(IntersectionSegmentRectangle(HG1,HD1,HG2,HD2,BD2,BG2)==1) return 1;
    if(IntersectionSegmentRectangle(HD1,BD1,HG2,HD2,BD2,BG2)==1) return 1;
    if(IntersectionSegmentRectangle(BD1,BG1,HG2,HD2,BD2,BG2)==1) return 1;
    if(IntersectionSegmentRectangle(BG1,HG1,HG2,HD2,BD2,BG2)==1) return 1;
    return 0;
}

void DefinitionRectangle(Point *HG,Point *HD, Point *BD, Point *BG,Point* centre,POSITION rectangle){
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

void DefinitionCercle(Cercle *C, POSITION cercle){
    C->x=(int)(cercle.x+cercle.w/2);
    C->y=(int)(cercle.y+cercle.h/2);
    C->rayon=(int)(cercle.h/2);

}
