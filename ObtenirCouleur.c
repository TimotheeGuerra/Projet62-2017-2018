#include "Fonctions.h"

/* ********************************************************************* */
/*definirPixel : permet de modifier la couleur d'un pixel
Parametres d'entree/sortie :
SDL_Surface *surface : la surface sur laquelle on va modifier la couleur d'un pixel
int x : la coordonnee en x du pixel a modifier
int y : la coordonnee en y du pixel a modifier
Uint32 pixel : le pixel a inserer
*/
void definirPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    /*nbOctetsParPixel represente le nombre d'octets utilises pour stocker un pixel.
    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    de l'image : 8, 16, 24 ou 32 bits.*/
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    /*Ici p est l'adresse du pixel que l'on veut modifier*/
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;

    /*Gestion differente suivant le nombre d'octets par pixel de l'image*/
    switch(nbOctetsParPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            /*Suivant l'architecture de la machine*/
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}
