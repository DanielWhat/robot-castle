/* A series of functions to animate Robot objects. 
 * 
 * Note that these functions do not literally animate the robots. These 
 * functions change the information in the Robot* data according to the 
 * animation, and it is up to the user to appropriately display the 
 * robot's location given the contents of the Robot* data type.*/

#include <GL/freeglut.h>
#include <math.h>
#include "robot.h"


void animate_robot_wavy_hands(Robot* robot);
/* @@ */



void toggle_eyes (Robot* robot);
/* Alternates the color of a robots eyes from red to blue or blue to red */



void animate_patrol_robot(Robot* robot, void (*callback) (int));
/* Animates the patrol robot when given a Robot* data type. A callback 
 * function is needed asw ell which will call this function when given 
 * an int 0 */
