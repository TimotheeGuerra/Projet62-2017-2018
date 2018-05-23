#include "Fonctions.h"

int** creeMatricePyramideCentrale(int nl, int nc, int niveau_sol, int niveau_pyramide, int pente_pyramide) {
	int i, j;
	int** matrice = alloueMatriceSanskinect(nl, nc);
	if (niveau_pyramide > niveau_sol)
	{
		for (i = 0; i < nl; i++)
		{
			for (j = 0; j < nc; j++)
			{
				matrice[i][j] = fmax(niveau_sol, niveau_pyramide - pente_pyramide *
					fmax((int)fabs(nl / 2 - i), (int)fabs(nc / 2 - j)));
			}
		}
	}
	else
	{
		for (i = 0; i < nl; i++)
		{
			for (j = 0; j < nc; j++)
			{
				matrice[i][j] = fmin(niveau_sol, niveau_pyramide + pente_pyramide *
					fmax((int)fabs(nl / 2 - i), (int)fabs(nc / 2 - j)));
			}
		}
	}
	return matrice;
}

int** alloueMatriceSanskinect(int nl, int nc) {
	int** p = NULL;
	int i;
	p =(int**) calloc(nl, sizeof(*p));
	if (p == NULL)
	{
		fprintf(stderr, "Erreur lors de l'allocation d'une matrice d'entier de taille %d x %d\n", nl, nc);
		return NULL;
	}
	*p =(int*) calloc(nl*nc, sizeof(**p));
	if (*p == NULL)
	{
		fprintf(stderr, "Erreur lors de l'allocation d'une matrice d'entier de taille %d x %d\n", nl, nc);
		free(p);
		return NULL;
	}
	for (i = 1; i < nl; i++) p[i] = p[i-1] + nc;
	return p;
}

void libereMatriceSansKinect(int** matrice) {
	free(*matrice);
	free(matrice);
}

void CreationTextureRelief(CONTEXT* C,int** matrice, int nl, int nc,int min,int max) {
    int newmax,newmin,i,j,niveauGris;
    Uint32 pixel;
    if ((min == 0) && (max == 0))
    {
        newmax = trouveMaximun(matrice, nl, nc);
        newmin = trouveMinimun(matrice, nl, nc);
    }
    else
    {
        newmax = max;
        newmin = min;
    }
    for (i = 0; i < nl; i++)
    {
        for (j = 0; j < nc; j++)
        {
            niveauGris = (255 * (matrice[i][j] - newmin)) / (newmax - newmin);
            if (niveauGris > 255) fprintf(stderr, "Erreur de calcul en %d,%d : %d\n", i, j, niveauGris);
            pixel=SDL_MapRGBA(C->S_circuit[0]->format, niveauGris, niveauGris, niveauGris, 255);
            definirPixel(C->S_circuit[0],j,i,pixel);

        }

    }
    SDL_SetColorKey(C->S_circuit[0],SDL_TRUE,SDL_MapRGB(C->S_circuit[0]->format,0,0,0));
    C->T_circuit[0]=SDL_CreateTextureFromSurface(C->sdlRenderer,C->S_circuit[0]);
}

int trouveMinimun(int** matrice, int nl, int nc) {
	int i, j;
	int min = matrice[0][0];
	for (i = 0; i < nl; i++)
	{
		for (j = 0; j < nc; j++)
		{
			if (matrice[i][j] < min) min = matrice[i][j];
		}
	}
	return min;
}

int trouveMaximun(int ** matrice, int nl, int nc) {
	int i, j;
	int max = matrice[0][0];
	for (i = 0; i < nl; i++)
	{
		for (j = 0; j < nc; j++)
		{
			if (matrice[i][j] > max) max = matrice[i][j];
		}
	}
	return max;
}
