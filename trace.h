#include <GL/glut.h>

#ifndef TRACE_H
#define TRACE_H

/* models a ray */
typedef struct {
    /* these should have exactly three dimensions each */
    GLfloat *point;
    GLfloat *direction;
} ray;

/* models an intersection of a ray with a Shape */
typedef struct {
    /* point and normal have three dimensions */
    GLfloat *point;
    GLfloat *normal;
    /* value of t for which given ray in intersect() method will hit the shape */
    GLfloat t;

    /* shapeNumber = index into Shapes[] array of shape that is intersected. */
    int shapeNumber;
} intersection;

/* defines properties for a material */
typedef struct {
    /* color of the material's surface */
    GLfloat *color;
    /* how much ambient, diffuse, and specular light are
     * reflected by this surface, respectively */
    GLfloat *ambient, *diffuse, *specular;
    /* shininess- how focused the reflection is, not how bright! */
    GLfloat shininess;
} mProps;

/* defines properties for lighting */
typedef struct {
    /* these each have 4 dimensions. They represent the colors of the
     * ambient, diffuse, and specular lighting emitted by this light. */
    GLfloat *ambient;
    GLfloat *diffuse;
    GLfloat *specular;
} lProps;

#endif
