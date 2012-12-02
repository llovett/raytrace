#include "Sphere.h"
#include <iostream>
#include <math.h>

// int main() {
//     Sphere *s = new Sphere(0, 0, 0, 5);
//     GLfloat point[3] = {10, 10, 0};
//     GLfloat direction[3] = {-9,-10,0};
//     ray r;
//     r.point = point;
//     r.direction = direction;

//     intersection *i = s->intersect(&r);
//     if ( i ) {
// 	std::cout << "Intersection at ("<<i->point[0]<<","<<i->point[1]<<","<<i->point[2]<<")"<<std::endl;
// 	std::cout << "Normal at intersection is ("<<i->normal[0]<<","<<i->normal[1]<<","<<i->normal[2]<<")"<<std::endl;
//     } else {
// 	std::cout << "No intersection." << std::endl;
//     }

//     return 0;
// }

// void printVector(GLfloat *v) {
//     int size = 3;
//     std::cout << "( ";
//     for ( int i=0; i<size; i++ ) {
// 	std::cout << v[i] << " ";
//     }
//     std::cout << ")" <<std::endl;
// }

intersection *Sphere::intersect(ray *r) const {
    GLfloat *P0 = r->point;
    GLfloat *v = r->direction;
    normalize(v);

    /* set up the equation to find the intersection
     * (see p. 18 of "VectorGeometry.pdf")
     * */
    GLfloat A[3] = { this->x - P0[0],
		     this->y - P0[1],
		     this->z - P0[2] };

    double vDotA = dot(v, A);
    double vSq = dot(v, v);
    double ASq = dot(A, A);
    double sqrtQuantity = 4*vDotA*vDotA - 4*vSq*(ASq - this->radius*this->radius);

    /* there is no intersection */
    if ( sqrtQuantity < 0 ) {
	return NULL;
    }
    double tPositive = (2*vDotA + sqrt(sqrtQuantity)) / 2*vSq;
    double tNegative = (2*vDotA - sqrt(sqrtQuantity)) / 2*vSq;

    /* no positive t value? */
    if ( tPositive < 0 && tNegative < 0 ) {
	return NULL;
    }
    double t = tPositive < tNegative? tPositive : tNegative;

    /* use the t-values to determine point of intersection */
    GLfloat *intersectPoint = new GLfloat[3];
    intersectPoint[0] = P0[0] + t*v[0];
    intersectPoint[1] = P0[1] + t*v[1];
    intersectPoint[2] = P0[2] + t*v[2];

    /* calculate the normal at the point of intersection */
    GLfloat *normal = new GLfloat[3];
    normal[0] = this->x - intersectPoint[0];
    normal[1] = this->y - intersectPoint[1];
    normal[2] = this->z - intersectPoint[2];
    normalize(normal);
    
    /* create intersection object out of normal and intersection point */
    intersection *i = new intersection();
    i->point = intersectPoint;
    i->normal = normal;
    i->t = t;
    return i;
}

