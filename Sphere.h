#include <GL/glut.h>
#include "Shape.h"
#include "trace.h"

class Sphere : public Shape {
public:
    virtual intersection *intersect(ray *) const;
    Sphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->radius = radius;
    };

private:
    GLfloat radius;
    GLfloat x, y, z;
};
