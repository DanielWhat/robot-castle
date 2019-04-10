#ifndef TEXTURES_H
#define TEXTURES_H

#include <GL/freeglut.h>
#include <stdbool.h>
#include <fstream>

using namespace std;

void initialise_skybox(GLuint skybox_textures[], string top, string bottom, string left, string right, string front, string back);
/* Initialises a skybox. Ensure all strings and filenames to bmp files. 
 * The skybox textures are put in the skybox_textures list in the 
 * following order: top, bottom, left, bottom, right, front, back. */



void initialise_textures(GLuint* texId, std::string filename);
/* Initialises textures for use on any object. Only works with BMP 
 * files. */

void initialise_textures_tga(GLuint* texId, char* filename);


void enable_quadratic_textures(GLuint* texId, GLUquadricObj* p);
/* Enables texturing on quadratic objects. Call this right before 
 * drawing a GLUQuadraticObj */

#endif /* TEXTURES_H */
