#include <GL/freeglut.h>
#include "particle.h"

//for random
#include <stdlib.h>
#include <time.h>

float gravity = 0.2;

void fire_particle(Particle* particle)
{
    particle->x = 0;
    particle->y = 0;
    particle->z = 0;
    
    particle->y_vel = (rand() % 20) / 10.0 + 0.5;
    particle->x_vel = (rand() % 30) / 210.0 - 0.0714;
    particle->z_vel = (rand() % 30) / 210.0 - 0.0714;
}


void update_particles(Particle* particle_list[], int num_particles)
{
    for (int i = 0; i < num_particles; i++) {
        update_single_particle(particle_list[i]);
        
    }
}




void update_single_particle(Particle* particle)
{
    if (particle->y > -5) {
        particle->x += particle->x_vel;
        particle->y += particle->y_vel;
        particle->z += particle->z_vel;
        
        particle->y_vel -= gravity;
    } else {
        
        fire_particle(particle);
    }
}



