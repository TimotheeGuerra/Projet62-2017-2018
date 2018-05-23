#ifndef _CONSTANTES
#define _CONSTANTES

//les constantes liees aux commandes
#define COMMANDE_M_DEFFI 0
#define COMMANDE_M_CHOISIR 1
#define COMMANDE_M_PRE 2

#define COMMANDE_J_ACC 0
#define COMMANDE_J_REC 1
#define COMMANDE_J_TOU 2
#define COMMANDE_J_FRE 3
#define COMMANDE_J_DER 4
#define COMMANDE_J_PAU 5
#define COMMANDE_J_PERS 6

#define TABLEAU_MENU 1
#define TABLEAU_JEU 2

//les constantes liees aux boutons de la manette
#define BA 0
#define BB 1
#define BX 2
#define BY 3
#define L1 4
#define R1 5
#define BACK 6
#define START 7
#define L3 8
#define R3 9
#define CROIX 100
#define J_GAUCHE 101
#define J_DROIT 104
#define LT 102
#define RT 105


//les constantes liees aux axes de la manette
#define L3_HZ 0
#define L3_VT 1
#define L2 2
#define R3_HZ 3
#define R3_VT 4
#define R2 5
#define VALEUR_J_MAX 32767
#define VALEUR_J_MIN -32768

//les constantes liees a la physique de la voiture
#define G 9.81
#define DELTA 0.03
#define MASSE 691
#define FROTTEMENT_FLUIDE 500
#define FROTTEMENT_SOLIDE 0.25
#define TEMPS 30
#define FORCE_MOTEUR_MAX 10000 //une F1 peut aller de 0 a 100km/h en 2s, d'où, avec sa masse, cette force moteur
#define FORCE_FREINAGE 5000
#define VITESSE_MAX (FORCE_MOTEUR_MAX-MASSE*FROTTEMENT_SOLIDE*G)/FROTTEMENT_FLUIDE
#define RUPTURE_PENTE_MAX M_PI/9
#define TEMPS_SAUT_MIN 1.2

//constantes liees aux voitures
#define VOITURE_BLEUE 0
#define VOITURE_VERTE 1
#define VOITURE_ROUGE 2

//les constantes liees aux objets de jeux (cerises, flags ...)
#define NB_CERISE 2
#define ELEMENT_CERISE 0
#define ELEMENT_CERISE_DOREE 1
#define ELEMENT_TROPHEE 2
#define DEBUT_FLAG 3
#define DEBUT_BALISE 6

#define MILIEU_COUPE_X 199
#define MILIEU_COUPE_Y 124

//les constantes liees au score
#define LETTRE_J 0
#define DEBUT_NOMBRE 1
#define W_SCORE 30
#define H_SCORE 35

//les constantes liees au menu
#define CONTOUR_PRECEDENT 0
#define CONTOUR_MENU 1
#define CONTOUR_PAUSE 2
#define BOUTON_PRECEDENT 3
#define BOUTON_START 4
#define BOUTON_QUITTER 5
#define BOUTON_1JOUEUR 6
#define BOUTON_2JOUEUR 7
#define BOUTON_3JOUEUR 8
#define BOUTON_AVERTISSEMENT 9
#define BOUTON_JEUCERISE 10
#define BOUTON_JEUPRISEFLAG 11
#define MENU_PAUSE 12
#define BOUTON_JEUCOURSE 13
#define BOUTON_AIDE 14
#define BOUTON_COMMANDES 15
#define MENU_PERSPECTIVE 16
#define EXPLICATION_1 17
#define EXPLICATION_2 18
#define EXPLICATION_3 19
#define EXPLICATION_4 20
#define EXPLICATION_5 21
#define EXPLICATION_6 22
#define BOUTON_CAPTURE 23
#define CONTOUR_SURLIGNAGE_ROUGE 24
#define CONTOUR_SURLIGNAGE_VERT 25


//la constante definissant le zoom maximal de la voiture.
#define ZOOM_MAX 2

//les constantes pour definir la position du joueur par rapport au video-projecteur
#define DEVANT 1
#define DERRIERE 2
#define GAUCHE 3
#define DROITE 4

//les constantes liees au son
#define SON_CHANGEMENT_POSITION_J 0
#define SON_MOTEUR 1
#define SON_CLIC 2
#define SON_DERAPAGE 3
#define SON_CRASH 4
#define SON_REBOND 5
#define SON_VICTOIRE 6

#define DUREE_MOTEUR 212
#define DUREE_DERAPAGE 23
#define DUREE_REBOND 10
#define DUREE_CRASH 500

#define CANAL_MOTEUR 0
#define CANAL_CHANGEMENT_POSITION_J 3
#define CANAL_CLIC 4
#define CANAL_DERAPAGE 5
#define CANAL_CRASH 8
#define CANAL_REBOND 11
#define CANAL_VICTOIRE 14

#endif // _CONSTANTES

