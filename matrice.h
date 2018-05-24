#include<SDL2/SDL.h>
#include"traitement.h"
#define maxint 65535

unsigned int** alloueMatriceInt(int nl,int nc);
void libereMatrice(void** matrice);
unsigned int trouveMaximunI(unsigned int ** matrice, int nl, int nc);
unsigned int trouveMinimunI(unsigned int** matrice, int nl, int nc);
int visualiseMatriceInt(unsigned int** matrice, int nl, int nc);
int indice_max(int* tab,int debut,int fin);
int indice_min(int* tab,int debut,int fin);
void visualise_image_kinect(unsigned int** matrice,COUPLE p1,COUPLE p2);
void dessine_rectangle(SDL_Renderer* pRenderer,COUPLE haut_gauche,COUPLE haut_droit,
		       COUPLE bas_droit,COUPLE bas_gauche);
void est_dans_trapeze(int i,int j,COUPLE haut_gauche,COUPLE haut_droit,COUPLE bas_droit,
		      COUPLE bas_gauche);
void recupere_calibrage_manuel(COUPLE* haut_gauche,COUPLE* haut_droit,
			       COUPLE* bas_droit,COUPLE* bas_gauche);//interaction avec l utilisateur pour obtenir les coordones des bords du bac
unsigned int** recopie_matrice(unsigned int** matrice,COUPLE haut_gauche,
			       COUPLE haut_droit,COUPLE bas_droit,COUPLE bas_gauche);//rogne la matrice du kinect pour enlever les bords inutils (hors du bac ) 
void calibrage_manuel(unsigned int** matrice, COUPLE* haut_gauche,COUPLE* haut_droit,
		      COUPLE* bas_droit,COUPLE* bas_gauche);
unsigned int** remplace_matrice(unsigned int** matrice,int nl_init,int nc_init,
				int nl_cible,int nc_cible);
unsigned int** retouche_matrice(unsigned int** matrice,int nl ,int nc);//renvoi une matrice ou les deformations optiques sont corrigees (trapeze en rectangle)
void completter_matrice(unsigned int** matrice_cible, unsigned int** matrice_marqueurs,int nl,int nc);//prends une matrice incomplete et complete par interpolation ls points manquant 

//voir pus de details dans les fichiers .c 
