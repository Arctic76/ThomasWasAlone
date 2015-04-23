#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include "main.h"
#include "level.h"
#include "formes/carre.h"
#include "perso.h"
#include "deplacements.h"

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void reshape(int winWidth, int winHeight) {
  glViewport(0, 0, winWidth, winHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluOrtho2D(left,right,top,bot);
  gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
}

void setVideoMode(int winWidth, int winHeight) {
  if(NULL == SDL_SetVideoMode(winWidth, winHeight, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    exit(EXIT_FAILURE);
  }
  reshape(winWidth, winHeight);
}

int main(int argc, char** argv) {

  unsigned int windowWidth  = WINDOW_WIDTH;
  unsigned int windowHeight = WINDOW_HEIGHT;

  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }

  setVideoMode(windowWidth, windowHeight);

  SDL_WM_SetCaption("Thomas Was Alone", NULL);

  int loop = 1;

  int **level = calloc(WINDOW_HEIGHT/TAILLE_CASE, sizeof(int*));
  // Création du level
  initLevel(level);


  //creation perso;
  Personnage perso1;
  Color3f RED;
  RED.r = 1;
  RED.g = 0;
  RED.b = 0;

  // perso, width, height, caseX, caseY, color
  initPerso(&perso1, 1, 1, 4, 26, RED);

  int gravite = 2;

  while(loop) {
    /* temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();

    glClear(GL_COLOR_BUFFER_BIT);

    //affichage du décor
    creeDecor(level);

    //affichage du joueur
    glColor3f(perso1.color.r, perso1.color.g, perso1.color.b);
    dessinCarre(1, perso1.posX, perso1.posY);
    glColor3f(1, 1, 1);

    // mouvement joueur
    SDL_Event e;
    directions(&perso1, e);

    // calcul de la gravité test
    if (perso1.saute)
    {
      perso1.gravite += gravite;
      perso1.posY += perso1.gravite;

      if (isPixelGround(perso1.posX, perso1.posY, level))
      {
        perso1.gravite = 0;
        perso1.saute = false;
      }
      
    }

    SDL_GL_SwapBuffers();

    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        loop = 0;
        break;
      }

      switch(e.type) {                 
        case SDL_VIDEORESIZE:
        windowWidth  = e.resize.w;
        windowHeight = e.resize.h;
        setVideoMode(windowWidth, windowHeight);
        break;

        case SDL_KEYDOWN:
          switch(e.key.keysym.sym){
            case 'q' : 
            case SDLK_ESCAPE :
              freeLevel(level);
              loop = 0;
              break;
              default : break;
          }
        break;

        default:
        break;
      }
    }

    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    if(elapsedTime < FRAMERATE_MILLISECONDS) {
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }
  }

  SDL_Quit();

  return EXIT_SUCCESS;
}
