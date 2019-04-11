#ifndef PARTICLE_H
#define PARTICLE_H 
#include <GL/freeglut.h>

typedef struct particle_s {
    
    float x;
    float y;
    float z;
    float x_vel;
    float y_vel;
    float z_vel;
    float ttl;
    
} Particle;


void fire_particle(Particle* particle);


void update_particles(Particle* particle_list[], int num_particles);


void update_single_particle(Particle* particle);


#endif /* PARTICLE_H */
