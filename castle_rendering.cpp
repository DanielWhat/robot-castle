#define GL_SILENCE_DEPRECATION

#include <GL/freeglut.h>
#include <math.h>
#include <stdlib.h>     
#include <time.h>       
#include "open_off.h"
#include "textures.h"
#include "spaceship.h"
#include "particle.h"

#include <iostream>
#include <fstream>

static GLuint pillar_texture;
static GLuint wall_texture;
static GLuint grass_texture;

static int x_tree_cord[50] = {0};
static int z_tree_cord[50] = {0};

void initialise_castle_textures(void)
/* Initialise textures and other things needed to properly display the 
 * castle */
{
    initialise_textures(&pillar_texture, "./bricks/old_bricks.bmp");
    initialise_textures(&wall_texture, "./bricks/wall.bmp");
    initialise_textures_tga(&grass_texture, "./diffus.tga");
    
    //generate 50 random tree locations
    srand(time(0));
    
    for (int i = 0; i < 50; i++) {
        x_tree_cord[i] = rand() % 500 - 250;
        z_tree_cord[i] = rand() % 500 - 250;
    }
}


void draw_single_grass(void)
/* Draws a small grass patch intended to surround the castle */
{
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, grass_texture);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0); glVertex3f(-2.5, 0, 0);
        glTexCoord2f(1, 0); glVertex3f(2.5, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(2.5, 5, 0);
        glTexCoord2f(0, 1); glVertex3f(-2.5, 5, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}


void draw_grass (void) 
{
    for (int i = 0; i < 50; i++) {
        glPushMatrix();
            glTranslatef(x_tree_cord[i], 0, z_tree_cord[i]); 
            glPushMatrix();
                draw_single_grass();
            glPopMatrix();
            
            glPushMatrix();
                glRotatef(90, 0, 1, 0);
                draw_single_grass();
            glPopMatrix();
        glPopMatrix();
    }    
}


void draw_pot(void)
/* Draws a pot for the fountain to go in */
{
    
    float x[21] = {3.9, 4.2, 4.6, 4.8, 4.95, 5, 4.75, 4.4, 4.1, 3.9, 4, 4.25, 5, 5.5, 6, 6.2, 6, 5.5, 5, 4.5, 4.5};
    float y[21] = {8, 9.1, 9.8, 10.1, 10.2, 10, 9.7, 9, 8, 6.5, 5, 4.5, 4, 3.6, 3, 2.5, 1.6, 1, 0.7, 0.2, 0};
    float z[21] = {0};
    float new_x[19], new_y[19], new_z[19];
    
    //Do the following enugh times to complete an entire rotation
    for (int j = 0; j < 360; j+=5) {
        
        //Create the new x, y, z coordinates. The new coords are a small rotation about y axis of the previous ones 
        for (int i = 0; i < 19; i++) {
            new_x[i] = cos(5 * (M_PI/180)) * x[i] + sin(5 * (M_PI/180)) * z[i];
            new_y[i] = y[i];
            new_z[i] = -sin(5 * (M_PI/180)) * x[i] + cos(5 * (M_PI/180)) * z[i];
        }
        
        //Join new x, y, z and old x, y, z to make a triangle strip which will be a small "slice" of the spaceship
        glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i < 19; i++) {
                if (i > 0) {
                    normalm(new_x[i-1], new_y[i-1], new_z[i-1],
                            x[i-1], y[i-1], z[i-1],
                            x[i], y[i], z[i]);
                }
                glVertex3f(x[i], y[i], z[i]);
                
                if (i > 0) {
                    normalm(new_x[i-1], new_y[i-1], new_z[i-1],
                            x[i], y[i], z[i],
                            new_x[i], new_y[i], new_z[i]);
                }
                glVertex3f(new_x[i], new_y[i], new_z[i]);
            }
        glEnd();
        
        //Before we move on to the next stage, let the old x, y, z be equal to the new x, y, z
        for (int i = 0; i < 19; i++) {
            x[i] = new_x[i];
            y[i] = new_y[i];
            z[i] = new_z[i];
        }
    }
}

void draw_pot_shadow(void) 
{
    
    
    
    
}


void draw_fountain (Particle* particle_list[], int num_particles, int camera_angle)
/* draws the fountain and updates the water particles */
{
    update_particles(particle_list, num_particles);
    
    glPushMatrix();
        //Draw fountain
        glPushMatrix();
            glTranslatef(0, -0.2, 0); // move it down slightly so that you can't see that water at the bottom
            glScalef(0.5, 0.5, 0.5);
            glColor3f(242/255.0, 225/255.0, 203/255.0);
            draw_pot();
        glPopMatrix();
        
        //draw water particles
        for (int i = 0; i < num_particles; i++) {
            glPushMatrix();
                glColor3f(86/255.0, 145/255.0, 239/255.0);
                glTranslatef(particle_list[i]->x, particle_list[i]->y, particle_list[i]->z); 
                glRotatef(90 - camera_angle, 0, 1, 0);
                glBegin(GL_QUADS);
                    glVertex3f(0, 0, 0);
                    glVertex3f(0.1, 0, 0);
                    glVertex3f(0.1, 0.1, 0);
                    glVertex3f(0, 0.1, 0);
                glEnd();
            glPopMatrix();
        }
    glPopMatrix(); 
    
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
        glColor3f(0.7, 0.7, 0.7);
        draw_single_castle_wall();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0, 0, -13 * 2);
        glRotatef(-90 - asin(7.5 / 15) * (180 / M_PI), 0, 1, 0);
        glTranslatef(15/2.0, 9/2.0, 0);
        glColor3f(0.7, 0.7, 0.7);
        draw_single_castle_wall();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0, 0, -13 * 2);
        glTranslatef(15/2.0, 9/2.0, 0);
        glColor3f(0.7, 0.7, 0.7);
        draw_single_castle_wall();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(15, 0, -13 * 2);
        glRotatef(-90 + asin(7.5 / 15) * (180 / M_PI), 0, 1, 0);
        glTranslatef(15/2.0, 9/2.0, 0);
        glColor3f(0.7, 0.7, 0.7);
        draw_single_castle_wall();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(15 + 7.5, 0, -13);
        glRotatef(-90 - asin(7.5 / 15) * (180 / M_PI), 0, 1, 0);
        glTranslatef(15/2.0, 9/2.0, 0);
        glColor3f(0.7, 0.7, 0.7);
        draw_single_castle_wall();
    glPopMatrix();
}



void draw_castle(const float x_castle[], const float y_castle[], const float z_castle[], const int t1_castle[], const int t2_castle[], const int t3_castle[], int num_triangles_castle) 
/* Draws the castle using glut objects and off_files.*/
{
    /* *** ENTRANCE *** */
    //Right entrance pillar
    glPushMatrix();
        glColor3f(0.7, 0.7, 0.7);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(14);
    glPopMatrix();
    
    //castle castle entrance
    glPushMatrix();
        glColor3f(0.5, 0.5, 0.5);
        glTranslatef((sqrt(0.5) + 0.6) / 2, 0, (sqrt(0.5) + 0.6) / 2);
        draw_off_file(x_castle, y_castle, z_castle, t1_castle, t2_castle, t3_castle, num_triangles_castle);
    glPopMatrix();
    
    //Left entrance pillar
    glPushMatrix();
        glColor3f(0.7, 0.7, 0.7);
        glTranslatef(15, 0, 0);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(14);
    glPopMatrix();
    
    /* *** WALLS *** */ 
    draw_castle_walls();
}
