#include<stdio.h>
#include<stdlib.h>
#include"matrice.h"
#include"traitement.h"
#include"acquisition.h"




unsigned int** initialisation_manuel_relief(int nl_Final,int nc_Final){
//Prends en parametre les dimmenssions de la matrice de relief souhaite. Renvoi l'adresse de la matrice  alloue dynamiquement et traitee
  COUPLE haut_gauche;
  COUPLE haut_droit;
  COUPLE bas_droit;
  COUPLE bas_gauche;
  int nl=480;//taille de  la matrice capture par le kinect
  int nc=640;
  int i,j;
  unsigned int** matrice=get_moy_depth(50);
	
  puts("debut de la capture relief"); 
  puts("calibrage manuel: cliquer sur les bords"); 
  calibrage_manuel(matrice,&haut_gauche,&haut_droit,&bas_droit,&bas_gauche);
  printf("fin calibrage \n");
	
  //saisi des bords par l utilisateur
  int nl_Calibre=bas_droit.l-haut_droit.l+1;//taille de la matrice calibre
  int nc_Calibre=bas_droit.c-bas_gauche.c+1;
  unsigned int** copie_matrice = recopie_matrice(matrice,haut_gauche,haut_droit,bas_droit,bas_gauche);
  libereMatrice(matrice);
  
  //transformation trapeze-rectangle:
  unsigned int** matrice_retouche= retouche_matrice(copie_matrice,nl_Calibre,nc_Calibre);
  libereMatrice(copie_matrice);
  printf("matrice decoupe\n");  
  
  //ajustement de la taille:
  unsigned int** matrice_redim= redimensionne_matrice(matrice_retouche, nl_Calibre, nc_Calibre, nl_Final, nc_Final);
  libereMatrice(matrice_retouche);
  
  //transformation distance-profondeur:
  unsigned int ** matrice_final=profondeur_to_altitude(matrice_redim,nl_Final,nc_Final);
  libereMatrice(matrice_redim);
  printf("matrice agrandi\n");
 
 	return(matrice_final);
    
}
