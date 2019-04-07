#define GL_SILENCE_DEPRECATION

#include <GL/freeglut.h>
#include <math.h>
#include "open_off.h"
#include "textures.h"

static GLuint pillar_texture;
static GLuint wall_texture;

void initialise_castle_textures(void)
/* Initialise textures needed to properly display the castle */
{
    initialise_textures(&pillar_texture, "gold_energy.bmp");
    initialise_textures(&wall_texture, "gold_energy.bmp");
}


void draw_octagon(int x, int y, int z)
/* Takes the normal vector to the octagon and draws an octagon at with 
 * centre at (0, 0, 0) and with each side having length 1 */
{
    glPushMatrix();
        glRotatef(atan(0.5 / (0.5 + sqrt(0.5))) * (180 / M_PI), 0, 0, 1);
        glTranslatef(-(0.5 + sqrt(0.5)), -(0.5 + sqrt(0.5)), 0); //Moves the centre of the octagon to 0, 0, 0
        glTranslatef(0, -(1 - sqrt(0.5)), 0); //Moves the octagon down so that the bottom of the octagon touches the x axis
        glBegin(GL_POLYGON); 
            glNormal3f(x, y, z);
            glVertex3f(0, 1, 0);
            glVertex3f(0, 2, 0);
            glVertex3f(sqrt(0.5), 2 + sqrt(0.5), 0);
            glVertex3f(sqrt(0.5) + 1, 2 + sqrt(0.5), 0);
            glVertex3f(sqrt(0.5) * 2 + 1, 2, 0);
            glVertex3f(sqrt(0.5) * 2 + 1, 1, 0);
            glVertex3f(sqrt(0.5) + 1, 1 - sqrt(0.5), 0);
            glVertex3f(sqrt(0.5), 1 - sqrt(0.5), 0);
        glEnd();
    glPopMatrix();
}



void draw_pillar (int height)
/* Draws a closed octogonal cylinder */
{
    //Draws the bottom of the pillar (an octagon)
    glPushMatrix();
        glTranslatef(0, 0, 0.01);
        draw_octagon(0, 0, -1);
    glPopMatrix();
    
    //draws the sides of the pillar
    GLUquadricObj* p = gluNewQuadric();
    glEnable(GL_TEXTURE_2D); //Enable texturing 
    enable_quadratic_textures(&pillar_texture, p);
    
    glPushMatrix();
        glRotatef(0, 1, 0, 0);
        gluCylinder(p, sqrt(0.5) + 0.6, sqrt(0.5) + 0.6, height, 8, 8);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    
    //Draws the top of the pillar (an octagon)
    glPushMatrix();
        glTranslatef(0, 0, 9.99);
        draw_octagon(0, 0, 1);
    glPopMatrix();
}



void draw_castle_minor_pillars(void)
/* Draws 4 minor pillars that surround the back of the castle */
{
    glPushMatrix();
        glTranslatef(-7.5, 0, -13);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(10);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(15 + 7.5, 0, -13);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(10);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0, 0, -13 * 2);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(10);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(15, 0, -13 * 2);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(10);
    glPopMatrix();
}


void draw_single_castle_wall(void)
/* Draws one single castle wall with textures added. */
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wall_texture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glBegin(GL_QUADS);
        //front
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0); glVertex3f(-15/2.0, -9/2.0, 0);
        glTexCoord2f(1, 0); glVertex3f(15/2.0, -9/2.0, 0);
        glTexCoord2f(1, 1); glVertex3f(15/2.0, 9/2.0, 0);
        glTexCoord2f(0, 1); glVertex3f(-15/2.0, 9/2.0, 0);
        
        //left side
        glNormal3f(-1, 0, 0);
        glVertex3f(-15/2.0, -9/2.0, 0);
        glVertex3f(-15/2.0, 9/2.0, 0);
        glVertex3f(-15/2.0, 9/2.0, -1);
        glVertex3f(-15/2.0, -9/2.0, -1);
        
        //back
        glNormal3f(0, 0, -1);
        glTexCoord2f(0, 0); glVertex3f(-15/2.0, -9/2.0, -1);
        glTexCoord2f(1, 0); glVertex3f(15/2.0, -9/2.0, -1);
        glTexCoord2f(1, 1); glVertex3f(15/2.0, 9/2.0, -1);
        glTexCoord2f(0, 1); glVertex3f(-15/2.0, 9/2.0, -1);
        
        //right side
        glNormal3f(1, 0, 0);
        glVertex3f(15/2.0, -9/2.0, 0);
        glVertex3f(15/2.0, 9/2.0, 0);
        glVertex3f(15/2.0, 9/2.0, -1);
        glVertex3f(15/2.0, -9/2.0, -1);
        
        //top
        glNormal3f(0, 1, 0);
        glTexCoord2f(1, 0); glVertex3f(-15/2.0, 9/2.0, 0);
        glTexCoord2f(0, 0); glVertex3f(-15/2.0, 9/2.0, -1);
        glTexCoord2f(0, 1); glVertex3f(15/2.0, 9/2.0, -1);
        glTexCoord2f(1, 1); glVertex3f(15/2.0, 9/2.0, 0);
        
        //bottom
        glNormal3f(0, -1, 0);
        glVertex3f(-15/2.0, -9/2.0, 0);
        glVertex3f(-15/2.0, -9/2.0, -1);
        glVertex3f(15/2.0, -9/2.0, -1);
        glVertex3f(15/2.0, -9/2.0, 0);
        
    glEnd();
    glDisable(GL_TEXTURE_2D);
}



void draw_castle_walls(void)
/* Draws the surrounding castle walls and supporting pillars which 
 * surround the castle. */
{
    //Drawing the surrounding small pillars
    draw_castle_minor_pillars();
    
    //Drawing connecting walls
    glPushMatrix();
        glRotatef(90 + asin(7.5 / 15) * (180 / M_PI), 0, 1, 0);
        glTranslatef(15/2.0, 9/2.0, 0);
        glColor3f(1.0, 0.0, 0.0);
        draw_single_castle_wall();
    glPopMatrix();
    
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
        glTranslatef(0, 0, -13 * 2);
        glRotatef(-90 - asin(7.5 / 15) * (180 / M_PI), 0, 1, 0);
        glTranslatef(15/2.0, 9/2.0, 0);
        glColor3f(1.0, 0.0, 0.0);
        draw_single_castle_wall();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0, 0, -13 * 2);
        glTranslatef(15/2.0, 9/2.0, 0);
        glColor3f(1.0, 0.0, 0.0);
        draw_single_castle_wall();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(15, 0, -13 * 2);
        glRotatef(-90 + asin(7.5 / 15) * (180 / M_PI), 0, 1, 0);
        glTranslatef(15/2.0, 9/2.0, 0);
        glColor3f(1.0, 0.0, 0.0);
        draw_single_castle_wall();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(15 + 7.5, 0, -13);
        glRotatef(-90 - asin(7.5 / 15) * (180 / M_PI), 0, 1, 0);
        glTranslatef(15/2.0, 9/2.0, 0);
        glColor3f(1.0, 0.0, 0.0);
        draw_single_castle_wall();
    glPopMatrix();
}



void draw_castle(const float x_castle[], const float y_castle[], const float z_castle[], const int t1_castle[], const int t2_castle[], const int t3_castle[], int num_triangles_castle) 
/* Draws the castle using glut objects and off_files.*/
{
    /* *** ENTRANCE *** */
    //Right entrance pillar
    glPushMatrix();
        glColor3f(0, 0, 1.0);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(14);
    glPopMatrix();
    
    //castle castle entrance
    glPushMatrix();
        glColor3f(0, 0, 0.6);
        glTranslatef((sqrt(0.5) + 0.6) / 2, 0, (sqrt(0.5) + 0.6) / 2);
        draw_off_file(x_castle, y_castle, z_castle, t1_castle, t2_castle, t3_castle, num_triangles_castle);
    glPopMatrix();
    
    //Left entrance pillar
    glPushMatrix();
        glColor3f(0, 0, 1.0);
        glTranslatef(15, 0, 0);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(14);
    glPopMatrix();
    
    /* *** WALLS *** */ 
    draw_castle_walls();
}
