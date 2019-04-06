#define GL_SILENCE_DEPRECATION 0

#include <math.h>
#include <stdio.h>
#include <climits>
#include <GL/freeglut.h>
#include "open_off.h"
#include "castle_rendering.h"
#include "cannon.h"


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



void display (void)
{
    float light_position[4] = {0, 10, 10, 1};
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    dx = cos(angle * (M_PI/180));
    dz = sin(angle * (M_PI/180));
    
    gluLookAt(camx, 3, camz, camx+dx, 3, camz+dz, 0.0, 1.0, 0.0);
    
    //gluLookAt(7.5, 0, 30 + camera_z_offset, 7.5, 8, camera_z_offset, 0, 1, 0);
    
    //glTranslatef(0, 0, 0 + camera_z_offset * 2);
    //glRotatef(angle, 0, 1, 0);
    //glTranslatef(-7.5, -20, -(30+ camera_z_offset));
    
    
    glPushMatrix();
        draw_axis();
    glPopMatrix();
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glDisable(GL_LIGHTING);
    drawFloor();
    glEnable(GL_LIGHTING);
    
    glPushMatrix();
        glColor3f(0.0, 0.0, 1.0);
        glTranslatef(20, 0, 0);
        draw_castle(x_castle, y_castle, z_castle, t1_castle, t2_castle, t3_castle, ntri_castle);
    glPopMatrix();
    
    //Enable specular lighting for cannon and draw cannon
    glPushMatrix();
        glTranslatef(10, 1, 10);
        glScalef(0.06, 0.06, 0.06);
        draw_cannon(x_cannon, y_cannon, z_cannon, t1_cannon, t2_cannon, t3_cannon, ntri_cannon);
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
    glutMainLoop();
    return 0;
}
