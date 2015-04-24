#include "deplacements.h"
#include "main.h"

/************************************/
/*   Gestion des déplacements       */
/************************************/


// Touches de direction

// Appuyer sur une touche
void appuyer(Personnage *perso, SDL_Event e){
    if(e.type == SDL_KEYDOWN){

        switch(e.key.keysym.sym){
            case SDLK_RIGHT :
                perso->droite = true;
            break; 

            case SDLK_LEFT : 
                perso->gauche = true;
            break;

            case SDLK_UP : 
                perso->haut = true;
            break;

            case SDLK_DOWN : 
                perso->bas = true;
            break;

            case SDLK_SPACE :
                    perso->haut = true;
            break;

            default : break;

        }
    }
}
// Appuyer sur une touche
void relacher(Personnage *perso, SDL_Event e){
    if(e.type == SDL_KEYUP){

        switch(e.key.keysym.sym){
            case SDLK_RIGHT :
                perso->droite = false;
            break; 

            case SDLK_LEFT : 
                perso->gauche = false;
            break;

            case SDLK_UP : 
                perso->haut = false;
            break;

            case SDLK_DOWN : 
                perso->bas = false;
            break;

            case SDLK_SPACE :
                    perso->haut = false;
            break;

            default : break;

        }
    }
}

void deplacement(Personnage *perso, int **map) {

    /* deplacement latéral  */

    perso->posX += perso->vitesse * perso->sens;                                                // X = vitesse * sens


    /* Colisions */

    int C = (perso->centerX + (perso->width*TAILLE_CASE/2) * perso->sens)/TAILLE_CASE;          // Colonne à tester pour la colision
    int L = (perso->centerY - (perso->height*TAILLE_CASE/2))/ TAILLE_CASE;                      // Ligne à tester pour la colision

    // Colision latérale
    for (; L<perso->centerY/TAILLE_CASE+1; L++) {
        if (map[L][C]==1) {
            perso->centerX = C*TAILLE_CASE + TAILLE_CASE/2 -TAILLE_CASE*perso->sens;
            perso->posX = perso->centerX - (perso->width*TAILLE_CASE)/2;
        }
    }

    // Colision sol + saut

    perso->centerY += perso->gravite;
    perso->posY = perso->centerY - (perso->height*TAILLE_CASE)/2;
 
    
    for (C = (perso->centerX - (perso->width*TAILLE_CASE/2) )/TAILLE_CASE; C<(perso->centerX+(perso->width*TAILLE_CASE/2))/TAILLE_CASE; C++) { 
        if (map[L][C] == 1 && perso->centerY >= (L-perso->height)*TAILLE_CASE+(perso->height*TAILLE_CASE/2)) {   
            perso->centerY = (L-perso->height)*TAILLE_CASE+(perso->height*TAILLE_CASE/2);
            perso->posY = perso->centerY - (perso->height*TAILLE_CASE/2);
            perso->gravite = -1*(int)(perso->saute)*11;
            return;             
        }
    }
     
    if (perso->gravite++ > TAILLE_CASE) perso->gravite = TAILLE_CASE;

    // Colision roof
    if (perso->gravite < 0) {                    
        L = (perso->centerY-(perso->height*TAILLE_CASE/2))/TAILLE_CASE;                   
        for (C=(perso->centerX-(perso->width*TAILLE_CASE/2))/TAILLE_CASE; C<(perso->centerX+(perso->width*TAILLE_CASE/2))/TAILLE_CASE; C++) { 
            if (map[L][C] > 0) {      
                perso->centerY = (L+2)*TAILLE_CASE-(perso->height*TAILLE_CASE/2);
                perso->posY = perso->centerY - (perso->height*TAILLE_CASE/2);
                perso->gravite=1;
            }
        }
    }
}
