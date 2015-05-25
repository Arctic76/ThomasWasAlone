#ifndef PERSO_H
#define PERSO_H


#include "moteur/main.h"
#include <stdbool.h>


typedef struct Personnage
{
	int id;
	int centerX;
	int centerY;
	int width;
	int height;
	Color3f color;
	// variables touches
	bool gauche;
	bool droite;
	int lastDirection;
	bool haut;
	bool bas;
	// variables déplacements
	int defaultGravite;
	int gravite;
	int vitesse;
	int sens;
	// variables mouvements
	bool lateral;
	bool saute;
	bool repos;

	bool active; // perso actif (controlable) ou pas
	int cursorTimer;
	bool freeze;
	bool end;

}Personnage;
#include "moteur/scrolling.h"

void initPerso(Personnage *personnage, int id, int width, int height, int posCaseX, int posCaseY, Color3f color, int gravite);
void gestionJoueur(Personnage *persoHandler);
void changeFocus(Personnage *persoHandler, int nbrPerso, Camera *camera);

#endif /* PERSO_H */
