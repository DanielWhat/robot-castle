#ifndef OPEN_OFF_H
#define OPEN_OFF_H 

#include <iostream>
#include <fstream>

using namespace std;

void loadMeshFile(const char* fname, float* x[], float* y[], float* z[], int* t1[], int* t2[], int* t3[], int* num_verticies, int* num_triangles);
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
