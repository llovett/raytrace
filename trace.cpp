/**
 * Trace.cpp
 *
 * Ray tracer that renders a scene of a big red "sun" with several green "planets".
 *
 * Author: Luke Lovett
 * Last modified: Thu Dec  6 00:44:41 EST 2012
 *
 * */

#include <GL/glut.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "trace.h"
#include "Sphere.h"
#include "Plane.h"
#include "Shape.h"

#define WIDTH 800
#define HEIGHT 800

/* N is the height of the bitmap; M is its width */
#define N WIDTH
#define M HEIGHT
/* How deep we should be ray-tracing */
#define MAX_DEPTH 5
/* Minimum reflection constant needed to calculate a ray-trace for light */
#define MIN_WEIGHT 0.01
/* The index-of-refraction of air (which we assume empty space in the scene is filled with)*/
#define AIR_REFRACTION 1

#define PI 3.14159

/* scene properties */
#define NUM_SPHERES 12
#define SPHERE_SIZE 0.35
#define SPHERE_DIST 3
#define SUN_X -2
#define SUN_Y -1
#define SUN_Z 1.0

using namespace std;

/* Position of the viewer in word coords */
GLfloat ViewerPosition[3] = {15.0, 0.0, 2.5};
/* Upper left corner pixel grid */
GLfloat GridX = 10, GridY = -2, GridZ = 4;
/* dimensions of the pixel grid. */
GLfloat GridWidth = 4, GridHeight = 4;

/* the bitmap */
GLfloat image[N][M][3];

/* primary colors for convenience */
GLfloat RED[4] = {1.0, 0.0, 0.0, 1.0};
GLfloat BLUE[4] = {0.0, 0.0, 1.0, 1.0};
GLfloat GREEN[4] = {0.0, 1.0, 0.0, 1.0};
GLfloat BLACK[4] = {0.0, 0.0, 0.0, 1.0};
GLfloat BACKGROUND[4] = {1.0, 1.0, 1.0, 1.0};

/* shapes in the scene */
vector<Shape*> Shapes;
/* lights in the scene */
vector<lProps*> Lights;

void init() {
    glClearColor(0.9, 0.9, 0.9, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT );
}

void reshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLfloat)w, 0.0, (GLfloat)h );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);
}

mProps *buildMaterial(GLfloat color[4],
		      GLfloat ambient[3],
		      GLfloat diffuse[3],
		      GLfloat specular[3],
		      GLfloat shininess,
		      GLfloat refract) {
    mProps *props = new mProps();

    GLfloat *myColor = new GLfloat[4];
    for ( int i=0; i<4; i++ ) {
	myColor[i] = color[i];
    }
    GLfloat *myAmbient = new GLfloat[3];
    GLfloat *myDiffuse = new GLfloat[3];
    GLfloat *mySpecular = new GLfloat[3];
    for ( int i = 0; i < 3; ++i ) {
	myAmbient[i] = ambient[i];
	myDiffuse[i] = diffuse[i];
	mySpecular[i] = specular[i];
    }

    props->ambient = myAmbient;
    props->diffuse = myDiffuse;
    props->specular = mySpecular;
    props->shininess = shininess;
    props->refract = refract;
    props->color = myColor;

    return props;
}

lProps *buildLight(GLfloat ared, GLfloat agreen, GLfloat ablue, GLfloat aalpha,
		   GLfloat dred, GLfloat dgreen, GLfloat dblue, GLfloat dalpha,
		   GLfloat sred, GLfloat sgreen, GLfloat sblue, GLfloat salpha,
		   GLfloat x, GLfloat y, GLfloat z) {
    lProps *props = new lProps();
    GLfloat *ambient = new GLfloat[4];
    GLfloat *diffuse = new GLfloat[4];
    GLfloat *specular = new GLfloat[4];

    ambient[0] = ared;
    ambient[1] = agreen;
    ambient[2] = ablue;
    ambient[3] = aalpha;

    diffuse[0] = dred;
    diffuse[1] = dgreen;
    diffuse[2] = dblue;
    diffuse[3] = dalpha;

    specular[0] = sred;
    specular[1] = sgreen;
    specular[2] = sblue;
    specular[3] = salpha;

    props->ambient = ambient;
    props->diffuse = diffuse;
    props->specular = specular;

    props->x = x;
    props->y = y;
    props->z = z;

    return props;
}

/**
 * buildScene()
 *
 * Constructs all of the objects and materials that will be rendered
 * in the scene as well as building any lights.
 * */
void buildScene() {
    /* some objects */

    GLfloat color[4] = { 0.5, 1.0, 1.0, 0.5 };
    GLfloat ambient[3] = { 0.0, 0.3, 0.6 };
    GLfloat diffuse[3] = { 0.9, 0.9, 0.9 };
    GLfloat specular[3] = { 1.0, 1.0, 1.0 };
    mProps *diffuseBlueMaterial = buildMaterial(color, ambient, diffuse, specular, 50, 1.333);

    double inc = 2*PI / NUM_SPHERES;
    for ( int i = 0; i < NUM_SPHERES; ++i ) {
	double x = cos(i*inc) * SPHERE_DIST + SUN_X;
	double y = sin(i*inc) * SPHERE_DIST + SUN_Y;
	double z = sin(i*inc) * 1.0 + SUN_Z;

	Sphere *s = new Sphere(x, y, z, SPHERE_SIZE);
	s->setMaterial( diffuseBlueMaterial );
	Shapes.push_back( s );
    }

    Sphere *bigRed = new Sphere(SUN_X, SUN_Y, SUN_Z, 2);
    GLfloat color2[4] = { 1.0, 0.8, 0.5, 1.0 };
    GLfloat ambient2[3] = { 0.6, 0.3, 0.0 };
    GLfloat diffuse2[3] = { 0.9, 0.9, 0.9 };
    GLfloat specular2[3] = { 0.8, 0.8, 0.8 };
    mProps *redMaterial = buildMaterial(color2, ambient2, diffuse2, specular2, 10, AIR_REFRACTION);
    bigRed->setMaterial( redMaterial );

    Plane *floor = new Plane(0, 0, -2,
			     0, 1, -2,
			     1, 0, -2);
    GLfloat floorColor[4] = { 0.1, 0.5, 0.2, 1.0 };
    GLfloat floorAmbient[4] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat floorDiffuse[4] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat floorSpecular[4] = { 0.3, 0.3, 0.3, 1.0 };
    mProps *floorMaterial = buildMaterial(floorColor, floorAmbient, floorDiffuse, floorSpecular, 15, AIR_REFRACTION);
    floor->setMaterial( floorMaterial );

    GLfloat wallColor[4] = { 0.3, 0.6, 0.0, 1.0 };
    GLfloat wallSpecular[4] = { 0.0, 0.0, 0.0, 1.0 };
    mProps *wallMaterial = buildMaterial(wallColor, floorAmbient, floorDiffuse, wallSpecular, 0, AIR_REFRACTION);

    Plane *wall = new Plane(-7, 0, 0,
			    -7, 1, 1,
			    -7, 1, 0);
    wall->setMaterial( wallMaterial );

    Shapes.push_back( bigRed );
    Shapes.push_back( wall );
    Shapes.push_back( floor );

    /* some lights */
    lProps *blueLight = buildLight(
	0.2, 0.2, 0.8, 1.0,	/* ambient color */
	0.2, 0.2, 0.9, 1.0,	/* diffuse color */
	1.0, 1.0, 1.0, 1.0, 	/* specular color */
    	0, -1, 4
	);
    lProps *redLight = buildLight(
    	0.9, 0.2, 0.1, 1.0,	/* ambient color */
    	1.0, 0.2, 0.2, 1.0,	/* diffuse color */
    	1.0, 0.0, 0.0, 1.0, 	/* specular color */
	0, 2, 4
    	);
    Lights.push_back( blueLight );
    Lights.push_back( redLight );
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(0, 0);
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

GLfloat intersectLight( lProps *light, ray *r ) {
    return (light->x - r->point[0])/r->direction[0];
}

intersection *intersect( ray *r ) {
    vector<intersection*> intersections;

    /* intersect ray with every shape in scene */
    for ( int i=0; i<Shapes.size(); i++ ) {
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
	if ( !closest || x->t < closest->t ) {
	    closest = x;
	}
    }

    return closest;
}

GLfloat *trace(ray *r, int level, GLfloat *weight, int shouldReflect) {
    // This returns the color of the ray
    intersection *p;
    GLfloat *color = new GLfloat[4];

    // Base case: max level reached
    if ( level > MAX_DEPTH ) {
	copy(BLACK, BLACK+4, color);
	return color;
    }
    // Base case: minimum weight reached
    int minWeightReached = 1;
    for ( int i = 0; i < 3; ++i ) {
	if ( weight[i] >= MIN_WEIGHT ) {
	    minWeightReached = 0;
	    break;
	}
    }
    if ( minWeightReached ) {
	copy(BLACK, BLACK+4, color);
	return color;
    }

    p = intersect(r);
    if (p != NULL) {
	Shape *theShape = Shapes[p->shapeNumber];

	/***********************/
	/* AMBIENT CALCULATION */
	/***********************/
	GLfloat *shapeColor = theShape->getColor();
	GLfloat *Ka = theShape->getAmbient();
	GLfloat ambientReflection[3] = {
	    Ka[0] * shapeColor[0],
	    Ka[1] * shapeColor[1],
	    Ka[2] * shapeColor[2]
	};
	/* Find net ambient light from all light sources */
	GLfloat ambientLight[3] = { 0.0, 0.0, 0.0 };
	for ( int i=0; i<Lights.size(); i++ ) {
	    lProps *light = Lights[i];
	    ambientLight[0] += ambientReflection[0] * light->ambient[0];
	    ambientLight[1] += ambientReflection[1] * light->ambient[1];
	    ambientLight[2] += ambientReflection[2] * light->ambient[2];
	}

	/***********************/
	/* DIFFUSE CALCULATION */
	/***********************/
	GLfloat diffuseLight[3] = { 0.0, 0.0, 0.0 };
	GLfloat *L;
	GLfloat *SN;
	for ( int i = 0; i < Lights.size(); ++i ) {
	    ray r;
	    GLfloat *point = p->point;
	    r.point = point;
	    GLfloat d[3] = {
		point[0] - Lights[i]->x,
		point[1] - Lights[i]->y,
		point[2] - Lights[i]->z
	    };
	    r.direction = d;
	    normalize( r.direction );
	    GLfloat t = intersectLight( Lights[i], &r );

	    /* see if there are any objects between intersection and light */
	    int impeded = 0;	/* assume nothing in the way */
	    for ( int j = 0; j < Shapes.size(); ++j ) {
		intersection *x = Shapes[j]->intersect( &r );
		if ( x && x->t < t ) {
		    impeded = 1;
		    break;
		}
	    }

	    L = r.direction;
	    SN = p->normal;
	    /* there was nothing in the way, so calculate diffuse light */
	    if ( !impeded ) {
		GLfloat *Kd = theShape->getDiffuse();
		GLfloat *intensity = Lights[i]->diffuse;

		for ( int k=0; k<3; k++ ) {
		    diffuseLight[k] += Kd[k] * dot(L,SN) * intensity[k] * theShape->getColor()[k];
		}
	    }
	}

	/************************/
	/* SPECULAR CALCULATION */
	/************************/

	// Calculate reflection vector
	// Since every term in the calculation is already normalized,
	// R itself should be normalized after this.
	GLfloat R[3];
	GLfloat LdotN = dot(L, SN);
	for ( int i=0; i<3; i++ ) {
	    R[i] = 2*LdotN*SN[i] - L[i];
	}
	normalize( R );

	// Get vector to the viewer
	GLfloat V[3];
	for ( int i=0; i<3; i++ ) {
	    V[i] = p->point[i] - ViewerPosition[i];
	}
	normalize( V );
	
	GLfloat specularLight[3] = { 0.0, 0.0, 0.0 };
	for ( int i=0; i<Lights.size(); i++ ) {
	    lProps *theLight = Lights[i];
	    GLfloat RdotVtoTheN = powf(dot(R, V), theShape->getShininess());
	    for ( int j=0; j<3; j++ ) {
		specularLight[j] += theShape->getSpecular()[j] * theLight->specular[j] * RdotVtoTheN;
	    }
	}

	/**************************/
	/* REFLECTION CALCULATION */
	/**************************/
	GLfloat reflect[3];
	GLfloat* reflectionLight;
	GLfloat c = -dot( SN, r->direction );
	if ( shouldReflect ) {
	    for ( int i = 0; i < 3; ++i ) {
		reflect[i] = r->direction[i] + 2*p->normal[i]*c;
	    }
	    normalize( reflect );
	    ray reflectRay;
	    reflectRay.point = p->point;
	    reflectRay.direction = reflect;
	    GLfloat newWeight[3] = {
		weight[0] * theShape->getSpecular()[0],
		weight[1] * theShape->getSpecular()[1],
		weight[2] * theShape->getSpecular()[2]
	    };
	    reflectionLight = trace( &reflectRay, level+1, newWeight, 1 );
	    for ( int i = 0; i < 3; ++i ) {
		reflectionLight[i] *= newWeight[i];
	    }
	} else {
	    reflectionLight = new GLfloat[4];
	    copy(BLACK, BLACK+4, reflectionLight);
	}

	/**************************/
	/* REFRACTION CALCULATION */
	/**************************/
	
	GLfloat *refractionLight;
	GLfloat n = AIR_REFRACTION / theShape->getRefraction();
	if ( theShape->material->color[3] < 1 ) {
	    GLfloat refract[3];
	    GLfloat c2 = sqrtf(1 - n*n*(1 - c*c));
	    for ( int i = 0; i < 3; ++i ) {
	    	refract[i] = n*r->direction[i] + n*c - c2 * SN[i];
	    }
	    // normalize( refract );
	    ray refractRay;
	    refractRay.point = p->point;
	    refractRay.direction = refract;
	    refractionLight = trace( &refractRay, level+1, weight, shouldReflect? 0 : 1 );
	} else {
	    refractionLight = new GLfloat[4];
	    copy(BLACK, BLACK+4, refractionLight);
	}

	/*************************/
        /* NET COLOR CALCULATION */
        /*************************/
	for ( int i=0; i<3; i++ ) {
	    color[i] = ambientLight[i] + diffuseLight[i] + specularLight[i] + reflectionLight[i] + refractionLight[i];
	}
	// color[3] = 1.0;
    } else if ( level == 0 ) {
	copy(BACKGROUND, BACKGROUND+4, color);
    } else {
	copy(BLACK, BLACK+4, color);
    }

    // Clamp the color
    for ( int i = 0; i < 3; ++i ) {
	color[i] = color[i] > 1? 1 : color[i];
	color[i] = color[i] < EPSILON? 0 : color[i];
    }

    
    return color;
}

void makePicture() {
    // return;
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
    	    GLfloat startingWeight[] = { 1.0, 1.0, 1.0 };
    	    color = trace(&r, 0, startingWeight, 1);
    	    copy(color, color+2, image[N-i-1][j]);
    	}
    }
}

int main(int argc, char** argv) {
    buildScene();
    makePicture();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Ray Traced Image");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    init();
    glutMainLoop();
}
