#define GL_SILENCE_DEPRECATION 0

#include <math.h>
#include <stdio.h>
#include <climits>
#include <GL/freeglut.h>
#include "open_off.h"
#include "castle_rendering.h"
#include "cannon.h"
#include "robot.h"

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

Robot robot_1 = {.x = -10, .y = 0, .z = 10, .angle = 90, .left_eye = {.r = 1, .g = 0, .b = 0}, .right_eye = {.r = 0, .g = 0, .b = 1}};  


void drawFloor()
{
    glColor3f(0., 0.5,  0.);            //Floor colour

    for(int i = -50; i <= 50; i ++)
    {
        glBegin(GL_LINES);          //A set of grid lines on the xz-plane
            glVertex3f(-50, -0.75, i);
            glVertex3f(50, -0.75, i);
            glVertex3f(i, -0.75, -50);
            glVertex3f(i, -0.75, 50);
        glEnd();
    }
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


void animate_robot(int data)
{
    static int counter = 0;
    
    if (robot_1.x < 30 && robot_1.angle == 90) {
        robot_1.x += 0.5;
        
    } else if (robot_1.x == 30 && robot_1.angle != -90) {
        robot_1.angle -= 10;
        
    } else if (robot_1.x > -10 && robot_1.angle == -90) {
        robot_1.x -= 0.5;
        
    } else if (robot_1.x == -10 && robot_1.angle != 90) {
        robot_1.angle += 10;
    }
    
    //Toggle eyes
    if (counter / 5 == 1) {
        robot_1.left_eye.r ^= 1;
        robot_1.left_eye.b ^= 1; 
        robot_1.right_eye.r ^= 1;
        robot_1.right_eye.b ^= 1;  
        counter = 0;
    } 
    counter++;
    
    glutTimerFunc(50, animate_robot, 0);
    glutPostRedisplay();
}


void draw_spaceship(void)
{
    glColor3f(1.0, 1.0, 0);
    glBegin(GL_LINES);
        for (int x = -5; x < 6; x++) {
            glVertex3f(x, 1, sqrt(5 - x * x));
        }   
    glEnd();
}



void display (void)
{
    float light_position[4] = {0, 10, 10, 1};
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    dx = cos(angle * (M_PI/180));
    dz = sin(angle * (M_PI/180));
    
    gluLookAt(camx, 10, camz, camx+dx, 10, camz+dz, 0.0, 1.0, 0.0);
    
    glPushMatrix();
        draw_axis();
    glPopMatrix();
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glDisable(GL_LIGHTING);
    drawFloor();
    glEnable(GL_LIGHTING);
    
    glPushMatrix();
        glTranslatef(20, 0, 0);
        draw_castle(x_castle, y_castle, z_castle, t1_castle, t2_castle, t3_castle, ntri_castle);
    glPopMatrix();
    
    //Enable specular lighting for cannon and draw cannon
    glPushMatrix();
        glTranslatef(10, 1, 10);
        glScalef(0.06, 0.06, 0.06);
        draw_cannon(x_cannon, y_cannon, z_cannon, t1_cannon, t2_cannon, t3_cannon, ntri_cannon);
    glPopMatrix();
    
    //Robot 1
    glPushMatrix();
        glTranslatef(robot_1.x, robot_1.y, robot_1.z);
        glRotatef(robot_1.angle, 0, 1, 0);
        glTranslatef(0, 1, 0); //Move up to origin
        draw_robot(robot_1);
    glPopMatrix();
    
    glPushMatrix();
        draw_spaceship();
    glPopMatrix();
    
    glutSwapBuffers();
}



void initialize (void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    load_mesh_file("./castle_gate.off", &x_castle, &y_castle, &z_castle, &t1_castle, &t2_castle, &t3_castle, &nvrt_castle, &ntri_castle);	
    load_mesh_file("./Cannon.off", &x_cannon, &y_cannon, &z_cannon, &t1_cannon, &t2_cannon, &t3_cannon, &nvrt_cannon, &ntri_cannon);
    
    initialise_castle_textures();
    
    glEnable(GL_LIGHTING); //Turn on lights
    glEnable(GL_LIGHT0);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey); //the passive light coming from all directions is a light grey
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white); //white light
    glLightfv(GL_LIGHT0, GL_SPECULAR, white); //white light
    
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
    animate_robot(0);
    glutMainLoop();
    return 0;
}
