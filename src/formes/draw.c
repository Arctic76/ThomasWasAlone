#include "formes/draw.h"

void dessinCarre(int posX, int posY, int numTexture, float offsetX, float offsetY){
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, numTexture); // minecraft

  glBegin(GL_POLYGON);
    // Haut gauche
    glTexCoord2f(offsetX, offsetY); glVertex2f(posX,posY);
    // Haut droite
    glTexCoord2f(offsetX+0.2, offsetY); glVertex2f(posX+TAILLE_CASE,posY);
    // BAs droite
    glTexCoord2f(offsetX+0.2, offsetY+1.0/3.0); glVertex2f(posX+TAILLE_CASE,posY+TAILLE_CASE);
    // Bas gauche
    glTexCoord2f(offsetX, offsetY+1.0/3.0); glVertex2f(posX,posY+TAILLE_CASE);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);

}

void dessinEmptyCarre(int posX, int posY, Color3f *color){
  glBegin(GL_LINE_LOOP);
    // Haut gauche
    glVertex2f(posX,posY);
    // Haut droite
    glVertex2f(posX+TAILLE_CASE,posY);
    // BAs droite
    glVertex2f(posX+TAILLE_CASE,posY+TAILLE_CASE);
    // Bas gauche
    glVertex2f(posX,posY+TAILLE_CASE);
  glEnd();
}

void dessinPerso(Personnage *perso, int numPerso){
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, numPerso+5);

  glBegin(GL_POLYGON);
    // Haut gauche
        glTexCoord2f(0, 0); glVertex2f(perso->box.pos.x,perso->box.pos.y);
    // Haut droite
        glTexCoord2f(1, 0); glVertex2f(perso->box.pos.x + perso->box.size.x,perso->box.pos.y );
    // Bas droite
        glTexCoord2f(1, 1); glVertex2f(perso->box.pos.x + perso->box.size.x,perso->box.pos.y + perso->box.size.y );
    // Bas gauche
        glTexCoord2f(0, 1); glVertex2f(perso->box.pos.x ,perso->box.pos.y + perso->box.size.y );

  glEnd();
  glDisable(GL_TEXTURE_2D);

}

void dessinActiveCursor(Personnage *perso){
  float i = 2*sin(perso->cursorTimer/2.0);
  glBegin(GL_TRIANGLES);
      // Haut gauche
          glVertex2f(perso->box.pos.x + perso->box.size.x/2 - (TAILLE_CASE/4), perso->box.pos.y - (TAILLE_CASE) +i );
      // Haut droite
          glVertex2f(perso->box.pos.x + perso->box.size.x/2 + (TAILLE_CASE/4) , perso->box.pos.y - (TAILLE_CASE) +i );
      // Bas millieu
          glVertex2f(perso->box.pos.x + perso->box.size.x/2, perso->box.pos.y - (TAILLE_CASE/2) +i );
  glEnd();
}

// 
void dessinMenu(Camera *camera){
  glEnable(GL_TEXTURE_2D);
  // FIXME : Bind texture
  glBindTexture(GL_TEXTURE_2D, 1);
  /* Dessin du quad */
  glBegin(GL_QUADS);
    glColor3f(1,1,1);
    glTexCoord2f(1, 1);
    glVertex2f(camera->x + 100, camera->y - 2*WINDOW_HEIGHT/6 +50);
    glTexCoord2f(1, 0);
    glVertex2f(camera->x + 100, camera->y - 2*WINDOW_HEIGHT/6 -50);
    glTexCoord2f(0, 0);
    glVertex2f(camera->x -100,camera->y - 2*WINDOW_HEIGHT/6 -50);
    glTexCoord2f(0, 1);
    glVertex2f(camera->x -100,camera->y - 2*WINDOW_HEIGHT/6 +50);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, 2);
  glBegin(GL_QUADS);
    glColor3f(1,1,1);
    glTexCoord2f(1, 1);
    glVertex2f(camera->x + 100, camera->y - WINDOW_HEIGHT/6 +50);
    glTexCoord2f(1, 0);
    glVertex2f(camera->x + 100, camera->y - WINDOW_HEIGHT/6 -50);
    glTexCoord2f(0, 0);
    glVertex2f(camera->x -100, camera->y - WINDOW_HEIGHT/6 -50);
    glTexCoord2f(0, 1);
    glVertex2f(camera->x -100, camera->y - WINDOW_HEIGHT/6 +50);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, 3);
  glBegin(GL_QUADS);
    glColor3f(1,1,1);
    glTexCoord2f(1, 1);
    glVertex2f(camera->x + 100, camera->y +50);
    glTexCoord2f(1, 0);
    glVertex2f(camera->x + 100, camera->y -50);
    glTexCoord2f(0, 0);
    glVertex2f(camera->x -100,camera->y -50);
    glTexCoord2f(0, 1);
    glVertex2f(camera->x -100,camera->y +50);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, 11);
  glBegin(GL_QUADS);
    glColor3f(1,1,1);
    glTexCoord2f(1, 1);
    glVertex2f(camera->x + 200, camera->y  + WINDOW_HEIGHT/4 +100);
    glTexCoord2f(1, 0);
    glVertex2f(camera->x + 200, camera->y  + WINDOW_HEIGHT/4 -100);
    glTexCoord2f(0, 0);
    glVertex2f(camera->x - 200, camera->y  + WINDOW_HEIGHT/4 -100);
    glTexCoord2f(0, 1);
    glVertex2f(camera->x - 200, camera->y  + WINDOW_HEIGHT/4 +100);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);

  glDisable(GL_TEXTURE_2D);
}

void loadTexture(const char* filename, GLuint textureID[11], int numTexture){
   SDL_Surface* image;
   image = IMG_Load(filename);
   if(image == NULL){
      printf("Error : image not found : %s\n", filename);
   } 
    GLenum format;
    switch(image->format->BytesPerPixel) {
      case 1:
        format = GL_RED;
        break;
      case 3:
        format = GL_RGB;
        break;
      case 4:
        format = GL_RGBA;
        break;
      default:
      /* On ne traite pas les autres cas */
      fprintf(stderr, "Format des pixels de l’image %s non pris en charge\n", filename);
      exit(EXIT_FAILURE);
    } 
  glGenTextures(1, textureID);

  glBindTexture(GL_TEXTURE_2D, numTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
  SDL_FreeSurface(image);
}

void dessinActiveMenu(int numMenu, Camera *camera){

  switch(numMenu){
    case 1:
      glBegin(GL_TRIANGLES);
        glVertex2f(camera->x - 150 , camera->y - 2*WINDOW_HEIGHT/6 +10);
        glVertex2f(camera->x - 150 , camera->y - 2*WINDOW_HEIGHT/6 -10);
        glVertex2f(camera->x - 150  +20, camera->y - 2*WINDOW_HEIGHT/6);
      glEnd();

      glBegin(GL_TRIANGLES);
        glVertex2f(camera->x + 150  -20, camera->y - 2*WINDOW_HEIGHT/6);
        glVertex2f(camera->x + 150 , camera->y - 2*WINDOW_HEIGHT/6 +10);
        glVertex2f(camera->x + 150 , camera->y - 2*WINDOW_HEIGHT/6 -10);          
      glEnd();
    break;

    case 2:
      glBegin(GL_TRIANGLES);        
        glVertex2f(camera->x - 150 , camera->y - WINDOW_HEIGHT/6 +10);
        glVertex2f(camera->x - 150, camera->y - WINDOW_HEIGHT/6 -10);
        glVertex2f(camera->x - 150 + 20, camera->y - WINDOW_HEIGHT/6);
      glEnd();

      glBegin(GL_TRIANGLES);
        glVertex2f(camera->x + 150 -20, camera->y - WINDOW_HEIGHT/6);
        glVertex2f(camera->x + 150 , camera->y - WINDOW_HEIGHT/6 +10);
        glVertex2f(camera->x + 150, camera->y - WINDOW_HEIGHT/6 -10);
      glEnd();
      break;

    case 3:
      glBegin(GL_TRIANGLES);
        glVertex2f(camera->x - 150 , camera->y +10);
        glVertex2f(camera->x - 150, camera->y -10);
        glVertex2f(camera->x - 150 + 20, camera->y);
      glEnd();

      glBegin(GL_TRIANGLES);
        glVertex2f(camera->x + 150 -20, camera->y);
        glVertex2f(camera->x + 150 , camera->y +10);
        glVertex2f(camera->x + 150, camera->y -10);
      glEnd();
      break;
  }
}


void drawBackground(Level *level, int levelNumber){
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, levelNumber+7);

  glBegin(GL_POLYGON);
    // Haut gauche
        glTexCoord2f(0, 0); glVertex2f(0,0);
    // Haut droite
        glTexCoord2f(1, 0); glVertex2f(level->width*TAILLE_CASE,0);
    // Bas droite
        glTexCoord2f(1, 1); glVertex2f(level->width*TAILLE_CASE, level->height*TAILLE_CASE );
    // Bas gauche
        glTexCoord2f(0, 1); glVertex2f(0, level->height*TAILLE_CASE );

  glEnd();
  glDisable(GL_TEXTURE_2D);
}


void freeTextures(const GLuint textureID[11]){
  int i = 0;
  for (i = 0; i < 10; i++)
  {
    glDeleteTextures(i, textureID);
  }
}
