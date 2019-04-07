#ifndef ROBOT_H
#define ROBOT_H

#include <GL/freeglut.h>
#include "loadBMP.h"

void draw_robot_body(GLUquadricObj* p);
/* Draws a robot body including the buttons on the front of the body. 
 * Takes a GLUquadraticObj pointer for drawing quadratics. */

 

void draw_robot_wheel(GLUquadricObj* p);
/* Draws a robot wheel, along with the bars that connect the wheel to 
 * the robot. Takes a GLUquadraticObj pointer for drawing quadratics. */



void draw_robot(void);

#endif /* ROBOT_H */
