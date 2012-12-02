#ifndef SHAPE_H
#define SHAPE_H

#include "trace.h"
#include <GL/glut.h>

class Shape {
public:
    virtual intersection *intersect(ray *) const = 0;
    mProps *material;
    void setMaterial(mProps *mat);
    GLfloat *getAmbient() const;
    GLfloat *getDiffuse() const;
    GLfloat *getSpecular() const;
    GLfloat getShininess() const;
    GLfloat *getColor() const;
};

GLfloat dist(GLfloat*, GLfloat*);
GLfloat *normalize(GLfloat *);
GLfloat dot(GLfloat*, GLfloat*);

#endif
