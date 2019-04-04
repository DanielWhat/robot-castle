/*
 * Functions given by Dr. Mukundan and refactored by myself.
 */

#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
#include "open_off.h"

using namespace std;

void normal(int tindx, const float x[], const float y[], const float z[], const int t1[], const int t2[], const int t3[])
/* Calculates the normal vector given x, y, z vertex arrays and t1, t2, 
 * t3 index arrays. Calls glNormal3f() to put the normal vector into the
 * gl pipeline. */
{
	float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
	float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
	float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
	float nx, ny, nz;
	nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
	ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
	nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
	glNormal3f(nx, ny, nz);
}



void draw_off_file(const float x[], const float y[], const float z[], const int t1[], const int t2[], const int t3[], int num_triangles)
/* Draws an off_file imported with load_mesh_file into x, y, z, t1, t2, 
 * t3 and num_triangles. */
{
    glBegin(GL_TRIANGLES);
        for(int triangle_index = 0; triangle_index < num_triangles; triangle_index++)
        {
           normal(triangle_index, x, y, z, t1, t2, t3);
           glVertex3d(x[t1[triangle_index]], y[t1[triangle_index]], z[t1[triangle_index]]);
           glVertex3d(x[t2[triangle_index]], y[t2[triangle_index]], z[t2[triangle_index]]);
           glVertex3d(x[t3[triangle_index]], y[t3[triangle_index]], z[t3[triangle_index]]);
        }
    glEnd();
}



void load_mesh_file(const char* fname, float* x[], float* y[], float* z[], int* t1[], int* t2[], int* t3[], int* num_verticies, int* num_triangles)
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
{
	ifstream fp_in;
	int num, ne;

	fp_in.open(fname, ios::in);
	if(!fp_in.is_open())
	{
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');				//ignore first line
	fp_in >> *num_verticies >> *num_triangles >> ne;			    // read number of vertices, polygons, edges

    *x = new float[*num_verticies];                        //create arrays
    *y = new float[*num_verticies];
    *z = new float[*num_verticies];

    *t1 = new int[*num_triangles];
    *t2 = new int[*num_triangles];
    *t3 = new int[*num_triangles];

	for(int i=0; i < *num_verticies; i++)                         //read vertex list 
		fp_in >> (*x)[i] >> (*y)[i] >> (*z)[i];
        

	for(int i=0; i < *num_triangles; i++)                         //read polygon list 
	{
		fp_in >> num >> (*t1)[i] >> (*t2)[i] >> (*t3)[i];
		if(num != 3)
		{
			cout << "ERROR: Polygon with index " << i  << " is not a triangle." << endl;  
			exit(1);
		}
	}

	fp_in.close();
	cout << " File successfully read." << endl;
}
