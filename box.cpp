#include "box.h"

void Box::render() const {
    glBegin(GL_QUADS);
    
    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(this->width, 0, 0);
    glVertex3f(this->width, 0, this->depth);
    glVertex3f(0, 0, this->depth);

    glNormal3f(0, 0, -1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, this->height, 0);
    glVertex3f(this->width, this->height, 0);
    glVertex3f(this->width, 0, 0);

    glNormal3f(-1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, this->depth);
    glVertex3f(0, this->height, this->depth);
    glVertex3f(0, this->height, 0);

    glNormal3f(1, 0, 0);
    glVertex3f(this->width, 0, 0);
    glVertex3f(this->width, 0, this->depth);
    glVertex3f(this->width, this->height, this->depth);
    glVertex3f(this->width, this->height, 0);

    glNormal3f(0, 1, 0);
    glVertex3f(0, this->height, 0);
    glVertex3f(this->width, this->height, 0);
    glVertex3f(this->width, this->height, this->depth);
    glVertex3f(0, this->height, this->depth);

    glNormal3f(0, 0, 1);
    glVertex3f(0, 0, this->depth);
    glVertex3f(this->width, 0, this->depth);
    glVertex3f(this->width, this->height, this->depth);
    glVertex3f(0, this->height, this->depth);

    glEnd();
}

void Box::intersect() const {
    /* nothing yet */
}
