#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include "moteur/main.h"
#include "initialisation/level.h"
#include "formes/draw.h"
#include "moteur/perso.h"
#include "moteur/touche.h"
#include "moteur/deplacements.h"
#include "moteur/camera.h"
#include "moteur/colision.h"
#include "tools/menu.h" 

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
  Menu menu;
  menu.active = true;
  menu.levelNumber = 1;

  if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }

  setVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT);

  SDL_WM_SetCaption("Thomas Was Alone", NULL);

  // Création du level
  Level* level;

  int persoInfos[3][8];

  levelStart:
  level = loadLevelFromFile(selectLevelFromNumber(menu.levelNumber), persoInfos, &nbrPerso);
  //creation camera
  Camera camera;
  
  //Tous les perso sont ils à leur case de fin
  bool end = false;

  Personnage *persoHandler;
  persoHandler = calloc(3,sizeof(Personnage));

  for (j = 0; j < nbrPerso; j++)
  {
    Color3f persoColor;
    persoColor.r = (float)persoInfos[j][4]; // cast en float car color
    persoColor.g = (float)persoInfos[j][5]; // cast en float car color
    persoColor.b = (float)persoInfos[j][6]; // cast en float car color
    // perso, width, height, caseX, caseY, color
    initPerso(&persoHandler[j], j, persoInfos[j][2], persoInfos[j][3], persoInfos[j][0], persoInfos[j][1], persoColor, persoInfos[j][7]);
  }

  // Par défaut perso 1 actif
  initCam(&persoHandler[0], &camera);
  glPushMatrix();

  persoHandler[0].active = true;

  /*** TEXTURES ***/

  GLuint textureID[11];
  loadTexture("./img/level1.png", textureID, 1);
  loadTexture("./img/level2.png", textureID, 2);
  loadTexture("./img/level3.png", textureID, 3);
  loadTexture("./img/minecraft_tile.png", textureID, 4);

  /*** GESTION DU SON ***/
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    printf("%s", Mix_GetError());
  Mix_AllocateChannels(2);
  Mix_Chunk *bruitages[3];
  bruitages[0] = Mix_LoadWAV("./sounds/slime.wav");
  bruitages[1] = Mix_LoadWAV("./sounds/champimeuh.wav");
  bruitages[2] = Mix_LoadWAV("./sounds/creeper.wav");


  Mix_Music *musicLevel[4];
  musicLevel[0] = Mix_LoadMUS("./sounds/menu.mp3");
  musicLevel[1] = Mix_LoadMUS("./sounds/level1.mp3");
  musicLevel[2] = Mix_LoadMUS("./sounds/level2.mp3");
  musicLevel[3] = Mix_LoadMUS("./sounds/level3.mp3");


  




  while(loop) {
    /* temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();
    glClear(GL_COLOR_BUFFER_BIT);

    /*** MENU ***/
    if (menu.active)
    {
      if( Mix_PlayingMusic() != 1) Mix_PlayMusic(musicLevel[0], -1); //Jouer infiniment la musique
      dessinMenu(textureID);
      dessinActiveMenu(menu.levelNumber);
    } else {

      /* AFFICHAGE */
      creeDecor(level);

      // CAM
      moveCamera(&persoHandler[level->persoActive], &camera, level);

      // Gestion des fins de niveaux
      // Vérifie que chaque perso est sur sa case de fin,
      // dans le cas échéant, passe au niveau suivant
      // FIXME : switch dégueux trouver autre chose (essayé avec un for sur le nombre de perso, marche pas)
      switch(nbrPerso){
        case 1:
          if (persoHandler[0].end)
          {
            end = true;
            glPopMatrix();
            glPushMatrix();
          }
          break;
        case 2:
          if (persoHandler[0].end && persoHandler[1].end)
          {
            end = true;
            glPopMatrix();
            glPushMatrix();
          }
          break;
        case 3:
          if (persoHandler[0].end && persoHandler[1].end && persoHandler[2].end)
          {
            end = true;
            glPopMatrix();
            glPushMatrix();
          }
          break;
      }
     if (end) 
     {
        Mix_HaltMusic();
        // Il n'y a que trois niveaux si on est au 3ème et qu'on le fini, retour au menu
        if (menu.levelNumber < 3)
        {
          
          menu.levelNumber++;
          Mix_PlayMusic(musicLevel[menu.levelNumber], -1); //Jouer infiniment la musique
          goto levelStart;
          end = true;
        } else {
          menu.active = true;
          goto levelStart;
          break;
        }
     }
     /**** FIN GESTION NIVEAUX ******/

      for (j = 0; j < nbrPerso; j++)
      {
        glColor3f(persoHandler[j].color.r, persoHandler[j].color.g, persoHandler[j].color.b);  // Affichage du joueur
        dessinPerso(&persoHandler[j]);
        glColor3f(1, 1, 1);
      }

      /* GESTION JOUEUR */

      gestionJoueur(persoHandler, nbrPerso);
      deplacementJoueur(persoHandler, nbrPerso, level);

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

    }
    
    SDL_GL_SwapBuffers();
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      /* GESTION TOUCHE */
      if (!menu.active)
      {
        for (j = 0; j < nbrPerso; j++)
        {
          if (persoHandler[j].active)
          {
            appuyer(&persoHandler[j],e, j, bruitages);
            relacher(&persoHandler[j],e, j, bruitages);
          }
        }
      } else {
        touchesMenu(e, &menu, musicLevel);
        if (!menu.active)
        {
          goto levelStart;
        }
        
      }  

      switch(e.type) {
        case SDL_QUIT:
          loop = 0;
          break;
          
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

            case SDLK_BACKSPACE :
              glPopMatrix();
              glPushMatrix();
              goto levelStart;
              break;

            case SDLK_TAB:
              changeFocus(persoHandler, nbrPerso, level);
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
  Mix_CloseAudio();
  SDL_Quit();

  return EXIT_SUCCESS;
}
