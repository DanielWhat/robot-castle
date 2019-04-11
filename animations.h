/* A series of functions to animate Robot objects. 
 * 
 * Note that these functions do not literally animate the robots. These 
 * functions change the information in the Robot* data according to the 
 * animation, and it is up to the user to appropriately display the 
 * robot's location given the contents of the Robot* data type.*/

#include <GL/freeglut.h>
#include <math.h>
#include <stdbool.h>
#include "robot.h"
#include "spaceship.h"
#include "cannon.h"


void animate_spaceship_takeoff(Spaceship* spaceship, void (*callback) (int), int callback_data);



void animate_passive_spaceship (Spaceship* spaceship, void (*callback) (int), int callback_data, bool use_callback);
/* Animates the spaceships passive/idle animation (note that the 
 * revolving balls animation is handled in spaceship.cpp. This should be
 * changed later.*/



void animate_robot_wavy_hands(Robot* robot);
/* @@ */



void toggle_eyes (Robot* robot);
/* Alternates the color of a robots eyes from red to blue or blue to red */


void normalise_vector (float* x, float* y);
/* Given an x and y coordinate of a vector, will calculate the unit 
 * vector and update x and y accordingly.*/


float get_angle_between_2_vectors (float x1, float y1, float x2, float y2);
/* Returns a float which is the angle of the two vectors in radians */



void animate_all (Robot* robot_1, Robot* robot_2, Robot* robot_3, Spaceship* spaceship, CannonBall* cannonball, bool has_cannon_been_fired, void (*callback) (int), int callback_data);
/* Makes calls to all animate functions. Using this function 
 * yeilds better performance that calling all the animation functions
 * individually. */
 
 
void animate_reload_robot (Robot* robot, CannonBall* cannonball, void (*callback) (int), int callback_data, bool has_cannon_been_fired, bool use_callback);


void animate_worker_robot(Robot* robot, void (*callback) (int), int callback_data, bool use_callback);
/*@@*/


void animate_patrol_robot(Robot* robot, void (*callback) (int), int callback_data, bool use_callback);
/* Animates the patrol robot when given a Robot* data type. A callback 
 * function is needed asw ell which will call this function when given 
 * an int 0 */
