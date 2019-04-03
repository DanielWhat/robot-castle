#include <iostream>
#include <fstream>
#include "open_off.h"

using namespace std;

void loadMeshFile(const char* fname, float* x[], float* y[], float* z[], int* t1[], int* t2[], int* t3[], int* num_verticies, int* num_triangles)
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
