#define GL_SILENCE_DEPRECATION 0

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <climits>
#include <GL/freeglut.h>

using namespace std;

int angle = -90;
int camera_x_offset = 0;
int camera_z_offset = 0;


float dx = 0;
float dz = 0;
float camx = 7.5;
float camz = 30;

// ********************* FOR OFF FILE LOADING *********************
//Castle Entrance Variables
float *x_castle, *y_castle, *z_castle;  //vertex coordinate arrays
int *t1_castle, *t2_castle, *t3_castle; //triangles
int nvrt_castle, ntri_castle;    //total number of vertices and triangles
//float xmin_castle, xmax_castle, ymin_castle, ymax_castle; //min, max values of  object coordinates

//Cannon Variables
float *x_cannon, *y_cannon, *z_cannon;
int *t1_cannon, *t2_cannon, *t3_cannon;
int nvrt_cannon, ntri_cannon;


void loadMeshFile(const char* fname, float* x[], float* y[], float* z[], int* t1[], int* t2[], int* t3[], int* nvrt, int* ntri)  
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
	fp_in >> *nvrt >> *ntri >> ne;			    // read number of vertices, polygons, edges

    *x = new float[*nvrt];                        //create arrays
    *y = new float[*nvrt];
    *z = new float[*nvrt];

    *t1 = new int[*ntri];
    *t2 = new int[*ntri];
    *t3 = new int[*ntri];

	for(int i=0; i < *nvrt; i++)                         //read vertex list 
		fp_in >> (*x)[i] >> (*y)[i] >> (*z)[i];
        

	for(int i=0; i < *ntri; i++)                         //read polygon list 
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
/*
//-- Computes the min, max values of coordinates  -----------------------
void computeMinMax()
{
	xmin = xmax = x[0];
	ymin = ymax = y[0];
	for(int i = 1; i < nvrt; i++)
	{
		if(x[i] < xmin) xmin = x[i];
		else if(x[i] > xmax) xmax = x[i];
		if(y[i] < ymin) ymin = y[i];
		else if(y[i] > ymax) ymax = y[i];
	}
}*/

//--Function to compute the normal vector of a triangle with index tindx ----------
void normal(int tindx, float* x, float* y, float* z, int* t1, int* t2, int* t3)
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



void drawFloor()
{
    glColor3f(0., 0.5,  0.);            //Floor colour

    for(int i = -50; i <= 50; i ++)
    {
        glBegin(GL_LINES);          //A set of grid lines on the xz-plane
            glVertex3f(-50, -0.75, i);
            glVertex3f(50, -0.75, i);
            glVertex3f(i, -0.75, -50);
            glVertex3f(i, -0.75, 50);
        glEnd();
    }
}


void special (int key, int x, int y) 
{
    if (key == GLUT_KEY_RIGHT) {
        angle += 5;
        
    } else if (key == GLUT_KEY_LEFT) {
        angle -= 5;
        
    } else if (key == GLUT_KEY_UP) {
        camera_z_offset -= 3;
        camx += dx;
        camz += dz;
        
    } else if (key == GLUT_KEY_DOWN) {
        camera_z_offset += 3;
        camx += -dx;
        camz += -dz;
    }

    glutPostRedisplay();
}



void draw_axis (void)
{
    //x axis
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINES);
        glVertex3f(-40.0, 0.0f, 0.0f);
        glVertex3f(40.0, 0.0f, 0.0f);
    glEnd();
    glFlush();
 
    //y axis
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, -40.0f, 0.0f);
        glVertex3f(0.0, 40.0f, 0.0f);
    glEnd();
    glFlush();
 
    //z axis
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0f ,-40.0f );
        glVertex3f(0.0, 0.0f ,40.0f );
    glEnd();
    glFlush();
}


void draw_octagon(int x, int y, int z)
/* Takes the normal vector to the octagon and draws an octagon at with 
 * centre at (0, 0, 0) and with each side having length 1 */
{
    glPushMatrix();
        glRotatef(atan(0.5 / (0.5 + sqrt(0.5))) * (180 / M_PI), 0, 0, 1);
        glTranslatef(-(0.5 + sqrt(0.5)), -(0.5 + sqrt(0.5)), 0); //Moves the centre of the octagon to 0, 0, 0
        glTranslatef(0, -(1 - sqrt(0.5)), 0); //Moves the octagon down so that the bottom of the octagon touches the x axis
        glBegin(GL_POLYGON); 
            glNormal3f(x, y, z);
            glVertex3f(0, 1, 0);
            glVertex3f(0, 2, 0);
            glVertex3f(sqrt(0.5), 2 + sqrt(0.5), 0);
            glVertex3f(sqrt(0.5) + 1, 2 + sqrt(0.5), 0);
            glVertex3f(sqrt(0.5) * 2 + 1, 2, 0);
            glVertex3f(sqrt(0.5) * 2 + 1, 1, 0);
            glVertex3f(sqrt(0.5) + 1, 1 - sqrt(0.5), 0);
            glVertex3f(sqrt(0.5), 1 - sqrt(0.5), 0);
        glEnd();
    glPopMatrix();
}


void draw_pillar (int height)
{
    //Draws the bottom of the pillar (an octagon)
    glPushMatrix();
        glTranslatef(0, 0, 0.01);
        draw_octagon(0, 0, -1);
    glPopMatrix();
    
    //draws the sides of the pillar
    GLUquadricObj* p = gluNewQuadric();
    glPushMatrix();
        glRotatef(0, 1, 0, 0);
        gluCylinder(p, sqrt(0.5) + 0.6, sqrt(0.5) + 0.6, height, 8, 8);
    glPopMatrix();
    
    //Draws the top of the pillar (an octagon)
    glPushMatrix();
        glTranslatef(0, 0, 9.99);
        draw_octagon(0, 0, 1);
    glPopMatrix();
}


void draw_castle_entrance (void)
{
    glBegin(GL_TRIANGLES);
        for(int tindx = 0; tindx < ntri_castle; tindx++)
        {
           normal(tindx, x_castle, y_castle, z_castle, t1_castle, t2_castle, t3_castle);
           glVertex3d(x_castle[t1_castle[tindx]], y_castle[t1_castle[tindx]], z_castle[t1_castle[tindx]]);
           glVertex3d(x_castle[t2_castle[tindx]], y_castle[t2_castle[tindx]], z_castle[t2_castle[tindx]]);
           glVertex3d(x_castle[t3_castle[tindx]], y_castle[t3_castle[tindx]], z_castle[t3_castle[tindx]]);
        }
    glEnd();
}


void draw_castle_walls(void)
{
    //Drawing minor pillars
    glPushMatrix();
        glTranslatef(-7.5, 0, -13);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(10);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(15 + 7.5, 0, -13);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(10);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0, 0, -13 * 2);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(10);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(15, 0, -13 * 2);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(10);
    glPopMatrix();
    
    
    //Drawing connecting walls
    glPushMatrix();
        glRotatef(90 + asin(7.5 / 15) * (180 / M_PI), 0, 1, 0);
        glTranslatef(15/2.0, 9/2.0, 0);
        glScalef(15, 9, 1);
        glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0, 0, -13 * 2);
        glRotatef(-90 - asin(7.5 / 15) * (180 / M_PI), 0, 1, 0);
        glTranslatef(15/2.0, 9/2.0, 0);
        glScalef(15, 9, 1);
        glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0, 0, -13 * 2);
        glTranslatef(15/2.0, 9/2.0, 0);
        glScalef(15, 9, 1);
        glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(15, 0, -13 * 2);
        glRotatef(-90 + asin(7.5 / 15) * (180 / M_PI), 0, 1, 0);
        glTranslatef(15/2.0, 9/2.0, 0);
        glScalef(15, 9, 1);
        glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(15 + 7.5, 0, -13);
        glRotatef(-90 - asin(7.5 / 15) * (180 / M_PI), 0, 1, 0);
        glTranslatef(15/2.0, 9/2.0, 0);
        glScalef(15, 9, 1);
        glutSolidCube(1.0);
    glPopMatrix();
}


void draw_castle (void) 
{
    /* *** ENTRANCE *** */
    //Right entrance pillar
    glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        draw_pillar(14);
    glPopMatrix();
    
    //castle castle entrance
    glPushMatrix();
        glTranslatef((sqrt(0.5) + 0.6) / 2, 0, (sqrt(0.5) + 0.6) / 2);
        draw_castle_entrance();
    glPopMatrix();
    
    //Left entrance pillar
    glPushMatrix();
        glTranslatef(15, 0, 0);
        glRotatef(-90, 1, 0, 0);
        draw_pillar(14);
    glPopMatrix();
    
    /* *** WALLS *** */ 
    draw_castle_walls();
}

void display (void)
{
    float light_position[4] = {0, 10, 10, 1};
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    dx = cos(angle * (M_PI/180));
    dz = sin(angle * (M_PI/180));
    
    gluLookAt(camx, 10, camz, camx+dx, 10, camz+dz,0.0f, 1.0f, 0.0f);
    
    //gluLookAt(7.5, 20, 30 + camera_z_offset, 7.5, 8, camera_z_offset, 0, 1, 0);
    
    //glTranslatef(0, 0, 0 + camera_z_offset * 2);
    //glRotatef(angle, 0, 1, 0);
    //glTranslatef(-7.5, -20, -(30+ camera_z_offset));
    
    
    glPushMatrix();
        draw_axis();
    glPopMatrix();
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glDisable(GL_LIGHTING);
    drawFloor();
    
    glEnable(GL_LIGHTING);
    glColor3f(0.0, 0.0, 1.0);
    
    draw_castle();
    
    glutSwapBuffers();
}



void initialize (void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    loadMeshFile("./castle_gate.off", &x_castle, &y_castle, &z_castle, &t1_castle, &t2_castle, &t3_castle, &nvrt_castle, &ntri_castle);	
    loadMeshFile("./Cannon.off", &x_cannon, &y_cannon, &z_cannon, &t1_cannon, &t2_cannon, &t3_cannon, &nvrt_cannon, &ntri_cannon);
    
    glEnable(GL_LIGHTING); //Turn on lights
    glEnable(GL_LIGHT0);
    
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);
}




int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Assignment One");
    
    initialize();
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    //glutKeyboardFunc(angle_rotate);
    glutMainLoop();
    return 0;
}
