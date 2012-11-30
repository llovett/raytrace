/*     trace.cpp  */

#include <GL/glut.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include "trace.h"
#include "Sphere.h"
#include "Shape.h"

/* N is the height of the bitmap; M is its width */
#define N 200
#define M 400
/* How deep we should be ray-tracing */
#define RECURSIVE_DEPTH 3

using namespace std;

/* Position of the viewer in word coords */
GLfloat ViewerPosition[3] = {15.0, 0.0, 2.0};
/* Upper left corner pixel grid */
GLfloat GridX = 10, GridY = -2, GridZ = 3;
/* dimensions of the pixel grid. */
GLfloat GridWidth = 4, GridHeight = 2;  
/* dimensions of the polygon with one vertex at the origin */
GLfloat PolyWidth = 4, PolyHeight = 4;

/* the bitmap */
GLfloat image[N][M][3];

/* primary colors for convenience */
GLfloat RED[3] = {1.0, 0.0, 0.0};
GLfloat BLUE[3] = {0.0, 0.0, 1.0};
GLfloat GREEN[3] = {0.0, 1.0, 0.0};

/* shapes in the scene (currently just 1) */
Shape *Shapes[] = { new Sphere(0, 0, 0, 3) };

void init() {
    glClearColor(1.0, 1.0, 0.0, 0.0);  // yellow background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 600.0, 0.0, 600.0 );
}

void reshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLfloat)w, 0.0, (GLfloat)h );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(300-M/2, 300-N/2);
    // position of the lower left corner
    // of the bitmap in window coords

    glDrawPixels(M, N, GL_RGB, GL_FLOAT, image);
    // draws rows 0 to N, cols 0 to M of the bitmap "image".
    // The bitmap is created below in makeBitmap(), which is
    // the first function called in the program.

    glFlush();
}

void makeRay(int i, int j, ray *r) {
    // This makes the ray from the viewer through pixel (i,j)
    GLfloat direction[3];
    // The world coordinates of the (i, j) pixel
    GLfloat pixel[3];
    int k;

    copy(ViewerPosition, ViewerPosition+3, r->point);

    pixel[0] = GridX;
    pixel[1] = GridY + (GridWidth*j)/M;
    pixel[2] = GridZ - (GridHeight*i)/N;
    for (k = 0; k < 3; k++ ) {
	direction[k] = pixel[k] - r->point[k];
    }
    copy(direction, direction+3, r->direction);
}

intersection *intersect( ray *r ) {
    vector<intersection*> intersections;

    /* intersect ray with every shape in scene */
    for ( int i=0; i<sizeof(Shapes)/sizeof(Shape); i++ ) {
	Shape *s = Shapes[i];
	intersection *x = s->intersect(r);
	if ( x ) {
	    x->shapeNumber = i;
	    intersections.push_back(x);
	}
    }
    
    /* find the closest intersection */
    intersection *closest = NULL;
    for ( int i=0; i<intersections.size(); i++ ) {
	intersection *x = intersections[i];
	if ( !closest || dist(x->point, r->point) < dist(closest->point, r->point) ) {
	    closest = x;
	}
    }

    return closest;
}

// intersection *Intersect( ray *r ) {
//     // This returns a non-null value if the ray intersects
//     // our polygon in the y-z plane
//     // Your Intersect( ) method will be more complex.

//     intersection *data;

//     GLfloat t = -(r->point[0]/r->direction[0]);
//     // t is the t-value corresponding to x=0.
//     GLfloat x = 0;
//     GLfloat y = r->point[1] + t*r->direction[1];
//     GLfloat z = r->point[2] + t*r->direction[2];
//     if ( (-PolyWidth/2 <= y) && (y <= PolyWidth/2) && (0 <= z) && (z <= PolyHeight)) {
// 	data = (intersection*)malloc( sizeof(intersection) );
// 	data->point[0] = x;
// 	data->point[1] = y;
// 	data->point[2] = z;
// 	return data;
//     }
//     else
// 	return NULL;
// }

GLfloat *trace(ray *r, int level, float weight) {
    // This returns the color of the ray
    intersection *p;
    GLfloat *color = new GLfloat[3];
    p = intersect(r);
    if (p != NULL) {
	if (p->point[1] < 0)
	    copy(RED, RED+3, color);
	else
	    copy(GREEN, GREEN+3, color);
    }
    else
	copy(BLUE, BLUE+3, color);
    return color;
}



void makePicture() {
    // This runs through the pixel grid, makes a ray from the
    // viewer through the pixel, and traces this ray.
    // The pixel gets the color returned by the trace.
    int i, j;
    GLfloat *color;

    for (i =0; i < N; i++) {
	for (j = 0; j < M; j++ ) {
	    ray r;
	    r.point = new GLfloat[3];
	    r.direction = new GLfloat[3];
	    makeRay(i, j, &r);
	    color = trace(&r, 0, 1.0);
	    copy(color, color+3, image[i][j]);
	}
    }
}

int main(int argc, char** argv) {
    makePicture();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Ray Traced Image");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
}


