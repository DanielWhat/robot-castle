#define GL_SILENCE_DEPRECATION 0

#include <math.h>
#include <stdio.h>
#include <climits>
#include <GL/freeglut.h>
#include "open_off.h"
#include "castle_rendering.h"
#include "cannon.h"
#include "robot.h"
#include "textures.h"
#include "animations.h"

//For debugging
#include <iostream>
#include <fstream>

int angle = -90;
int camera_x_offset = 0;
int camera_z_offset = 0;

float dx = 0;
float dz = 0;
float camx = 7.5;
float camz = 30;

float white[4] = {1.0, 1.0, 1.0, 1.0};
float grey[4] = {0.2, 0.2, 0.2, 1.0};
float black[4] = {0.0, 0.0, 0.0, 1.0};

// ********************* FOR OFF FILE LOADING *********************
//Castle Entrance Variables
float *x_castle, *y_castle, *z_castle;  //vertex coordinate arrays
int *t1_castle, *t2_castle, *t3_castle; //triangles
int nvrt_castle, ntri_castle;    //total number of vertices and triangles

//Cannon Variables
float *x_cannon, *y_cannon, *z_cannon;
int *t1_cannon, *t2_cannon, *t3_cannon;
int nvrt_cannon, ntri_cannon;

GLuint spaceship_texture;

Robot robot_1 = {.x = -10, .y = 0, .z = 10, .angle_y = 90, .angle_z = 0, .left_arm = {.humerus_side_angle = 0, .humerus_forward_angle = 0}, .right_arm = {.humerus_side_angle = 0, .humerus_forward_angle = 0},.left_eye = {.r = 1, .g = 0, .b = 0}, .right_eye = {.r = 0, .g = 0, .b = 1}};  


void draw_floor(void)
{
	//float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glColor4f(0.0, 0.7, 0.0, 1.0);  //The floor is green in colour
	glNormal3f(0.0, 1.0, 0.0);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);

	//The floor is made up of several tiny squares on a 200x200 grid. Each square has a unit size. //@@ change to 1600 for production
	glBegin(GL_QUADS);
	for(int i = -400; i < 400; i++)
	{
		for(int j = -400;  j < 400; j++)
		{
			glVertex3f(i, 0, j);
			glVertex3f(i, 0, j+1);
			glVertex3f(i+1, 0, j+1);
			glVertex3f(i+1, 0, j);
		}
	}
	glEnd();
    //glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}


void special (int key, int x, int y) 
{
    if (key == GLUT_KEY_RIGHT) {
        angle += 5;
        
    } else if (key == GLUT_KEY_LEFT) {
        angle -= 5;
        
    } else if (key == GLUT_KEY_UP) {
        camera_z_offset -= 3;
        camx += dx;
        camz += dz;
        
    } else if (key == GLUT_KEY_DOWN) {
        camera_z_offset += 3;
        camx += -dx;
        camz += -dz;
    }

    glutPostRedisplay();
}



void draw_axis (void)
{
    //x axis
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINES);
        glVertex3f(-40.0, 0.0f, 0.0f);
        glVertex3f(40.0, 0.0f, 0.0f);
    glEnd();
    glFlush();
 
    //y axis
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, -40.0f, 0.0f);
        glVertex3f(0.0, 40.0f, 0.0f);
    glEnd();
    glFlush();
 
    //z axis
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0f ,-40.0f );
        glVertex3f(0.0, 0.0f ,40.0f );
    glEnd();
    glFlush();
}


void animate_robots(int selector)
{
    if (selector == 0) {
        
        animate_patrol_robot(&robot_1, animate_robots);
    }
}



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



void draw_spaceship(void)
{
    float x[19] = {0, 2, 3.1, 4, 4.6, 4.75, 5, 5.7, 7, 10, 11.2, 11.8, 12, 11.8, 11.5, 11, 10, 7, 0};
    float y[19] = {10, 9.85, 9.6, 9, 7.6, 6, 5.6, 5.3, 5.1, 4.8, 4.6, 4.3, 4, 3.4, 3, 2.7, 2.4, 2.3, 2.1};
    float z[19] = {0};
    float new_x[19], new_y[19], new_z[19];
    
    float y_tex_cord[19] = {0, 0.07183167024826759, 0.11223310925819929, 0.15097322102694813, 0.20552537848006547, 0.26308104711811175, 0.2799750648682411, 0.30725114359479977, 0.3543587448246648, 0.4623403751161782, 0.505911502392443, 0.5299370997984082, 0.5428504703879912, 0.5655020208517456, 0.5834096438516726, 0.6042933415043925, 0.6416855560974432, 0.7491909696017439, 1.0};
    
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spaceship_texture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    for (int j = 0; j < 365; j+=5) {
        for (int i = 0; i < 19; i++) {
            new_x[i] = cos(5 * (M_PI/180)) * x[i] + sin(5 * (M_PI/180)) * z[i];
            new_y[i] = y[i];
            new_z[i] = -sin(5 * (M_PI/180)) * x[i] + cos(5 * (M_PI/180)) * z[i];
        }
        
        glColor3f(0, 1.0, 0);
        glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i < 19; i++) {
                if (i > 0) {
                    normalm(new_x[i-1], new_y[i-1], new_z[i-1],
                            x[i-1], y[i-1], z[i-1],
                            x[i], y[i], z[i]);
                }
                
                glTexCoord2f(1, y_tex_cord[i]);
                glVertex3f(x[i], y[i], z[i]);
                
                if (i > 0) {
                    normalm(new_x[i-1], new_y[i-1], new_z[i-1],
                            x[i], y[i], z[i],
                            new_x[i], new_y[i], new_z[i]);
                }
                glTexCoord2f(0, y_tex_cord[i]);
                glVertex3f(new_x[i], new_y[i], new_z[i]);
            }
        glEnd();
        
        for (int i = 0; i < 19; i++) {
            x[i] = new_x[i];
            y[i] = new_y[i];
            z[i] = new_z[i];
        }
    }
    glDisable(GL_TEXTURE_2D);
}


void display (void)
{
    float light_position[4] = {0, 20, 10, 1};
    float spotlight_position[4] = {0, 7, -10, 1};
    float spotlight_direction[4] = {0, -1, 3, 0};
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    dx = cos(angle * (M_PI/180));
    dz = sin(angle * (M_PI/180));
    
    gluLookAt(camx, 10, camz, camx+dx, 10, camz+dz, 0.0, 1.0, 0.0);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    //Robot 1
    glPushMatrix();
        glTranslatef(robot_1.x, robot_1.y, robot_1.z);
        glRotatef(robot_1.angle_y, 0, 1, 0);
        glRotatef(robot_1.angle_z, 0, 0, 1);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlight_direction);
        glLightfv(GL_LIGHT1, GL_POSITION, spotlight_position);
        glTranslatef(0, 1, 0); //Move up to origin
        draw_robot(robot_1);
    glPopMatrix();
    
    glPushMatrix();
        draw_axis();
    glPopMatrix();
    
    //glDisable(GL_LIGHTING);
    
    
    //glEnable(GL_LIGHTING);
    
    glPushMatrix();
        glTranslatef(20, 0, 0);
        glScalef(2, 2, 2);
        draw_castle(x_castle, y_castle, z_castle, t1_castle, t2_castle, t3_castle, ntri_castle);
    glPopMatrix();
    
    //Enable specular lighting for cannon and draw cannon
    glPushMatrix();
        glTranslatef(10, 1, 10);
        glScalef(0.06, 0.06, 0.06);
        draw_cannon(x_cannon, y_cannon, z_cannon, t1_cannon, t2_cannon, t3_cannon, ntri_cannon);
    glPopMatrix();
    
    /*glPushMatrix();
        glTranslatef(-4, -0.5, 0);
        glRotatef(29, 0, 1, 0);
        glRotatef(-90, 1, 0, 0);
        glScalef(0.3, 0.3, 0.3);
        draw_paraboloid(25);
    glPopMatrix();*/
    
    glPushMatrix();
        glTranslatef(30, 0, 0);
        draw_spaceship();
    glPopMatrix();
    
    //Make sure floor is drawn before any lighting
    glPushMatrix();
        glScalef(0.5, 1, 0.5);  //@@ change to 0.125 for production
        glTranslatef(0, -1, 0);
        draw_floor();
    glPopMatrix();
    
    glutSwapBuffers();
}



void initialize (void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    load_mesh_file("./castle_gate.off", &x_castle, &y_castle, &z_castle, &t1_castle, &t2_castle, &t3_castle, &nvrt_castle, &ntri_castle);	
    load_mesh_file("./Cannon.off", &x_cannon, &y_cannon, &z_cannon, &t1_cannon, &t2_cannon, &t3_cannon, &nvrt_cannon, &ntri_cannon);
    
    initialise_castle_textures();
    initialise_textures(&spaceship_texture, "13777.bmp");
    
    glEnable(GL_LIGHTING); //Turn on lights
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1); //this will be a spotlight
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey); //the passive light coming from all directions is a light grey
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white); //white light
    glLightfv(GL_LIGHT0, GL_SPECULAR, white); //white light
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white); 
    glLightfv(GL_LIGHT1, GL_SPECULAR, white); 
    
    //convert to spotlight
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15); //this determines size of the spotlight
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100.0); //this determines essentially the luminocity of the spotlight
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); //Let ambient and diffuse colours track the current colour
    glEnable(GL_COLOR_MATERIAL); //Enable the above setting
    
    //glMaterialfv(GL_FRONT, GL_SPECULAR, white); //Make the specular color white for all objects?
    glMaterialf(GL_FRONT, GL_SHININESS, 30); //Make the radius of the specular highlight large-ish
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);
}



void keyboard(unsigned char key, int x, int y)
{
    if (key == 'c') {
        fire_cannon();
        
    } else if (key == 'w') {
        cannon_angle += (cannon_angle < 86) ? 2 : 0;
        update_cannonball_position();
        glutPostRedisplay();
        
    } else if (key == 'a') {
        cannon_angle -= (cannon_angle > -10) ? 2 : 0;
        update_cannonball_position();
        glutPostRedisplay();
    }
}



int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Assignment One");
    
    initialize();
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    animate_robots(0);
    glutMainLoop();
    return 0;
}
