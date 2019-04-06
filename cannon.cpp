/* Description: A series of functions for drawing, rotating, and firing
 * the cannon.*/

#define GL_SILENCE_DEPRECATION 0

#include <GL/freeglut.h>
#include "open_off.h"
#include "cannon.h"

//@@@ Need to find a fix for the white and black thing
static float white_c[4] = {1.0, 1.0, 1.0, 1.0};
static float black_c[4] = {0.0, 0.0, 0.0, 1.0};

/* For controlling the physics of the cannonball */
static float x_velocity = 0;
static float y_velocity = 0;
static float z_velocity = 0;
static float gravity = 1;


void move_cannonball(void)
{
    
    y_velocity -= gravity;
    
}


void fire_cannon(void)
/* Sets the initial velocity of the cannonball and calls move_cannonball
 * to animate the cannonball's movement */
{
    x_velocity = 10;
    y_velocity = 10;
    
    move_cannonball();
}

void keyboard(char key, int x, int y)
{
    if (key == 'c') {
        fire_cannon();
    }
}


void draw_cannon(const float x[], const float y[], const float z[], const int t1[], const int t2[], const int t3[], int num_triangles)
/* Draws a cannon. The metal cannon barrel object should be in the lists
 * x, y, z, t1, t2, t3 and the integer num_triangles. See open_off.h for
 * how to read an OFF file into this list form. */
{
    //Draw metal cannon
    glMaterialfv(GL_FRONT, GL_SPECULAR, white_c); //Enable specular lighting
    glPushMatrix();
        glColor3f(0.4, 0.5, 0.4);
        glRotatef(30, 0, 0, 1);
        glTranslatef(15, -30, 0);
        draw_off_file(x, y, z, t1, t2, t3, num_triangles);
    glPopMatrix();
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
