
#ifndef _RELIEF
#define _RELIEF

/*                              FONCTION DU FICHIER CreationRelief.c                                  */


/**Alloue dinamiquement et en contigue une matrice d'entier de taille nl x nc */
int** alloueMatriceSanskinect(int nl, int nc);

/**Libere l'espace memoire lie a la matrice d'entiers*/
void libereMatriceSansKinect(int** matrice);

/**permet la creation d'une pyramide centrals*/
int** creeMatricePyramideCentrale(int nl, int nc, int niveau_sol, int niveau_pyramide, int pente_pyramide);

/**Affiche une fenetre SDL de taille (nl,nc) ou la couleur d'un pixel depend lineairement de la valeur de l'element de la matrice associe
L'etalonnage se fait automatiquement si min=max=0, sinon  il se fait selon les valeurs de min et max*/
void CreationTextureRelief(CONTEXT* C,int** matrice, int nl, int nc,int min,int max);

/**trouve le minimum dans une matrice 2D d'entier*/
int trouveMinimun(int** matrice, int nl, int nc);

/**trouve le maximum dans une matrice 2D d'entier*/
int trouveMaximun(int ** matrice, int nl, int nc);

#endif // _RELIEF
