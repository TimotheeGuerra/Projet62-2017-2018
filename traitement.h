#ifndef _TRAITEMENT
#define _TRAITEMENT

int nb_erreurs_image(unsigned int** matrice);
//Compte le nombre de pixel aberrants dans la matrice
unsigned int** moyenne_matrice(unsigned int*** tab_matrice,int nb_matrice);
//Realise la moyenne  de n matrice. Les valeurs aberrantes sont supprimiees
unsigned int** profondeur_to_altitude(unsigned int** matrice_profondeur,int nl,int nc);
//Recree un autre matrice correspondant a la transformation d'une matrice de profondeur vers une matrice d'altitude

#endif
