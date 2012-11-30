#ifndef SHAPE_H
#define SHAPE_H

#include "trace.h"
#include <GL/glut.h>

class Shape {
public:
    virtual intersection *intersect(ray *) const = 0;

protected:
    GLfloat dot(GLfloat*, GLfloat*) const;
    void normalize(GLfloat *) const;
};

GLfloat dist(GLfloat*, GLfloat*);

#endif
