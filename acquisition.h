#include<libfreenect_sync.h>
#include"matrice.h"

unsigned int ** get_depth();
/*Cette fonction cree une matrice 2D contenant la distance entre le kinect et son environnement
Cette fonction se charge de l'allocation dynamique pour une matrice de 640*480*/

unsigned int *** get_n_depth(int n);
/*Cree un tableau contenant n matrices prises a la suite par le kinect
Cette fonction se charge de l'allocation dynamique des n matrices de taille 640*480
*/

unsigned int** get_moy_depth(int n);
/*Retourne un matrice qui est la moyenne de n images prises à la suite
Cette fonction se charge de l'allocation dynamique de la matrice moyenne
Attention: Au cours de l'execution de la fonction, n+1 allocation dynamiques de matrice 640*480 sont  effectuees puis n sont liberees. Cela necessite une utilisation importante de la RAM pour des valeurs de n grand
*/
