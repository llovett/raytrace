#include "Shape.h"
#include <algorithm>
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

void Shape::setMaterial(mProps *mat) {
    this->material = mat;
}

GLfloat *Shape::getColor() const {
    if ( this->material ) {
	GLfloat *color = new GLfloat[4];
	std::copy(this->material->color, this->material->color+4, color);
	return color;
    }
    return NULL;
}

GLfloat *Shape::getAmbient() const {
    if ( this->material ) {
	return this->material->ambient;
    }
    return NULL;
}

GLfloat *Shape::getDiffuse() const {
    if ( this->material ) {
	return this->material->diffuse;
    }
    return NULL;
}

GLfloat *Shape::getSpecular() const {
    if ( this->material ) {
	return this->material->specular;
    }
    return NULL;
}

GLfloat Shape::getShininess() const {
    if ( this->material ) {
	return this->material->shininess;
    }
    return 0.0f;
}

GLfloat dist(GLfloat *d1, GLfloat *d2) {
    return sqrt( (d1[0]-d2[0])*(d1[0]-d2[0]) +
		 (d1[1]-d2[1])*(d1[1]-d2[1]) +
		 (d1[2]-d2[2])*(d1[2]-d2[2]) );
}
