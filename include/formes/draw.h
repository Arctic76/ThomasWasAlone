#ifndef CARRE_H
#define CARRE_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h> 
#include "moteur/main.h"
#include "moteur/perso.h"
#include <math.h>

void dessinCarre(int posX, int posY, int numTexture, float offsetX, float offsetY);
void dessinEmptyCarre(int posX, int posY, Color3f *color);
void dessinPerso(Personnage *perso, int numPerso);
void dessinActiveCursor(Personnage *perso);
void dessinMenu(Camera *camera);
void loadTexture(const char* filename, GLuint textureID[11], int numTexture);
void dessinActiveMenu(int numMenu, Camera *camera);
void drawBackground(Level *level, int levelNumber);
void freeTextures(const GLuint textureID[11]);
#endif
