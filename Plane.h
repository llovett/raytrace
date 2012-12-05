#include <GL/glut.h>
#include "Shape.h"
#include "trace.h"

class Plane : public Shape {
public:
    virtual intersection *intersect(ray *) const;
    Plane(GLfloat x1, GLfloat y1, GLfloat z1,
	  GLfloat x2, GLfloat y2, GLfloat z2,
	  GLfloat x3, GLfloat y3, GLfloat z3) {
	this->x1 = x1;
	this->y1 = y1;
	this->z1 = z1;
	this->x2 = x2;
	this->y2 = y2;
	this->z2 = z2;
	this->x3 = x3;
	this->y3 = y3;
	this->z3 = z3;
    };

private:
    GLfloat x1, y1, z1, x2, y2, z2, x3, y3, z3;
};
