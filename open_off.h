/*
 * Functions given by Dr. Mukundan and refactored by myself.
 */

#ifndef OPEN_OFF_H
#define OPEN_OFF_H 

#include <iostream>
#include <fstream>
#include <GL/freeglut.h>

using namespace std;

void normal(int tindx, const float x[], const float y[], const float z[], const int t1[], const int t2[], const int t3[]);
/* Calculates the normal vector given x, y, z vertex arrays and t1, t2, 
 * t3 index arrays. Calls glNormal3f() to put the normal vector into the
 * gl pipeline. */



void draw_off_file(const float x[], const float y[], const float z[], const int t1[], const int t2[], const int t3[], int num_triangles);
/* Draws an off_file imported with load_mesh_file into x, y, z, t1, t2, 
 * t3 and num_triangles. */



void load_mesh_file(const char* fname, float* x[], float* y[], float* z[], int* t1[], int* t2[], int* t3[], int* num_verticies, int* num_triangles);
/* Loads an OFF meshfile which specifies only triangles. 
 * The vertex data is loaded into 6 lists; x, y, z, t1, t2, t3. 
 * 
 * For each vertex i in the OFF file, the list x will the x coordinate 
 * of the vertex i. The same applies for lists y and z, but with y and z
 * respectively instead of x. 
 * 
 * The length of the list t1 is equal to the number of triangles in the 
 * OFF file. Each entry in list t1 contains an index for lists x, and z
 * such that (x[t1[i]], y[t1[i]], z[t1[i]]) is the first vertex 
 * coordinate of the i-th triangle. 
 * The same applies for lists t2 and t3, but with 2 and 3 
 * respectively instead of 1.
 * */
 
 #endif /*OPEN_OFF_H*/
