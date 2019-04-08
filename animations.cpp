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
    
    glutTimerFunc(50, callback, callback_data);
    glutPostRedisplay();
}


void animate_passive_spaceship (Spaceship* spaceship, void (*callback) (int), int callback_data)
/* Animates the spaceships passive/idle animation*/
{
    spaceship->is_lights_on = (spaceship->is_lights_on) ? false : true;
    
    glutTimerFunc(500, callback, callback_data);
    glutPostRedisplay();
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



void animate_patrol_robot(Robot* robot, void (*callback) (int))
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
    
    glutTimerFunc(50, callback, 0);
    glutPostRedisplay();
}
