/*     trace.cpp  */

/* This is a simple non-recursive "ray tracer" */
/* The only object in the scene is a rectangle with */
/* x=0, -2<=y<2, 0<=z<=4.  */


#include <GL/glut.h>

// N is the height of the bitmap; M is its width
#define N 200 
#define M 400 

struct ray {
    GLfloat point[3];
    GLfloat direction[3];
};

struct intersection {
    GLfloat point[3];
    GLfloat normal[3];
    int objectNumber;
};

GLfloat ViewerPosition[3] = {15.0, 0.0, 2.0};
GLfloat GridX = 10, GridY = -2, GridZ = 3; // Upper left corner pixel grid 
                                          
GLfloat GridWidth = 4, GridHeight = 2;  // dimensions of the pixel grid. 

GLfloat PolyWidth = 4, PolyHeight = 4;  // Dimensions of a polygon with one
                                        // vertex at the orign
GLfloat image[N][M][3];			// This is the image bitmap
GLfloat RED[3] = {1.0, 0.0, 0.0};
GLfloat BLUE[3] = {0.0, 0.0, 1.0};
GLfloat GREEN[3] = {0.0, 1.0, 0.0};


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
    glRasterPos2i(300-M/2, 300-N/2);  // position of the lower left corner
    // of the bitmap in window coords

    glDrawPixels(M, N, GL_RGB, GL_FLOAT, image);  
    // draws rows 0 to N, cols 0 to M of the bitmap "image".  
    // The bitmap is created below in makeBitmap(), which is 
    // the first function called in the program.

    glFlush();
}

void copy3(GLfloat *x, GLfloat *y) {
    /* sets x = y */
    int i;
    for (i = 0; i < 3; i++) {
	x[i] = y[i];
    }
}

void MakeRay(int i, int j, struct ray *r) {
    // This makes the ray from the viewer through pixel (i,j)
    GLfloat direction[3];
    GLfloat pixel[3]; // The world coordinates of the (i, j) pixel
    int k;

    copy3(r->point, ViewerPosition);
    pixel[0] = GridX;
    pixel[1] = GridY + (GridWidth*j)/M;
    pixel[2] = GridZ - (GridHeight*i)/N;
    for (k = 0; k < 3; k++ ) {
	direction[k] = pixel[k] - r->point[k];
    }
    copy3(r->direction, direction);
}

struct intersection *Intersect( struct ray *r ) {
    // This returns a non-null value if the ray intersects 
    // our polygon in the y-z plane
    // Your Intersect( ) method will be more complex.

    struct intersection *data;

    GLfloat t = -(r->point[0]/r->direction[0]);
    // t is the t-value corresponding to x=0.
    GLfloat x = 0;
    GLfloat y = r->point[1] + t*r->direction[1];
    GLfloat z = r->point[2] + t*r->direction[2];
    if ( (-PolyWidth/2 <= y) && (y <= PolyWidth/2) && (0 <= z) && (z <= PolyHeight)) {
	data = (intersection*)malloc( sizeof(struct intersection) );
	data->point[0] = x;
	data->point[1] = y;
	data->point[2] = z;
	return data;
    }
    else
	return NULL;
}

GLfloat *Trace(struct ray *r, int level, float weight) {
    // This returns the color of the ray
    struct intersection *p;
    GLfloat *color = (GLfloat*)malloc(3*sizeof(GLfloat));
    p = Intersect(r);
    if (p != NULL) {
	if (p->point[1] < 0)
	    copy3(color, RED);
	else
	    copy3(color, GREEN);
    }
    else
	copy3(color, BLUE);
    return color;
}


  
void MakePicture() {  
    // This runs through the pixel grid, makes a ray from the
    // viewer through the pixel, and traces this ray.
    // The pixel gets the color returned by the trace.
    struct ray r;
    int i, j;
    GLfloat *color;

    for (i =0; i < N; i++) {
	for (j = 0; j < M; j++ ) {
	    MakeRay(i, j, &r);
	    color = Trace(&r, 0, 1.0);
	    copy3(image[i][j], color);
	}
    }
}

int main(int argc, char** argv) {
    MakePicture();
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


