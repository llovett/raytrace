#include <GL/glut.h>

#ifndef TRACE_H
#define TRACE_H

typedef struct {
    /* these should have exactly three dimensions each */
    GLfloat *point;
    GLfloat *direction;
} ray;

typedef struct {
    /* point and normal have three dimensions */
    GLfloat *point;
    GLfloat *normal;
    int objectNumber;
} intersection;

#endif
