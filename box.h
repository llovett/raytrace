#include <GL/glut.h>

class Box {
private:
    GLfloat width, height, depth;
    
public:
    Box(GLfloat width=5, GLfloat height=5, GLfloat depth=5) {
	this->width = width;
	this->height = height;
	this->depth = depth;
    };
    void render() const;
    void intersect() const;
};
