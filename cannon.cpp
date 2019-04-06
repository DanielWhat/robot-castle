/* Description: A series of functions for drawing, rotating, and firing
 * the cannon.*/

#define GL_SILENCE_DEPRECATION 0
//an offset to account for the fact that the cannonball may not begin at y = 0 when inside the cannon
//To understand the formula remember that we want to rotate the ball as if it was at (55, 0) but then 
//transpose it to (55, 28) so that it fits inside the cannon
#define Y_OFFSET (sin(cannon_angle * (M_PI/180)) * 55 + cos(cannon_angle * (M_PI/180)) * 0 + 28)

#include <math.h>
#include <GL/freeglut.h>
#include "open_off.h"
#include "cannon.h"
#include <stdbool.h>

//@@@ Need to find a fix for the white and black thing
static float white_c[4] = {1.0, 1.0, 1.0, 1.0};
static float black_c[4] = {0.0, 0.0, 0.0, 1.0};

/* For controlling the physics of the cannonball */
static float gravity = 1;
static float friction = 1;
static bool is_cannon_fired = false;
int cannon_angle = 20;
static float cannon_power = 20;
static CannonBall cannon_ball = {.x = 0, .y = Y_OFFSET, .z = 0, .velocity_x = 0, .velocity_y = 0, .velocity_z = 0};


void update_cannonball_position(void)
/* Updates the y position of the cannon ball when the cannon is rotated.
 * Only works when the cannon ball is in the cannon */
 
{
    if (!is_cannon_fired) {
        cannon_ball.y = Y_OFFSET;
    }
}



void move_cannonball(int data)
/* Updates the cannon ball's position and velocity when it is in motion */
{
    cannon_ball.x += (cannon_ball.x >= 0) ? cannon_ball.velocity_x : 0;
    cannon_ball.y += (cannon_ball.y >= 0) ? cannon_ball.velocity_y : 0;
    cannon_ball.velocity_y -= gravity;
    
    //If the cannonball is on the ground
    if (cannon_ball.y <= 0) {
        
        cannon_ball.y = 0;
        //Cannon ball is now slowing down on the ground
        cannon_ball.velocity_x = (cannon_ball.velocity_x > 0) ? cannon_ball.velocity_x - friction : 0; //But don't let velocity go negative
    }
    
    //The ball is not moving anymore
    if (cannon_ball.velocity_y == 0 && cannon_ball.velocity_x == 0) {
        //nothing
        
    } else {
        glutTimerFunc(50, move_cannonball, 0);
    }
    glutPostRedisplay();
}



void fire_cannon()
/* Sets the initial velocity of the cannon_ball and calls move_cannon_ball
 * to animate the cannon_ball's movement */
{
    cannon_ball.velocity_x = cos(cannon_angle * (M_PI / 180)) * cannon_power;
    cannon_ball.velocity_y = sin(cannon_angle * (M_PI / 180)) * cannon_power;
    
    is_cannon_fired = true;
    
    move_cannonball(0);
}



void draw_cannon(const float x[], const float y[], const float z[], const int t1[], const int t2[], const int t3[], int num_triangles)
/* Draws a cannon and cannon ball. The metal cannon barrel object should
 * be in the lists x, y, z, t1, t2, t3 and the integer num_triangles. 
 * See open_off.h for how to read an OFF file into this list form. This 
 * function also handles some minor updating of the CannonBall object */
{
    glMaterialfv(GL_FRONT, GL_SPECULAR, white_c); //Enable specular lighting
    //Draw metal cannon
    glPushMatrix();
        glColor3f(0.4, 0.5, 0.4);
        glRotatef(cannon_angle, 0, 0, 1);
        glTranslatef(15, -30, 0);
        draw_off_file(x, y, z, t1, t2, t3, num_triangles);
    glPopMatrix();
    
    glMaterialf(GL_FRONT, GL_SHININESS, 60); //increasing shininess for cannonball (lowering the radius of specular reflection)
    //Draw cannonball
    glColor3f(0.45, 0.45, 0.45);
    glPushMatrix();
        glTranslatef(cannon_ball.x, cannon_ball.y - Y_OFFSET, cannon_ball.z);
        glRotatef(cannon_angle, 0, 0, 1);
        glTranslatef(55, 0, 0);
        glutSolidSphere(5.0, 50, 50);
    glPopMatrix();
    glMaterialf(GL_FRONT, GL_SHININESS, 30); //reset shininess
    glMaterialfv(GL_FRONT, GL_SPECULAR, black_c); //disable specular lighting
    
    //Draw wooden brace @@@ <-- Texture this
    glColor3f(128.0/255, 70.0/255, 27.0/255);
    glPushMatrix();
        glTranslatef(15.0, -10.0, 16);
        glScalef(60, 40, 5);
        glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(15.0, -10.0, -16);
        glScalef(60, 40, 5);
        glutSolidCube(1.0);
    glPopMatrix();
}
