#include "moteur/main.h"
#include "initialisation/level.h"
#include "formes/carre.h"

#define LINES WINDOW_HEIGHT/TAILLE_CASE //30
#define COLUMNS WINDOW_WIDTH/TAILLE_CASE //40


void initLevel(int** level){

	int i = 0;
 	int j = 0;
  	for (i = 0; i < COLUMNS; i++)
  	{
    	level[i] = calloc(COLUMNS, sizeof(int));
  	}

  	for (i = 0; i < LINES; i++)
  	{
    	for (j = 0; j < COLUMNS; j++)
    	{
      	level[i][j] = 0;
    	}
  	}
  level[24][18] = 1;
  level[24][19] = 1;
  level[26][16] = 1;
  level[17][9] = 1;
  for (j = 0; j < COLUMNS; j++)
  {
    level[27][j] = 1;
    level[28][j] = 1;
  }
}

void freeLevel(int** level){
	int i = 0;
	for(i = 0; i < COLUMNS; i++) {
		free(level[i]);
		level[i] = NULL;
	}
	free(level);
	level = NULL;
}


void creeDecor(int **level){
	int i,j;

	for (i = 0; i < LINES; i++) // height
	{
		for (j = 0; j < COLUMNS; j++) //width
		{	
			//affiche le grid
			dessinCarre(0,j*32,i*32);
			if (level[i][j] == 1)
			{
				dessinCarre(1,j*32,i*32);
			}
		}
	}
}

int convertPixelToCase(int pixel){
	return pixel/TAILLE_CASE;
}

int convertCaseToPixel(float Case){
	return Case*TAILLE_CASE;
}