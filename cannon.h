/* Description: A series of functions for drawing, rotating, and firing
 * the cannon.*/

#ifndef CANNON_H
#define CANNON_H
#define GL_SILENCE_DEPRECATION 0

#include <math.h>
#include <GL/freeglut.h>
#include "open_off.h"

typedef struct cannon_ball_s
{
    float x;
    float y;
    float z;
    float velocity_x;
    float velocity_y;
    float velocity_z;
    
} CannonBall;

extern int cannon_angle;


void update_cannonball_position(void);
/* Updates the y position of the cannon ball when the cannon is rotated.
 * Only works when the cannon ball is in the cannon */
 
 
 
void move_cannonball(void);
/* Updates the cannon ball's position and velocity when it is in motion */



void fire_cannon(void);
/* Sets the initial velocity of the cannonball and calls move_cannonball
 * to animate the cannonball's movement */



void draw_cannon(const float x[], const float y[], const float z[], const int t1[], const int t2[], const int t3[], int num_triangles);
/* Draws a cannon. The metal cannon barrel object should be in the lists
 * x, y, z, t1, t2, t3 and the integer num_triangles. See open_off.h for
 * how to read an OFF file into this list form. */
 
#endif /* CANNON_H */
