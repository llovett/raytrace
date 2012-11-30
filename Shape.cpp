#include "Shape.h"
#include <math.h>

GLfloat Shape::dot(GLfloat *d1, GLfloat *d2) const {
    return d1[0]*d2[0] + d1[1]*d2[1] + d1[2]*d2[2];
}

void Shape::normalize(GLfloat *d) const {
    GLfloat origin[3] = {0, 0, 0};
    GLfloat magnitude = dist(origin, d);
    for ( int i=0; i<3; i++ ) {
	d[i] /= magnitude;
    }
}

GLfloat dist(GLfloat *d1, GLfloat *d2) {
    return sqrt( (d1[0]-d2[0])*(d1[0]-d2[0]) +
		 (d1[1]-d2[1])*(d1[1]-d2[1]) +
		 (d1[2]-d2[2])*(d1[2]-d2[2]) );
}
