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
#include "textures.h"
#include "particle.h"
#include <stdlib.h>
#include <time.h>

//#include "loadBMP.h"

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

//for camera toggle
bool is_default_cam = true;

//Cannon Variables
float *x_cannon, *y_cannon, *z_cannon;
int *t1_cannon, *t2_cannon, *t3_cannon;
int nvrt_cannon, ntri_cannon;

//Objects
Robot robot_1 = {.x = -10, .y = 0, .z = 10, .angle_y = 90, .angle_z = 0, .angle_x = 0, .left_arm = {.humerus_side_angle = 0, .humerus_forward_angle = 0}, .right_arm = {.humerus_side_angle = 0, .humerus_forward_angle = 0},.left_eye = {.r = 1, .g = 0, .b = 0}, .right_eye = {.r = 0, .g = 0, .b = 1}};  
Robot robot_2 = {.x = -10, .y = 0, .z = 30, .angle_y = 180, .angle_z = 0, .angle_x = 15, .left_arm = {.humerus_side_angle = 0, .humerus_forward_angle = -95}, .right_arm = {.humerus_side_angle = 0, .humerus_forward_angle = -95},.left_eye = {.r = 1, .g = 0, .b = 0}, .right_eye = {.r = 0, .g = 0, .b = 1}};  
Robot robot_3 = {.x = 35, .y = 0, .z = -5, .angle_y = 0, .angle_z = 0, .angle_x = 0, .left_arm = {.humerus_side_angle = 0, .humerus_forward_angle = 0}, .right_arm = {.humerus_side_angle = 0, .humerus_forward_angle = 0},.left_eye = {.r = 1, .g = 0, .b = 0}, .right_eye = {.r = 0, .g = 0, .b = 1}};  

Spaceship spaceship = {.angle = 0, .height = 0, .leg_height = 0, .is_lights_on = true};

//for takeoff
bool is_spaceship_taking_off = false;

bool has_cannon_been_fired = false;

GLuint skybox_texture_ids[6];


//particles
int num_particles = 2000;
Particle* particle_list[2000];


void draw_floor(void)
{
	float black[4] = {0};
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, skybox_texture_ids[5]);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, black); // ensure no specular reflections

	//The floor is made up of several tiny squares on a 200x200 grid. Each square has a unit size.
    glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
	for(int i = -250; i < 250; i++)
	{
		for(int j = -250;  j < 250; j++)
		{
			glTexCoord2f((250 + i)/500.0, (250 + j)/500.0); glVertex3f(i, 0, j);
			glTexCoord2f((250 + i)/500.0, (250 + j+1)/500.0); glVertex3f(i, 0, j+1);
			glTexCoord2f((250 + i+1)/500.0, (250 + j+1)/500.0); glVertex3f(i+1, 0, j+1);
			glTexCoord2f((250 + i+1)/500.0, (250 + j)/500.0); glVertex3f(i+1, 0, j);
		}
	}
    glDisable(GL_TEXTURE_2D);
	glEnd();
}


void draw_skybox(void)
{
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    
    //front
    glBindTexture(GL_TEXTURE_2D, skybox_texture_ids[0]);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-250, -10, -250);
        glTexCoord2f(1, 0); glVertex3f(250, -10, -250);
        glTexCoord2f(1, 1); glVertex3f(250, 240, -250);
        glTexCoord2f(0, 1); glVertex3f(-250, 240, -250);
    glEnd();
    
    //right
    glBindTexture(GL_TEXTURE_2D, skybox_texture_ids[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(250, -10, -250);
        glTexCoord2f(1, 0); glVertex3f(250, -10, 250);
        glTexCoord2f(1, 1); glVertex3f(250, 240, 250);
        glTexCoord2f(0, 1); glVertex3f(250, 240, -250);
    glEnd();
    
    //left
    glBindTexture(GL_TEXTURE_2D, skybox_texture_ids[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 0); glVertex3f(-250, -10, -250);
        glTexCoord2f(0, 0); glVertex3f(-250, -10, 250);
        glTexCoord2f(0, 1); glVertex3f(-250, 240, 250);
        glTexCoord2f(1, 1); glVertex3f(-250, 240, -250);
    glEnd();
    
    //top
    glBindTexture(GL_TEXTURE_2D, skybox_texture_ids[3]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-250, 240, -250);
        glTexCoord2f(1, 0); glVertex3f(250, 240, -250);
        glTexCoord2f(1, 1); glVertex3f(250, 240, 250);
        glTexCoord2f(0, 1); glVertex3f(-250, 240, 250);
    glEnd();
    
    //back
    glBindTexture(GL_TEXTURE_2D, skybox_texture_ids[4]);
    glBegin(GL_QUADS);
        glColor3f(0, 1, 0);
        glTexCoord2f(1, 0); glVertex3f(-250, -10, 250);
        glTexCoord2f(0, 0); glVertex3f(250, -10, 250);
        glTexCoord2f(0, 1); glVertex3f(250, 240, 250);
        glTexCoord2f(1, 1); glVertex3f(-250, 240, 250);
    glEnd();
    
    
    //floor
    /*glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, skybox_texture_ids[5]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-250, -10, -250);
        glTexCoord2f(1, 0); glVertex3f(250, -10, -250);
        glTexCoord2f(1, 1); glVertex3f(250, -10, 250);
        glTexCoord2f(0, 1); glVertex3f(-250, -10, 250);
    glEnd();*/
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
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
        animate_patrol_robot(&robot_1, animation_selector, 0, true);
        
    } else if (selector == 1) {
        animate_spaceship_takeoff(&spaceship, animation_selector, 1);
        
    } else if (selector == 2) {
        animate_passive_spaceship(&spaceship, animation_selector, 2);
        
    } else if (selector == 3) {
        animate_worker_robot(&robot_2, animation_selector, 3, true);
        
    } else if (selector == 4) {
        animate_all_robots(&robot_1, &robot_2, animation_selector, 4);
        
    } else if (selector == 5) {
        CannonBall* cannonball = get_cannonball_pointer();
        animate_reload_robot(&robot_3, cannonball, animation_selector, 5, has_cannon_been_fired, true);
        
        if (cannonball->in_cannon) {
            reset_cannonball();
            has_cannon_been_fired = false;
        }
    }
}



void display (void)
{
    float light_position[4] = {0, 60, -40, 1};
    float spotlight_position[4] = {0, 7, -10, 1};
    float spotlight_direction[4] = {0, -1, 3, 0};
    float shadow_transformation[16] = {light_position[1], 0, 0, 0,
                                        -light_position[0], 0, -light_position[2], -1,
                                        0, 0, light_position[1], 0,
                                        0, 0, 0, light_position[1]};
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    dx = cos(angle * (M_PI/180));
    dz = sin(angle * (M_PI/180));
    
    //don't let camera leave the arena
    if (camx > 235 || camx < -235) {
        camx = (camx > 235) ? 235 : -235;
    }
    
    if (camz > 235 || camz < -235) {
        camz = (camz > 235) ? 235 : -235;
    }
    
    if (is_default_cam) {
        gluLookAt(camx, 10, camz, camx+dx, 10, camz+dz, 0.0, 1.0, 0.0);
        
    } else {
        //view from spaceship
        gluLookAt(37, spaceship.height+10, -23, 37, -1, 4, 0.0, 1.0, 0.0);
    }
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    
    
    // ***************** DRAWING SHADOWS *****************
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);
        glMultMatrixf(shadow_transformation);
        glTranslatef(37, 0, -30);
        draw_spaceship_shadow(spaceship, -1);
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);
        glMultMatrixf(shadow_transformation);
        glTranslatef(50, 1.8, 10);
        glRotatef(-90, 0, 1, 0);
        glScalef(0.06, 0.06, 0.06);
        draw_cannon_shadow(x_cannon, y_cannon, z_cannon, t1_cannon, t2_cannon, t3_cannon, ntri_cannon);
    glPopMatrix();
    
    glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);
        glMultMatrixf(shadow_transformation);
        glTranslatef(robot_2.x, robot_2.y, robot_2.z);
        glRotatef(robot_2.angle_y, 0, 1, 0);
        glPushMatrix();
            glTranslatef(0, 0, 3);
            glTranslatef(0, 2, 0); //Move up to origin
            glScalef(1, 2, 1);
            glutSolidCube(2.0);
        glPopMatrix();
        glRotatef(robot_2.angle_x, 1, 0, 0);
        glTranslatef(0, 1, 0); //Move up to origin
        draw_robot_shadow(robot_2);
    glPopMatrix();
    
    
    //Patrol Robot
    glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);
        glMultMatrixf(shadow_transformation);
        glTranslatef(robot_1.x, robot_1.y, robot_1.z);
        glRotatef(robot_1.angle_y, 0, 1, 0);
        glRotatef(robot_1.angle_z, 0, 0, 1);
        glTranslatef(0, 1, 0); //Move up to origin
        draw_robot_shadow(robot_1);
    glPopMatrix();
    
    //Reload robot
    glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);
        glMultMatrixf(shadow_transformation);
        glTranslatef(robot_3.x, robot_3.y, robot_3.z);
        glRotatef(robot_3.angle_y, 0, 1, 0);
        glTranslatef(0, 1, 0);
        draw_robot_shadow(robot_3);
    glPopMatrix();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    
     // ***************** DRAWING SHADOWS END *****************
    
    
    //spaceship
    glPushMatrix();
        glTranslatef(37, 0, -30);
        glColor3f(0.8, 0.8, 0.8);
        draw_spaceship(spaceship, GL_LIGHT2);
    glPopMatrix();
    
    
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
    
    
    //Reload Robot
    glPushMatrix();
        glTranslatef(robot_3.x, robot_3.y, robot_3.z);
        glRotatef(robot_3.angle_y, 0, 1, 0);
        glTranslatef(0, 1, 0);
        draw_robot(robot_3);
    glPopMatrix();
    
    
    //particles and fountain
    glPushMatrix();
        glTranslatef(60, 0, 16);
        draw_fountain(particle_list, num_particles, angle);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(10, 0, 16);
        draw_fountain(particle_list, num_particles, angle);
    glPopMatrix();
    
    
    /*glPushMatrix();
        draw_axis();
    glPopMatrix();*/
    
    //castle
    glPushMatrix();
        glTranslatef(20, 0, 0);
        glScalef(2, 2, 2);
        draw_castle(x_castle, y_castle, z_castle, t1_castle, t2_castle, t3_castle, ntri_castle);
    glPopMatrix();
    
    //cannon and cannonball
    glPushMatrix();
        glTranslatef(50, 1.8, 10);
        glRotatef(-90, 0, 1, 0);
        glScalef(0.06, 0.06, 0.06);
        draw_cannon(x_cannon, y_cannon, z_cannon, t1_cannon, t2_cannon, t3_cannon, ntri_cannon);
    glPopMatrix();
    
    draw_skybox();
    
    //Make sure floor is drawn before any lighting
    glPushMatrix();
        glTranslatef(0, -0.1, 0);
        draw_floor();
    glPopMatrix();    
    
    glPushMatrix();
        draw_grass();
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
    //glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);
    gluPerspective(80.0, 1.0, 10.0, 550.0);
    
    initialise_skybox(skybox_texture_ids, "./skybox_textures/front.bmp", "./skybox_textures/left.bmp", "./skybox_textures/right.bmp", "./skybox_textures/top.bmp", "./skybox_textures/back.bmp", "./skybox_textures/bottom.bmp");
    
    for (int i = 0; i < num_particles; i++) {
        particle_list[i] = new Particle;
        
    }
    
     srand(time(0));
    
    //fire all the particles
    for (int i = 0; i < num_particles; i++) {
        fire_particle(particle_list[i]);
        
    }
}



void keyboard(unsigned char key, int x, int y)
{
    if (key == 'c') {
        if (!has_cannon_been_fired) {
            fire_cannon();
            has_cannon_been_fired = true;
        }
        
    } else if (key == 'w') {
        cannon_angle += (cannon_angle < 86) ? 2 : 0;
        update_cannonball_position();
        glutPostRedisplay();
        
    } else if (key == 'a') {
        cannon_angle -= (cannon_angle > -10) ? 2 : 0;
        update_cannonball_position();
        glutPostRedisplay();
        
    } else if (key == 's' && !is_spaceship_taking_off) {
        animation_selector(1);
        is_spaceship_taking_off = true; //to only allow spaceshipt to take off once
        
    } else if (key == 'p') {
        
        is_default_cam ^= true;
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
    //animation_selector(0);
    //animation_selector(2);
    //animation_selector(3);
    animation_selector(4);
    animation_selector(5);
    glutMainLoop();
    return 0;
}
