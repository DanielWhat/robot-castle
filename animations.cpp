/* A series of functions to animate Robot objects and the spaceship. 
 * 
 * Note that these functions do not literally animate the robots. These 
 * functions change the information in the Robot* data according to the 
 * animation, and it is up to the user to appropriately display the 
 * robot's location given the contents of the Robot* data type.*/
 
#define GL_SILENCE_DEPRECATION

#include <GL/freeglut.h>
#include <math.h>
#include <stdbool.h>
#include "robot.h"
#include "animations.h"
#include "spaceship.h"
#include "cannon.h"

#include <iostream>
#include <fstream>

void animate_spaceship_takeoff(Spaceship* spaceship, void (*callback) (int), int callback_data)
{
    static float rotate_incr = 0.1;
    static float height_incr = 0.1;
    
    spaceship->angle += rotate_incr;
    spaceship->height += height_incr/10;
    
    rotate_incr += 0.2;
    height_incr += (spaceship->height < 40) ? 0.1 : -height_incr/10;
    
    if (spaceship->height > 40 && spaceship->leg_height < 2) {
        
        //bring in the legs for takeoff
        spaceship->leg_height += 0.05;
    }
    
    glutPostRedisplay();
    glutTimerFunc(100, callback, callback_data);
}


void animate_passive_spaceship (Spaceship* spaceship, void (*callback) (int), int callback_data, bool use_callback)
/* Animates the spaceships passive/idle animation (note that the 
 * revolving balls animation is handled in spaceship.cpp. This should be
 * changed later. */
{
    static int counter = 0;
    
    if (counter % 3 == 0) {
        spaceship->is_lights_on = !spaceship->is_lights_on;
        counter = 0;
    }
    counter++;
    
    if (use_callback) {
        glutPostRedisplay();
        glutTimerFunc(500, callback, callback_data);
    }
}


void animate_robot_wavy_hands(Robot* robot)
/* @@ */
{
    robot->right_arm.humerus_forward_angle += 20;
    
    if (robot->right_arm.humerus_forward_angle < -90 || robot->right_arm.humerus_forward_angle > 90) {
        robot->right_arm.humerus_side_angle -= 20;
    }
    
    robot->left_arm.humerus_forward_angle -= 20;
    
    if (robot->left_arm.humerus_forward_angle < -90 || robot->left_arm.humerus_forward_angle > 90) {
        robot->left_arm.humerus_side_angle += 20;
    }
}



void toggle_eyes (Robot* robot) 
/* Alternates the color of a robots eyes from red to blue or blue to red */
{
    robot->left_eye.r ^= 1;
    robot->left_eye.b ^= 1; 
    robot->right_eye.r ^= 1;
    robot->right_eye.b ^= 1;  
}



void nothing (int data) {
    
    
}


void normalise_vector (float* x, float* y)
/* Given an x and y coordinate of a vector, will calculate the unit 
 * vector and update x and y accordingly.*/
{
    float vector_magnitude = 0;
    vector_magnitude = sqrt(*x * *x + *y * *y);
    if (*x != 0 || *y != 0) {
        *x /= (vector_magnitude + 0.0);
        *y /= (vector_magnitude + 0.0);
    }
}


float get_angle_between_2_vectors (float x1, float y1, float x2, float y2) 
/* Returns a float which is the angle of the two vectors in radians */
{
    float dot_product = x1 * x2 + y1 * y2;
    float magnitude_v1 = sqrt(x1 * x1 + y1 * y1);
    float magnitude_v2 = sqrt(x2 * x2 + y2 * y2);
    
    if ((x1 == 0 && y1 == 0) || (x2 == 0 && y2 == 0)) {
        return 0;
        
    } else {
        return acos(dot_product / (magnitude_v1 * magnitude_v2));
    }
}



void animate_all (Robot* robot_1, Robot* robot_2, Robot* robot_3, Spaceship* spaceship, CannonBall* cannonball, bool has_cannon_been_fired, void (*callback) (int), int callback_data)
/* Makes calls to all animate functions. Using this function 
 * yeilds better performance that calling all the animation functions
 * individually. */
{
    //the callbacks never occur so doesn't matter what we these functions as a callback
    animate_patrol_robot(robot_1, nothing, -1, false); 
    
    animate_worker_robot(robot_2, nothing, -1, false); 
    
    animate_reload_robot(robot_3, cannonball, nothing, -1, has_cannon_been_fired, false);
    
    animate_passive_spaceship(spaceship, nothing, -1, false);
    
    glutPostRedisplay();
    glutTimerFunc(100, callback, callback_data);
}



void animate_reload_robot (Robot* robot, CannonBall* cannonball, void (*callback) (int), int callback_data, bool has_cannon_been_fired, bool use_callback)
{
    static bool robot_picked_up_cannonball = false;
    float direction_vector_x = 0;
    float direction_vector_z = 0;
    float angle;
    float destination_x;
    float destination_z;
    
    //remember the cannon is scaled down by glScalef(0.06, 0.06, 0.06) and translated by glTranslatef(50, 1.8, 10); for this code to make sense
    if (has_cannon_been_fired) {
        
        if (robot_picked_up_cannonball) { //i.e robot is returning with the cannonball
            destination_x = 50;
            destination_z = 14;
            
        } else { //robot is getting the cannonball
            destination_x = 50;
            destination_z = (cannonball->x*0.06 + 10);
        }
        
        //get the vector describing the direction the robot needs to move in
        direction_vector_x = destination_x - robot->x;
        direction_vector_z = destination_z - robot->z;
        
        //normalise the vector
        normalise_vector(&direction_vector_x, &direction_vector_z);
        
        if (!robot_picked_up_cannonball) {
            if (robot->z < destination_z || robot->x < destination_x) {
                robot->x += (robot->x < destination_x) ? direction_vector_x : 0;
                robot->z += (robot->z < destination_z) ? direction_vector_z : 0;
                
            } else if (cannonball->y > 0) {
                //need to wait for cannonball to fall
                
            } else { 
                robot_picked_up_cannonball = true;
                robot->right_arm.humerus_forward_angle = -90;
                cannonball->velocity_x = 0;
                cannonball->velocity_y = 0;
                cannonball->velocity_z = 0;
            }
        } else {
            if (robot->z > destination_z || robot->x > destination_x) {
                robot->x += direction_vector_x;
                robot->z += direction_vector_z;
                
            } else { //robot has returned the cannonball
                cannonball->in_cannon = true;
                robot_picked_up_cannonball = false;
            }
            
            //move the cannonball with the robot
            cannonball->x = (robot->z - 10 - 4.2) * (1/0.06); //-4 to get it in robot's hand
            cannonball->z = 0.9 * (1/0.06); // move it into robot hand
            cannonball->y = 3.4 * (1/0.06) ; //height of robot hand
        }
        //the robot is by default looking in the direction (0, 0, 1)
        angle = get_angle_between_2_vectors(0, 1, direction_vector_x, direction_vector_z);
        robot->angle_y = angle * (180/M_PI);
        
    } else { //cannon has not been fired or the robot has just returned the cannonball and the cannon has been reset
        direction_vector_x = 35 - robot->x;
        direction_vector_z = -5 - robot->z;
        
        normalise_vector(&direction_vector_x, &direction_vector_z);
        
        if (robot->z > -5 || robot->x > 35) {
            robot->x += direction_vector_x;
            robot->z += direction_vector_z;
                
        } else {
            robot->x = 35;
            robot->z = -5;
        }
        
        
        //the robot is by default looking in the direction (0, 0, 1)
        angle = get_angle_between_2_vectors(0, 1, direction_vector_x, direction_vector_z);
        robot->angle_y = -angle * (180/M_PI);
        
        if (robot->right_arm.humerus_forward_angle <= 0) {
            robot->right_arm.humerus_forward_angle += 3;
        }
    }
    
    toggle_eyes(robot);
    
    if (use_callback) {
        glutPostRedisplay();
        glutTimerFunc(100, callback, callback_data);
    }
}



void animate_worker_robot(Robot* robot, void (*callback) (int), int callback_data, bool use_callback)
{
    static int counter = 0;
    
    if (robot->z > -55 && robot->angle_y == 180) {
        robot->z -= 0.5;
        
    } else if (robot->z <= -55 && robot->angle_y > 90) {
        robot->x += sin(robot->angle_y * (M_PI/180))/2;
        robot->z += cos(robot->angle_y * (M_PI/180))/2;
        robot->angle_y -= 5;
        
    } else if (robot->x < 75 && robot->angle_y == 90) {
        robot->x += 0.5;
        
    } else if (robot->x >= 75 && robot->angle_y > 0) {
        robot->x += sin(robot->angle_y * (M_PI/180))/2;
        robot->z += cos(robot->angle_y * (M_PI/180))/2;
        robot->angle_y -= 5;
        
    } else if (robot->z < 30 && robot->angle_y == 0) {
        robot->z += 0.5;
        
    } else if (robot->z >= 30 && robot->angle_y > -90) {
        robot->x += sin(robot->angle_y * (M_PI/180))/2;
        robot->z += cos(robot->angle_y * (M_PI/180))/2;
        robot->angle_y -= 5;
        
    } else if (robot->x > -10 && robot->angle_y == -90) {
        robot->x -= 0.5;
        
    } else if (robot->x <= -10 && robot->angle_y > -180) {
        robot->x += sin(robot->angle_y * (M_PI/180))/2;
        robot->z += cos(robot->angle_y * (M_PI/180))/2;
        robot->angle_y -= 5;
        
        //reset angle
        if (robot->angle_y == -180) {
            robot->angle_y = 180;
            
        }
    } 
    
    if (counter / 3 == 1) {
        toggle_eyes(robot);
        counter = 0;
    }
    counter++;
    
    if (use_callback) {
        glutPostRedisplay();
        glutTimerFunc(100, callback, callback_data);
    }
}



void animate_patrol_robot(Robot* robot, void (*callback) (int), int callback_data, bool use_callback)
/* Animates the patrol robot when given a Robot* data type. A callback 
 * function is needed asw ell which will call this function when given 
 * an int 0 */
{
    static int counter = 0;
    
    //Move forwards until robot is at x >= 30
    if (robot->x < 30 && robot->angle_y == 90) {
        robot->x += 0.5;
        robot->angle_z += (robot->angle_z > 0) ? -0.5 : 0; //continue to straighten the robot if you need to
        
    //Turn backwards until robot has turned -180 deg
    } else if (robot->x >= 30 && robot->angle_y > -90) {
        robot->x += sin(robot->angle_y * (M_PI/180))/2;
        robot->z += cos(robot->angle_y * (M_PI/180))/2;
        robot->angle_y -= 5;
        
        //Tilt the robot as he turns the coner
        if (robot->angle_y > -20) {
            robot->angle_z += 0.5;
        } else if (robot->angle_y < -30) {
            robot->angle_z -= 0.5;
        }
    
    //Move backwards until robot x is <= -10
    } else if (robot->x > -10 && robot->angle_y == -90) {
        robot->x -= 0.5;
        robot->angle_z += (robot->angle_z > 0) ? -0.5 : 0; //continue to straighten the robot if you need to
    
    //Turn forwards until robot has turned -180 deg
    } else if (robot->x <= -10 && robot->angle_y != (90-360)) {
        robot->x += sin(robot->angle_y * (M_PI/180))/2;
        robot->z += cos(robot->angle_y * (M_PI/180))/2;
        robot->angle_y -= 5;
        
        //Tilt the robot as he turns the coner
        if (robot->angle_y > -200) {
            robot->angle_z += 0.5;
        } else if (robot->angle_y < -210) {
            robot->angle_z -= 0.5;
        }
        
        //reset angle back to 90 as 90-360 same as 90
        if (robot->angle_y == (90-360)) {
            robot->angle_y = 90;
        }
    }
    
    //Animate robot arm movement
    animate_robot_wavy_hands(robot);
    
    //Toggle eyes every 5 frames
    if (counter / 5 == 1) {
        toggle_eyes(robot);
        counter = 0;
    }
    counter++;
    
    if (use_callback) {
        glutPostRedisplay();
        glutTimerFunc(100, callback, callback_data);
    }
}
