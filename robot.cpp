#define GL_SILENCE_DEPRECATION

#include <GL/freeglut.h>
#include "robot.h"

//@@@ Need to find a fix for the white and black thing
static float white_r[4] = {1.0, 1.0, 1.0, 1.0};
static float black_r[4] = {0.0, 0.0, 0.0, 1.0};

static float angle_robot = 80;


void draw_robot_body(GLUquadricObj* p)
/* Draws a robot body including the buttons on the front of the body. 
 * Takes a GLUquadraticObj pointer for drawing quadratics. */
{
    /*glEnable(GL_TEXTURE_2D); //Enable texturing
    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    loadBMP("gold_energy.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluQuadricTexture (p, GL_TRUE);
    
    glBindTexture(GL_TEXTURE_2D, texId);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);*/
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white_r); //Enable specular light
    
    //Robot body
    glPushMatrix();
        glColor3f(0.6, 0.6, 0.6);
        glTranslatef(0, 3.2, 0);
        glRotatef(90, 1, 0, 0);
        gluCylinder(p, 0.8, 0.8, 1.9, 30, 30);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, black_r); //disable specular light
    
    //Robot body buttons
    /*glPushMatrix();
        glTranslatef(0, 3, 1);
        glBegin(GL_QUADS);
            glColor3f(1.0, 0, 0.2);
            glVertex3f(0, 0, 0);
            glVertex3f(0.2, 0, 0);
            glVertex3f(0.2, 0.2, 0);
            glVertex3f(0, 0.2, 0);
        glEnd();
    glPopMatrix();*/
}


void draw_robot_wheel(GLUquadricObj* p)
/* Draws a robot wheel, along with the bars that connect the wheel to 
 * the robot. Takes a GLUquadraticObj pointer for drawing quadratics. */
{
    //Draw wheel
    glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);
        glRotatef(90, 0, 1, 0);
        glutSolidTorus(0.2, 0.8, 30, 30);
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white_r); //Enable specular light
    
    //Draw middle spoke
    glPushMatrix();
        glColor3f(0.6, 0.6, 0.6);
        glTranslatef(-0.25, 0, 0);
        glRotatef(90, 0, 1, 0);
        gluCylinder(p, 0.1, 0.1, 0.5, 8, 8);
    glPopMatrix();
    
    //left bar
    glPushMatrix();
        glColor3f(0.6, 0.6, 0.6);
        glTranslatef(-0.65, 1.65, 0);
        glRotatef(15, 0, 0, 1);
        glRotatef(90, 1, 0, 0);
        gluCylinder(p, 0.1, 0.1, 1.7, 8, 8);
    glPopMatrix();
    
    //right bar
    glPushMatrix();
        glColor3f(0.6, 0.6, 0.6);
        glTranslatef(0.65, 1.65, 0);
        glRotatef(-15, 0, 0, 1);
        glRotatef(90, 1, 0, 0);
        gluCylinder(p, 0.1, 0.1, 1.7, 8, 8);
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, black_r); //disable specular light
}



void draw_robot(Robot robot)
{
    //Draw wheel
    GLUquadricObj* p = gluNewQuadric();
    draw_robot_wheel(p);
    
    //Draw body
    draw_robot_body(p);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white_r); //Enable specular light
    
    //Robot head
    glPushMatrix();
        glColor3f(0.6, 0.6, 0.6);
        glTranslatef(0, 3.5, 0);
        glutSolidSphere(1.8/2, 30, 30);
    glPopMatrix();
    
    //Robot Arm left
    glPushMatrix();
        glColor3f(0.6, 0.6, 0.6);
        glTranslatef(-0.9, 2.5, 0);
        glRotatef(-angle_robot, 0, 0, 1);
        glRotatef(90, 1, 0, 0);
        glutSolidSphere(0.15, 15, 15);
        glPushMatrix();
            glTranslatef(0, 0, 1 + 0.15/2);
            glutSolidSphere(0.15, 15, 15);
        glPopMatrix();
        gluCylinder(p, 0.15, 0.15, 1, 8, 8);
    glPopMatrix();
    
    //Robot Arm right
    glPushMatrix();
        glColor3f(0.6, 0.6, 0.6);
        glTranslatef(0.9, 2.5, 0);
        glRotatef(angle_robot, 0, 0, 1);
        glRotatef(90, 1, 0, 0);
        glutSolidSphere(0.15, 15, 15);
        glPushMatrix();
            glTranslatef(0, 0, 1 + 0.15/2);
            glutSolidSphere(0.15, 15, 15);
        glPopMatrix();
        gluCylinder(p, 0.15, 0.15, 1, 8, 8);
    glPopMatrix();
    
    //Robot eye right
    glPushMatrix();
        glColor3f(robot.right_eye.r, robot.right_eye.g, robot.right_eye.b);
        glTranslatef(0.4, 3.7, 0.8);
        glutSolidSphere(0.15, 15, 15);
    glPopMatrix();
    
    //Robot eye left
    glPushMatrix();
        glColor3f(robot.left_eye.r, robot.left_eye.g, robot.left_eye.b);
        glTranslatef(-0.4, 3.7, 0.8);
        glutSolidSphere(0.15, 15, 15);
    glPopMatrix();
    
    //Mouth
    glPushMatrix();
        glColor3f(0, 0, 0);
        glTranslatef(-0.27, 3.3, 0.9);
        glBegin(GL_QUADS);
            glVertex3f(0, 0, 0);
            glVertex3f(0.5, 0, 0);
            glVertex3f(0.5, 0.1, 0);
            glVertex3f(0, 0.1, 0);
        glEnd();
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, black_r); //Disable specular light
}
