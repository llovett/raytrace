#include "Plane.h"
#include "Matrix.h"
#include "trace.h"
#include <math.h>
#include <iostream>

using namespace std;

intersection *Plane::intersect(ray *r) const {
    /* find a normal to the plane */
    float v1[3] = {
	x2 - x1,
	y2 - y1,
	z2 - z1
    };
    float v2[3] = {
	x3 - x2,
	y3 - y2,
	z3 - z2
    };
    GLfloat *N = crossProduct( v1, v2 );
    normalize(N);
    
    /* ray-plane intersection */
    GLfloat t;
    float NdotV = dot(N, r->direction);
    /* does not intersect plane */
    if ( NdotV < EPSILON ) {
	return NULL;
    }
    
    GLfloat D = N[0]*x1 + N[1]*y1 + N[2]*z1;
    t = (D - dot(N, r->point)) / NdotV;

    /* find intersection point */
    GLfloat *intersectPoint = new GLfloat[3];
    for ( int i = 0; i < 3; ++i ) {
	intersectPoint[i] = r->point[i] + t*r->direction[i];
    }

    /* create intersection object out of normal and intersection point */
    intersection *i = new intersection();
    i->point = intersectPoint;
    i->normal = N;
    i->t = t;
    return i;
}

