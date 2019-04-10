#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <GL/freeglut.h>
#include <math.h>
#include <stdbool.h>
#include "textures.h"

typedef struct spaceship_s {
    
    float angle;
    float height; 
    float leg_height;
    bool is_lights_on;
    
} Spaceship;


void normalm(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 );



void initialise_spaceship(int light);
/* Initialise textures and lighting needed to properly display the 
 * spaceship */


void normalf(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 );



float my_round(float x, int decimal_places);


void draw_paraboloid(int height);



void draw_spaceship_legs(void);



void draw_spaceship_body (bool is_textures);
/* Draws a spaceship body using surfaces of rotation method. Also adds a 
 * modulated texture to the spaceship body */


void draw_spaceship_shadow(Spaceship spaceship, int light);
/* A copy of the draw_spaceship function but without any built in colors
 * to allow the user to draw shadows */



void draw_spaceship(Spaceship spaceship, int light);
/* Draws a spaceship body, legs and lights. Takes a spaceship object and
 * light parameter (i.e GL_LIGHT2) */


#endif /* SPACESHIP_H */
