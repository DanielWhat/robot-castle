/* Description: A series of functions for drawing, rotating, and firing
 * the cannon.*/

#ifndef CANNON_H
#define CANNON_H

//an offset to account for the fact that the cannonball may not begin at y = 0 when inside the cannon
//To understand the formula remember that we want to rotate the ball as if it was at (55, 0) but then 
//transpose it to (55, 28) so that it fits inside the cannon
#define Y_OFFSET (sin(cannon_angle * (M_PI/180)) * 55 + cos(cannon_angle * (M_PI/180)) * 0 + 28)

#include <math.h>
#include <GL/freeglut.h>
#include "open_off.h"
#include <stdbool.h>

typedef struct cannon_ball_s
{
    float x;
    float y;
    float z;
    float velocity_x;
    float velocity_y;
    float velocity_z;
    bool in_cannon;
    bool is_bouncy;
    
} CannonBall;

extern int cannon_angle;

void draw_cannon_shadow (const float x[], const float y[], const float z[], const int t1[], const int t2[], const int t3[], int num_triangles);
/* Draws a version of the cannon without colors for shadows */


void update_cannonball_position(void);
/* Updates the y position of the cannon ball when the cannon is rotated.
 * Only works when the cannon ball is in the cannon */
 
 
 
void move_cannonball(void);
/* Updates the cannon ball's position and velocity when it is in motion */


void reset_cannonball(void);


CannonBall* get_cannonball_pointer (void);
/* Returns a cannonball object describing the current location of the 
 * cannonball */


void fire_cannon(void);
/* Sets the initial velocity of the cannonball and calls move_cannonball
 * to animate the cannonball's movement */



void draw_cannon(const float x[], const float y[], const float z[], const int t1[], const int t2[], const int t3[], int num_triangles);
/* Draws a cannon. The metal cannon barrel object should be in the lists
 * x, y, z, t1, t2, t3 and the integer num_triangles. See open_off.h for
 * how to read an OFF file into this list form. */
 
#endif /* CANNON_H */
