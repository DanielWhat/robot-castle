#ifndef CASTLE_RENDERING_H
#define CASTLE_RENDERING_H

#include <GL/freeglut.h>
#include <math.h>
#include <stdlib.h>     
#include <time.h>  
#include "open_off.h"
#include "spaceship.h"
#include "particle.h"

void initialise_castle_textures(void);
/* Initialise textures needed to properly display the castle */


void draw_single_grass(void);
/* Draws a small grass patch intended to surround the castle */

void draw_grass (void);

void draw_pot(void);
/* Draws a pot for the fountain to go in */

void draw_pot_shadow(void);


void draw_fountain (Particle* particle_list[], int num_particles, int camera_angle);

void draw_octagon(int x, int y, int z);
/* Takes the normal vector to the octagon and draws an octagon at with 
 * centre at (0, 0, 0) and with each side having length 1 */



void draw_pillar (int height);
/* Draws a closed octogonal cylinder */



void draw_castle_minor_pillars(void);
/* Draws 4 minor pillars that surround the back of the castle */



void draw_single_castle_wall(void);
/* Draws one single castle wall with textures added. */



void draw_castle_walls(void);
/* Draws the surrounding castle walls and supporting pillars which 
 * surround the castle. */



void draw_castle (const float x_castle[], const float y_castle[], const float z_castle[], const int t1_castle[], const int t2_castle[], const int t3_castle[], int num_triangles_castle);
/* Draws the castle using glut objects and off_files.*/

#endif /* CASTLE_RENDERING_H */
