#ifndef ROBOT_H
#define ROBOT_H

#include <GL/freeglut.h>

typedef struct robot_eye_s
{
    int r;
    int g;
    int b;
    
} RobotEye;


typedef struct robot_arm_s
{
    float humerus_side_angle;
    float humerus_forward_angle; 
    
} RobotArm;


typedef struct robot_s
{
    float x;
    float y;
    float z;
    float angle_y;
    float angle_z;
    RobotArm left_arm;
    RobotArm right_arm;
    RobotEye left_eye;
    RobotEye right_eye;
    
} Robot;



void draw_robot_body(GLUquadricObj* p);
/* Draws a robot body including the buttons on the front of the body. 
 * Takes a GLUquadraticObj pointer for drawing quadratics. */

 

void draw_robot_wheel(GLUquadricObj* p);
/* Draws a robot wheel, along with the bars that connect the wheel to 
 * the robot. Takes a GLUquadraticObj pointer for drawing quadratics. */



void draw_robot(Robot robot);
/* Takes a robot object in order to properly draw the robots features 
 * and limb orientations. */
 

#endif /* ROBOT_H */
