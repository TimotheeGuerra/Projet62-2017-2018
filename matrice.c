#include<stdlib.h>
#include<stdio.h>
#include<SDL2/SDL.h>
#include"Fonctions.h"
#define MAXINT 65535

unsigned int** alloueMatriceInt(int nl,int nc){
  int i;
  unsigned int** p = calloc(nl,sizeof(*p));
  *p = calloc(nl*nc,sizeof(**p));
  for(i=1;i<nl;i++){
    p[i]=p[i-1]+nc;
  }
  return p;
}

void libereMatrice(void** matrice){
  free(*matrice);
  free(matrice);
}

unsigned int trouveMaximunI(unsigned int ** matrice, int nl, int nc) {
	int i, j;
	unsigned int max = matrice[0][0];
	for (i = 0; i < nl; i++){
		for (j = 0; j < nc; j++){
			if (matrice[i][j] > max) max = matrice[i][j];
		}
	}
	return max;
}

unsigned int trouveMinimunI(unsigned int** matrice, int nl, int nc) {
	int i, j;
	unsigned int min = matrice[0][0];
	for (i = 0; i < nl; i++) {
		for (j = 0; j < nc; j++){
			if (matrice[i][j] < min) min = matrice[i][j];
		}
	}
	return min;
}

int visualiseMatriceInt(unsigned int** matrice, int nl, int nc) {
  //Create a SDL window which will represent the matrix
  //Black is a high value  whereas white represents a small  one 
   
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0){
		fprintf(stderr, "Echec d'initialisation de la SDL dans la fonction visualiseMatrice: %s \n", SDL_GetError());
		return -1;
	}
	
	SDL_Window* pWindow = NULL;
	SDL_Renderer* pRenderer = NULL;
	SDL_Event ev;
	int i, j, min, max, niveauGris;
	int continuer = 1;
	
       	pWindow = SDL_CreateWindow("Apercu de la matrice", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, nc, nl, SDL_WINDOW_SHOWN);
	if(!pWindow){fprintf(stderr,"Impossible de creer la fenetre: %s\n",SDL_GetError()); return -1;}
	
	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if(!pRenderer){fprintf(stderr,"Impossible de creer le renderer: %s\n",SDL_GetError()); return -1;}
	
	max = trouveMaximunI(matrice, nl, nc);
	min = trouveMinimunI(matrice, nl, nc);
	//Color each pixels of the matrix				
	for (i = 0; i < nl; i++){
	    for (j = 0; j < nc; j++){
		niveauGris = (255 * (matrice[i][j] - min)) / (max - min);
		if (niveauGris > 255) fprintf(stderr, "Erreur de calcul en %d,%d : %d\n", i, j, niveauGris);
		SDL_SetRenderDrawColor(pRenderer, niveauGris, niveauGris, niveauGris, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawPoint(pRenderer, j, i);						
	      }					
	  }
	SDL_RenderPresent(pRenderer);

	//Event management 
	while (continuer){
	  while (SDL_PollEvent(&ev)){
	    switch (ev.type){
	    case(SDL_WINDOWEVENT):
	      if (ev.window.event == SDL_WINDOWEVENT_CLOSE) continuer = 0;
	      break;
	    case(SDL_KEYDOWN):
	      if (ev.key.keysym.sym == SDLK_ESCAPE) continuer = 0;
	      break;
	    case(SDL_MOUSEBUTTONDOWN):
	      j = ev.button.x;
	      i = ev.button.y;
	      niveauGris = (255 * (matrice[i][j] - min)) / (max - min);
	      fprintf(stdout, "Le pixel en (%d,%d) a pour couleur %d et pour valeur %hu \n", i, j, niveauGris, matrice[i][j]);
	      break;
	    default:
	      break;
	    }
	  }
	}
	SDL_DestroyRenderer(pRenderer);
	return 1;
}


int indice_max(int* tab,int debut,int fin){
  //Cherche l'indice de l'element maximum entre l'indice de debut inclu et l'indice de fin exclu
  int i,k=debut,m=tab[debut];
  for(i=debut+1;i<fin;i++){
    if(tab[i]>m){
      k=i;
      m=tab[i];
    }
  }
  return k;
}

int indice_min(int* tab,int debut,int fin){
  //Cherche l'indice de l'element minimum entre l'indice de debut inclu et l'indice de fin exclu
  int i,k=debut,m=tab[debut];
  for(i=debut+1;i<fin;i++){
    if(tab[i]<m){
      k=i;
      m=tab[i];
    }
  }
  return k;
}


void visualise_image_kinect(unsigned int** matrice,COUPLE p1, COUPLE p2){
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0){
    fprintf(stderr, "Echec d'initialisation de la SDL dans la fonction visualiseMatrice: %s \n", SDL_GetError());
    return;
  }
	
  SDL_Window* pWindow = NULL;
  SDL_Renderer* pRenderer = NULL;
  SDL_Event ev;
  int i, j, min, max, niveauGris;
  int continuer = 1;
	
  pWindow = SDL_CreateWindow("Apercu de la matrice", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,480, SDL_WINDOW_SHOWN);
  if(!pWindow){fprintf(stderr,"Impossible de creer la fenetre: %s\n",SDL_GetError()); return ;}
	
  pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
  if(!pRenderer){fprintf(stderr,"Impossible de creer le renderer: %s\n",SDL_GetError()); return ;}
	
  max = trouveMaximunI(matrice, 480, 640);
  min = trouveMinimunI(matrice, 480, 640);
  
  //Color each pixels of the matrix				
  for (i = 0; i < 480; i++){
    for (j = 0; j < 640; j++){
      niveauGris = (255 * (matrice[i][j] - min)) / (max - min);
      if (niveauGris > 255) fprintf(stderr, "Erreur de calcul en %d,%d : %d\n", i, j, niveauGris);
      SDL_SetRenderDrawColor(pRenderer, niveauGris, niveauGris, niveauGris, SDL_ALPHA_OPAQUE);
      SDL_RenderDrawPoint(pRenderer, j, i);						
    }					
  }
  //Ajout de la limite de bord haut
  SDL_SetRenderDrawColor(pRenderer,255,0,0,SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(pRenderer,p1.c,p1.l,p2.c,p2.l);
    
  SDL_RenderPresent(pRenderer);
  while (continuer){
    while (SDL_PollEvent(&ev)){
      switch (ev.type){
      case(SDL_WINDOWEVENT):
	if (ev.window.event == SDL_WINDOWEVENT_CLOSE) continuer = 0;
	break;
      case(SDL_KEYDOWN):
	if (ev.key.keysym.sym == SDLK_ESCAPE) continuer = 0;
	break;
      case(SDL_MOUSEBUTTONDOWN):
	j = ev.button.x;
	i = ev.button.y;
	niveauGris = (255 * (matrice[i][j] - min)) / (max - min);
	fprintf(stdout, "Le pixel en (%d,%d) a pour couleur %d et pour valeur %hu \n", i, j, niveauGris, matrice[i][j]);
	break;
      default:
	break;
      }
    }
  }
  SDL_DestroyRenderer(pRenderer);
}


void recupere_calibrage_manuel(COUPLE* haut_gauche,COUPLE* haut_droit,
		      COUPLE* bas_droit,COUPLE* bas_gauche){	

  SDL_Event ev;
  int k=0;
  COUPLE Czero ={0,0};
  *haut_gauche=Czero;
  *haut_droit=Czero;
  *bas_gauche=Czero;
  *bas_gauche=Czero;

  while(k<6){
    SDL_WaitEvent(&ev);
    
    if(ev.type==SDL_MOUSEBUTTONDOWN){

      if(k==0){
	haut_droit->l = ev.button.y;
	haut_gauche->l = ev.button.y;
	k+=1;
	continue;
      }
      if(k==1){
	bas_gauche->l= ev.button.y;
	bas_droit->l=ev.button.y;
	k+=1;
	continue;
      }
      if(k==2){
	haut_gauche->c=ev.button.x;
	k+=1;
	continue;
      }
      if(k==3){
	haut_droit->c=ev.button.x;
	k+=1;
	continue;
      }
      if(k==4){
	bas_droit->c=ev.button.x;
	k+=1;
	continue;
      }
      if(k==5){
	bas_gauche->c=ev.button.x;
	k+=1;
      }
    }
  }
  printf("coordonées des bords:\n {%d,%d} {%d,%d}\n{%d,%d} {%d,%d}\n",haut_gauche->c,haut_gauche->l,haut_droit->c,haut_droit->l,bas_gauche->c,bas_gauche->l,bas_droit->c,bas_droit->l);
}

void dessine_rectangle(SDL_Renderer* pRenderer,COUPLE haut_gauche,COUPLE haut_droit,
		       COUPLE bas_droit,COUPLE bas_gauche){
  SDL_SetRenderDrawColor(pRenderer,255,0,0,255);
  SDL_RenderDrawLine(pRenderer,haut_gauche.c,haut_gauche.l,haut_droit.c,haut_droit.l);
  SDL_RenderDrawLine(pRenderer,bas_gauche.c,bas_gauche.l,bas_droit.c,bas_droit.l);
  SDL_RenderDrawLine(pRenderer,haut_gauche.c,haut_gauche.l,bas_gauche.c,bas_gauche.l);
  SDL_RenderDrawLine(pRenderer,haut_droit.c,haut_droit.l,bas_droit.c,bas_droit.l);

}

int est_dans_trapeze(int i,int j,COUPLE haut_gauche,COUPLE haut_droit,COUPLE bas_droit,COUPLE bas_gauche){

	if(i<haut_gauche.l || i> bas_droit.l || j< bas_gauche.c  || j>bas_droit.c) return 0; //cas externe trivial
  if(j>=haut_gauche.c && j<= haut_droit.c) return 1; // cas interne trivial 
  if(j < haut_gauche.c){ 
    if(i - haut_gauche.l >=((bas_gauche.l - haut_gauche.l) *(haut_gauche.c - j) )/(haut_gauche.c - bas_gauche.c)){
    	return 1;
    }
    return 0;
  }
  if(j > haut_droit.c){// cas dans le triangle droit
    if(i - haut_droit.l >= ( (bas_droit.l - haut_droit.l) * (haut_droit.c - j) ) / (haut_droit.c - bas_droit.c)) return 1;
    return 0;
  }
	printf("erreur pour (%d,%d)\n",i,j);
	return 0; 
}


unsigned int** recopie_matrice(unsigned int** matrice,COUPLE haut_gauche,COUPLE haut_droit,COUPLE bas_droit,COUPLE bas_gauche){
  unsigned int** copie_matrice = alloueMatriceInt(bas_droit.l - haut_droit.l +1 ,
						  bas_droit.c - bas_gauche.c +1 );
  int i,j;
  for(i=haut_droit.l; i <= bas_droit.l ; i++){
    for(j=bas_gauche.c;j<= bas_droit.c;j++){
      if(est_dans_trapeze(i,j,haut_gauche,haut_droit,bas_droit,bas_gauche))
	copie_matrice[i-haut_droit.l][j-bas_gauche.c]=matrice[i][j];
      else
	copie_matrice[i-haut_droit.l][j-bas_gauche.c]=MAXINT;
    }
  }
  return copie_matrice;
}

  
void calibrage_manuel(unsigned int** matrice,COUPLE* haut_gauche,COUPLE* haut_droit,
			       COUPLE* bas_droit,COUPLE* bas_gauche){
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0){
    fprintf(stderr, "Echec d'initialisation de la SDL dans la fonction visualiseMatrice: %s \n", SDL_GetError());
    return;
  }
	
  SDL_Window* pWindow = NULL;
  SDL_Renderer* pRenderer = NULL;
  SDL_Event ev;
  int i, j, min, max, niveauGris,continuer;
	
  pWindow = SDL_CreateWindow("Apercu de la matrice", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,480, SDL_WINDOW_SHOWN);
  if(!pWindow){fprintf(stderr,"Impossible de creer la fenetre: %s\n",SDL_GetError()); return ;}
	
  pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
  if(!pRenderer){fprintf(stderr,"Impossible de creer le renderer: %s\n",SDL_GetError()); return ;}
	
  max = trouveMaximunI(matrice, 480, 640);
  min = trouveMinimunI(matrice, 480, 640);
  
  //Color each pixels of the matrix				
  for (i = 0; i < 480; i++){
    for (j = 0; j < 640; j++){
      niveauGris = (255 * (matrice[i][j] - min)) / (max - min);
      if (niveauGris > 255) fprintf(stderr, "Erreur de calcul en %d,%d : %d\n", i, j, niveauGris);
      SDL_SetRenderDrawColor(pRenderer, niveauGris, niveauGris, niveauGris, SDL_ALPHA_OPAQUE);
      SDL_RenderDrawPoint(pRenderer, j, i);						
    }					
  }

  SDL_RenderPresent(pRenderer);
  recupere_calibrage_manuel(haut_gauche,haut_droit,bas_droit,bas_gauche);
  dessine_rectangle(pRenderer,*haut_gauche,*haut_droit,*bas_droit,*bas_gauche);
  SDL_RenderPresent(pRenderer);  
  SDL_Delay(2000);
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
}

  
  



unsigned int** redimensionne_matrice(unsigned int** matrice_init, int nl_init, int nc_init,int nl_cible,int nc_cible){
  unsigned int** matrice_cible=alloueMatriceInt(nl_cible,nc_cible);
  unsigned int** matrice_marqueurs=alloueMatriceInt(nl_cible,nc_cible);
  int i,j,i_cible,j_cible;
  for(i=0;i<nl_init;i++){
    for(j=0;j<nc_init;j++){
      i_cible=i*nl_cible/nl_init;
      j_cible=j*nc_cible/nc_init;
      matrice_cible[i_cible][j_cible]=matrice_init[i][j];
      matrice_marqueurs[i_cible][j_cible]=1;
    }
  }
  completter_matrice(matrice_cible,matrice_marqueurs,nl_cible,nc_cible);
  libereMatrice(matrice_marqueurs);
  return(matrice_cible);
}


unsigned int** retouche_matrice(unsigned int** matrice_init, int nl, int nc){
  unsigned int** matrice_cible=alloueMatriceInt(nl,nc);
  unsigned int** matrice_marqueurs=alloueMatriceInt(nl,nc);
  BORNES* matrice_bornes_c=calloc(nl,sizeof(*matrice_bornes_c));
  int borne_inf=0;
  int borne_sup=0;
  int i,j;
  for(i=0;i<nl;i++){
    borne_inf=0;
    borne_sup=0;
    for(j=0;j<nc;j++){
      if(matrice_init[i][j]!=MAXINT){
	if(borne_inf==0){
	  borne_inf=j;
	  borne_sup=j;
	}
	else{
	  borne_sup++;
	}
      }
    }
    matrice_bornes_c[i].inf=borne_inf;
    matrice_bornes_c[i].sup=borne_sup;
		
  }
  int i_cible,j_cible;
  for(i=0;i<nl;i++){
    for(j=0;j<nc;j++){
      if(matrice_init[i][j]!=MAXINT){
				
				i_cible=i;
				j_cible=(j-matrice_bornes_c[i].inf)*nc/(matrice_bornes_c[i].sup - matrice_bornes_c[i].inf+1);
				//printf("%d--%d-cible %d-%d\n",i,j,i_cible,j_cible);
				matrice_cible[i_cible][j_cible]=matrice_init[i][j];
				matrice_marqueurs[i_cible][j_cible]=1;
      }
    }
  }
	completter_matrice(matrice_cible,matrice_marqueurs,nl,nc);
	free(matrice_bornes_c);
	libereMatrice(matrice_marqueurs);
	
  return(matrice_cible);
}

void completter_matrice(unsigned int** matrice_cible, unsigned int** matrice_marqueurs,int nl,int nc){
	int i, j, i_cible, j_cible;
	int adresse_avant;
  int adresse_apres;
  int remplissage;
  unsigned int valeur_avant;
  unsigned int valeur_apres;
  for(i_cible=0;i_cible<nl;i_cible++){
    j=0;
    adresse_avant=-1;
    adresse_apres=-1;
    remplissage=10;//par defaut
    while(j<nc){
      if( matrice_marqueurs[i_cible][j]==0){
				adresse_apres=j;
				remplissage=0;
      }
      if(matrice_marqueurs[i_cible][j]==1){
	if(remplissage==0){
	  adresse_apres=j;
	  remplissage=1;
	}
	else{
	  adresse_apres=j;
	  adresse_avant=j;
	  remplissage=10;
	}

      }
      if(j==nc-1 && remplissage!=1 ){
	remplissage=1;
	adresse_apres=-1;
      }

      if(remplissage==1){
                   
	if(adresse_avant==-1){
	  valeur_avant=matrice_cible[i_cible][adresse_apres];
	  valeur_apres=valeur_avant;
	}
	else{if(adresse_apres==-1){
	    valeur_avant=matrice_cible[i_cible][adresse_avant];
	    valeur_apres=valeur_avant;
	    adresse_apres=nc;
	  }
	  else{
	    valeur_avant=matrice_cible[i_cible][adresse_avant];
	    valeur_apres=matrice_cible[i_cible][adresse_apres];
	  }
	}
	int baladeur;
	//printf("//ligne:%d,zone:%d-%d,valeur: %d \n",i_cible,adresse_avant,adresse_apres,valeur_avant);
	for(baladeur=adresse_avant+1;baladeur<adresse_apres;baladeur++){
	  matrice_cible[i_cible][baladeur]=(baladeur-adresse_avant)*(valeur_avant-valeur_apres)/(adresse_avant-adresse_apres)+valeur_avant;
	  matrice_marqueurs[i_cible][baladeur]=1;
			
	}
	adresse_avant=adresse_apres;

      }

      j++;
    }

  }

  for(j_cible=0;j_cible<nc;j_cible++){
    i=0;
    adresse_avant=-1;
    adresse_apres=-1;
    remplissage=10;//par defaut vaut 10, vaut 0 lorsque iln'y avait pas de donné la case avant, 1 si la on passe de case sans donné à case avec donné 
    while(i<nl){

      if( matrice_marqueurs[i][j_cible]==0){
	adresse_apres=i;
	remplissage=0;
      }
      if(matrice_marqueurs[i][j_cible]==1){
	if(remplissage==0){
	  adresse_apres=i;
	  remplissage=1;
	}
	else{
	  adresse_apres=i;
	  adresse_avant=i;
	  remplissage=10;
	}


      }
      if(i==nl-1 && remplissage!=1 ){
	remplissage=1;
	adresse_apres=i-1;
      }
      if(remplissage==1){
                   
	if(adresse_avant==-1){
	  valeur_avant=matrice_cible[adresse_apres][j_cible];
	  valeur_apres=valeur_avant;
	}
	else{if(adresse_apres==-1){
	    valeur_avant=matrice_cible[adresse_avant][j_cible];
	    valeur_apres=valeur_avant;
	    adresse_apres=nc;
	  }
	  else{
	    valeur_avant=matrice_cible[adresse_avant][j_cible];
	    valeur_apres=matrice_cible[adresse_apres][j_cible];
	  }
	}
	int baladeur;
	for(baladeur=adresse_avant+1;baladeur<adresse_apres;baladeur++){
	  matrice_cible[baladeur][j_cible]=(baladeur-adresse_avant)*(valeur_avant-valeur_apres)/(adresse_avant-adresse_apres)+valeur_avant;
	  matrice_marqueurs[baladeur][j_cible]=1;
	}
	adresse_avant=adresse_apres;
      }

      i++;
    }

  }
}


