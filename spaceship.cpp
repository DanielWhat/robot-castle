#define GL_SILENCE_DEPRECATION

#include <GL/freeglut.h>
#include <math.h>
#include <stdbool.h>
#include "spaceship.h"
#include "textures.h"


static GLuint spaceship_texture;

/* @@@ */
static float white_s[4] = {1.0, 1.0, 1.0, 1.0};
static float black_s[4] = {0.0, 0.0, 0.0, 1.0};


void normalm(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 )
{
    float nx, ny, nz;
    nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
    ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
    nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);
    glNormal3f(nx, ny, nz);
}



void initialise_spaceship(int light)
/* Initialise textures and lighting needed to properly display the 
 * spaceship */
{
    float white[4] = {1.0, 1.0, 1.0, 1.0};
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    
    initialise_textures(&spaceship_texture, "13777.bmp");
    
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white); 
    glLightfv(GL_LIGHT2, GL_SPECULAR, white); 
       
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 70); //this determines size of the spotlight
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1.0); //this determines essentially the luminocity of the spotlight
}



/*void normalf(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float nx = y1 * z2 - y2 * z1;
    float ny = -(x1 * z2 - x2 * z1);
    float nz = x1 * y2 - x2 * y1;
    
    glNormal3f(-nx, ny, nz); 
}  */


void normalf(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3 )
{
    float nx, ny, nz;
    nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
    ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
    nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);
    glNormal3f(-nx, -ny, -nz);
}



float my_round(float x, int decimal_places)
{
    if (x < 1 / pow(10, decimal_places)) {
        x = 0;
    }
    return floorf(x * pow(10, decimal_places)) / pow(10, decimal_places);
}



void draw_paraboloid(int height)
{
    
    glPushMatrix();
        for (float y = 0; y < height-1; y += 0.1) {
            glBegin(GL_TRIANGLE_STRIP);
            
                float x = -sqrt(height - (y+1));
                float x_incr = 0.05;
                
                //Graphing for positive z
                for (x = -sqrt(height - (y+1)); x*x + (y+1) < height + x_incr/10; x += x_incr) {
                    if (x > -sqrt(height - (y+1))) {
                        float prev_x = x - x_incr;
                        normalf(prev_x, y-0.05,  sqrt(height - prev_x * prev_x - (y-0.05)), prev_x, y+1, sqrt(height - prev_x * prev_x - (y+1)), x, y+1, sqrt(my_round(height - x * x - (y+1), 5)));
                    }

                    glVertex3f(x, y+1, sqrt(my_round(height - x * x - (y+1), 5)));
                    
                    if (x > -sqrt(height - (y+1))) {
                        float prev_x = x - x_incr;
                        normalf(prev_x, y+1, sqrt(height - prev_x * prev_x - (y+1)), x, y+1, sqrt(my_round(height - x * x - (y+1), 5)), x, y-0.05,  sqrt(height - x * x - (y-0.05)));
                    }
                    glVertex3f(x, y-0.05,  sqrt(height - x * x - (y-0.05)));
                }
                
                normalf(x-0.01, y-0.05,  sqrt(height - (x-0.01) * (x-0.01) - (y-0.05)), (x-0.01), y+1, sqrt(height - (x-0.01) * (x-0.01) - (y+1)), x, y-0.05,  0);
                glVertex3f(x, y-0.05,  0);
                glVertex3f(x, y+1,  0);
            glEnd();
        }
    glPopMatrix();
}



void draw_spaceship_legs(void)
{
    glMaterialfv(GL_FRONT, GL_SPECULAR, white_s);
    
    glPushMatrix();
    glScalef(0.278, 0.278, 0.278);
    
        glPushMatrix();
            glColor3f(0.6, 0.6, 0.6);
            glRotatef(180, 0, 1, 0);
            glTranslatef(0, 10, 0);
            glRotatef(180, 1, 0, 0);
            draw_paraboloid(10);
        glPopMatrix();
        
        glPushMatrix();
            glColor3f(0.6, 0.6, 0.6);
            glTranslatef(0, 10, 0.1);
            glRotatef(180, 1, 0, 0);
            draw_paraboloid(10);
        glPopMatrix();
        
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, black_s);
    
}



void draw_spaceship_body (void) 
/* Draws a spaceship body using surfaces of rotation method. Also adds a 
 * modulated texture to the spaceship body. */
{
    float x[19] = {0, 2, 3.1, 4, 4.6, 4.75, 5, 5.7, 7, 10, 11.2, 11.8, 12, 11.8, 11.5, 11, 10, 7, 0};
    float y[19] = {10, 9.85, 9.6, 9, 7.6, 6, 5.6, 5.3, 5.1, 4.8, 4.6, 4.3, 4, 3.4, 3, 2.7, 2.4, 2.3, 2.1};
    float z[19] = {0};
    float new_x[19], new_y[19], new_z[19];
    
    float y_tex_cord[19] = {0, 0.07183167024826759, 0.11223310925819929, 0.15097322102694813, 0.20552537848006547, 0.26308104711811175, 0.2799750648682411, 0.30725114359479977, 0.3543587448246648, 0.4623403751161782, 0.505911502392443, 0.5299370997984082, 0.5428504703879912, 0.5655020208517456, 0.5834096438516726, 0.6042933415043925, 0.6416855560974432, 0.7491909696017439, 1.0};
    
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spaceship_texture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    int counter = 0;
    
    //Do the following enugh times to complete an entire rotation
    for (int j = 0; j < 365; j+=5) {
        
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
                
                glTexCoord2f((counter + 1)/3, y_tex_cord[i]);
                glVertex3f(x[i], y[i], z[i]);
                
                if (i > 0) {
                    normalm(new_x[i-1], new_y[i-1], new_z[i-1],
                            x[i], y[i], z[i],
                            new_x[i], new_y[i], new_z[i]);
                }
                glTexCoord2f(counter/3, y_tex_cord[i]);
                glVertex3f(new_x[i], new_y[i], new_z[i]);
            }
        glEnd();
        
        //Before we move on to the next stage, let the old x, y, z be equal to the new x, y, z
        for (int i = 0; i < 19; i++) {
            x[i] = new_x[i];
            y[i] = new_y[i];
            z[i] = new_z[i];
        }
        
        counter++;
        
        if (counter >= 3) {
            counter = 0;
        }
    }
    glDisable(GL_TEXTURE_2D);
    
}



void draw_spaceship(Spaceship spaceship, int light)
/* Draws a spaceship body, legs and lights. Takes a spaceship object and
 * light parameter (i.e GL_LIGHT2) */
{
    float light_pos[4] = {0, 1, 0, 1};
    float light_dir[4] = {0, -1, 0, 0};
    
    glPushMatrix();
        glTranslatef(0, spaceship.height, 0);
        glRotatef(spaceship.angle, 0, 1, 0);
        
        if (spaceship.is_lights_on) {
            glEnable(light);
            glLightfv(light, GL_SPOT_DIRECTION, light_dir);
            glLightfv(light, GL_POSITION, light_pos); //"draw" light
            
        } else {
            glDisable(light);
        
        }
        
        draw_spaceship_body();
        
        //Draw legs
        glPushMatrix();
            glPushMatrix();
                glTranslatef(9, spaceship.leg_height, 0);
                draw_spaceship_legs();
            glPopMatrix();
            
            glPushMatrix();
                glTranslatef(-9, spaceship.leg_height, 0);
                draw_spaceship_legs();
            glPopMatrix();
            
            glPushMatrix();
                glTranslatef(0, spaceship.leg_height, -9);
                draw_spaceship_legs();
            glPopMatrix();
            
            glPushMatrix();
                glTranslatef(0, spaceship.leg_height, 9);
                draw_spaceship_legs();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

