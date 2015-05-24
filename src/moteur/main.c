#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>


#include "moteur/main.h"
#include "initialisation/level.h"
#include "formes/carre.h"
#include "moteur/perso.h"
#include "moteur/touche.h"
#include "moteur/deplacements.h"
#include "moteur/scrolling.h"
#include "moteur/colision.h"

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void reshape(int winWidth, int winHeight) {
  glViewport(0, 0, winWidth, winHeight);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //gluOrtho2D(left,right,top,bot);
  gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
}

void setVideoMode(int winWidth, int winHeight) {
  if(NULL == SDL_SetVideoMode(winWidth, winHeight, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE)) {
    fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
    exit(EXIT_FAILURE);
  }
  reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
}

int main(int argc, char** argv) {
  int loop = 1;
  int nbrPerso = 0;
  int j = 0;

  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }

  setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);

  SDL_WM_SetCaption("Thomas Was Alone", NULL);


  int **level = calloc(LINES + COLUMNS, sizeof(int*));
  // Création du level
  initLevel(level);
  int persoInfos[3][2];
  // Par défaut, coordoonnées des perso à -1 
  // afin de savoir si le perso est utilisé ou pas
  persoInfos[0][1] = -1;
  persoInfos[0][1] = -1;
  persoInfos[1][0] = -1;
  persoInfos[1][1] = -1;
  persoInfos[2][0] = -1;
  persoInfos[2][1] = -1;

  //FIXME : Choose level in menu 
  char const *path = "./levels/level-2.csv";
  loadLevelFromFile(level, path, persoInfos);
  //creation camera
  Camera camera;

  //test
  Personnage *persoHandler;
  persoHandler = calloc(3,sizeof(Personnage));

  Color3f RED;
  RED.r = 1;
  RED.g = 0;
  RED.b = 0;

  while(persoInfos[nbrPerso][0] != -1 && persoInfos[nbrPerso][1] != -1 && nbrPerso <3){
    // FIXME : gérer la couleur
    // perso, width, height, caseX, caseY, color
    initPerso(&persoHandler[nbrPerso], 2, 2, persoInfos[nbrPerso][0], persoInfos[nbrPerso][1], RED);
    nbrPerso++;
  }


  // Par défaut perso 1 actif
  initCam(&persoHandler[0], &camera);
  glPushMatrix();
  persoHandler[0].active = true;

  //glScalef(1.2,1.2,0);
  centerCam(&persoHandler[0], &camera);
  

  while(loop) {
    /* temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();

    glClear(GL_COLOR_BUFFER_BIT);

    /* AFFICHAGE */

    creeDecor(level);                                           // Affichage décor

    for (j = 0; j < nbrPerso; j++)
    {
      glColor3f(persoHandler[j].color.r, persoHandler[j].color.g, persoHandler[j].color.b);  // Affichage du joueur
      dessinPerso(&persoHandler[j]);
      glColor3f(1, 1, 1);
    }

    /* GESTION JOUEUR */

    gestionJoueur(persoHandler);
    deplacementJoueur(persoHandler, level, &camera);

    // cursor
    for (j = 0; j < nbrPerso; j++)
    {
       if (persoHandler[j].active)
      {
        if(persoHandler[j].cursorTimer<180) {
          dessinActiveCursor(&persoHandler[j]);
          persoHandler[j].cursorTimer++;
        }
      }
    }
    
    // camera

    if(camera.is_transition == false) scrolling(&camera);
    else if(camera.is_transition == true){
      if(camera.Dx < -0.001 || camera.Dx > 0.001){
        smoothTransition(&camera);
      }
      else camera.is_transition = false;
    }

    SDL_GL_SwapBuffers();
        SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        loop = 0;
        break;
      }

      /* GESTION TOUCHE */
    for (j = 0; j < nbrPerso; j++)
    {
      if (persoHandler[j].active)
      {
        appuyer(&persoHandler[j],e);
        relacher(&persoHandler[j],e);
      }
    }
      

      switch(e.type) {                 
        case SDL_VIDEORESIZE:
        WINDOW_WIDTH  = e.resize.w;
        WINDOW_HEIGHT = e.resize.h;
        setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
        break;

        case SDL_KEYDOWN:
          switch(e.key.keysym.sym){
            case 'q' : 
            case SDLK_ESCAPE :
              freeLevel(level);
              free(persoHandler);
              persoHandler = NULL;
              loop = 0;
              break;
            case SDLK_TAB:
              changeFocus(&persoHandler[0], &persoHandler[1], &persoHandler[2], &camera);
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
