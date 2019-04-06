/* Description: A series of functions for drawing, rotating, and firing
 * the cannon.*/

#ifndef CANNON_H
#define CANNON_H
#define GL_SILENCE_DEPRECATION 0

#include <GL/freeglut.h>
#include "open_off.h"


void move_cannonball(void);



void fire_cannon(void);
/* Sets the initial velocity of the cannonball and calls move_cannonball
 * to animate the cannonball's movement */



void keyboard(unsigned char key, int x, int y);



void draw_cannon(const float x[], const float y[], const float z[], const int t1[], const int t2[], const int t3[], int num_triangles);
/* Draws a cannon. The metal cannon barrel object should be in the lists
 * x, y, z, t1, t2, t3 and the integer num_triangles. See open_off.h for
 * how to read an OFF file into this list form. */
 
#endif /* CANNON_H */
