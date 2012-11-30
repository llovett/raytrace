#include <GL/glut.h>
#include "Shape.h"

#ifndef TRACE_H
#define TRACE_H

class Shape;

typedef struct {
    /* these should have exactly three dimensions each */
    GLfloat *point;
    GLfloat *direction;
} ray;

typedef struct {
    /* point and normal have three dimensions */
    GLfloat *point;
    GLfloat *normal;

    /* shape that is intersected.
     * is the index into Shapes[] array */
    int shapeNumber;
} intersection;

#endif
