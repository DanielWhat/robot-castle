#define GL_SILENCE_DEPRECATION 0

#include <math.h>
#include <stdio.h>
#include <climits>
#include <stdbool.h>
#include <GL/freeglut.h>
#include "open_off.h"
#include "castle_rendering.h"
#include "cannon.h"
#include "robot.h"
#include "animations.h"
#include "spaceship.h"

//For debugging @@
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
float red_s[4] = {1.0, 0.0, 0.0, 1.0};
float dark_red_s[4] = {0.4, 0.0, 1.0, 1.0};

// ********************* FOR OFF FILE LOADING *********************
//Castle Entrance Variables
float *x_castle, *y_castle, *z_castle;  //vertex coordinate arrays
int *t1_castle, *t2_castle, *t3_castle; //triangles
int nvrt_castle, ntri_castle;    //total number of vertices and triangles

//Cannon Variables
float *x_cannon, *y_cannon, *z_cannon;
int *t1_cannon, *t2_cannon, *t3_cannon;
int nvrt_cannon, ntri_cannon;

Robot robot_1 = {.x = -10, .y = 0, .z = 10, .angle_y = 90, .angle_z = 0, .angle_x = 0, .left_arm = {.humerus_side_angle = 0, .humerus_forward_angle = 0}, .right_arm = {.humerus_side_angle = 0, .humerus_forward_angle = 0},.left_eye = {.r = 1, .g = 0, .b = 0}, .right_eye = {.r = 0, .g = 0, .b = 1}};  
Robot robot_2 = {.x = -10, .y = 0, .z = 30, .angle_y = 180, .angle_z = 0, .angle_x = 15, .left_arm = {.humerus_side_angle = 0, .humerus_forward_angle = -95}, .right_arm = {.humerus_side_angle = 0, .humerus_forward_angle = -95},.left_eye = {.r = 1, .g = 0, .b = 0}, .right_eye = {.r = 0, .g = 0, .b = 1}};  


Spaceship spaceship = {.angle = 0, .height = 0, .leg_height = 0, .is_lights_on = true};

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


void animation_selector(int selector)
{
    if (selector == 0) {
        animate_patrol_robot(&robot_1, animation_selector);
        
    } else if (selector == 1) {
        animate_spaceship_takeoff(&spaceship, animation_selector, 1);
        
    } else if (selector == 2) {
        animate_passive_spaceship(&spaceship, animation_selector, 2);
        
    } else if (selector == 3) {
        animate_worker_robot(&robot_2, animation_selector, 3);
        
    }
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
    
    
    //Patrol Robot
    glPushMatrix();
        glTranslatef(robot_1.x, robot_1.y, robot_1.z);
        glRotatef(robot_1.angle_y, 0, 1, 0);
        glRotatef(robot_1.angle_z, 0, 0, 1);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlight_direction);
        glLightfv(GL_LIGHT1, GL_POSITION, spotlight_position);
        glTranslatef(0, 1, 0); //Move up to origin
        draw_robot(robot_1);
    glPopMatrix();
    
    
    //Worker Robot
    glPushMatrix();
        glTranslatef(robot_2.x, robot_2.y, robot_2.z);
        glRotatef(robot_2.angle_y, 0, 1, 0);
        glPushMatrix();
            glColor3f(0, 0, 1.0);
            glTranslatef(0, 0, 3);
            glTranslatef(0, 2, 0); //Move up to origin
            glScalef(1, 2, 1);
            glutSolidCube(2.0);
        glPopMatrix();
        glRotatef(robot_2.angle_x, 1, 0, 0);
        glTranslatef(0, 1, 0); //Move up to origin
        draw_robot(robot_2);
    glPopMatrix();
    
    
    glPushMatrix();
        glTranslatef(37, 0, -30);
        glColor3f(0.8, 0.8, 0.8);
        draw_spaceship(spaceship, GL_LIGHT2);
    glPopMatrix();
    
    
    glPushMatrix();
        draw_axis();
    glPopMatrix();
    
    
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
    
    glEnable(GL_LIGHTING); //Turn on lights
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1); //this will be a spotlight
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey); //the passive light coming from all directions is a light grey
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white); //white light
    glLightfv(GL_LIGHT0, GL_SPECULAR, white); //white light
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white); 
    glLightfv(GL_LIGHT1, GL_SPECULAR, white); 
    
    initialise_castle_textures();
    initialise_spaceship(GL_LIGHT2);
    
    //convert to spotlight
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15); //this determines size of the spotlight
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100.0); //this determines essentially the luminocity of the spotlight
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); //Let ambient and diffuse colours track the current colour
    glEnable(GL_COLOR_MATERIAL); //Enable the above setting
    
    //glMaterialfv(GL_FRONT, GL_SPECULAR, white); //Make the specular color white for all objects?
    glMaterialf(GL_FRONT, GL_SHININESS, 20); //Make the radius of the specular highlight large-ish
    
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
        
    } else if (key == 's') {
        animation_selector(1);
        
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
    animation_selector(0);
    animation_selector(2);
    animation_selector(3);
    glutMainLoop();
    return 0;
}
