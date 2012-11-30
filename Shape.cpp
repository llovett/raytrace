#include "Shape.h"
#include <math.h>

GLfloat Shape::dist(GLfloat *d1, GLfloat *d2) const {
    return sqrt( (d1[0]-d2[0])*d1[0]-d2[0] +
		 (d1[1]-d2[1])*d1[1]-d2[1] +
		 (d1[2]-d2[2])*d1[2]-d2[2] );
}

GLfloat Shape::dot(GLfloat *d1, GLfloat *d2) const {
    return d1[0]*d2[0] + d1[1]*d2[1] + d1[2]*d2[2];
}
