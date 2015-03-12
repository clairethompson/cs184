#ifndef VECTOR_H
#define VECTOR_H

#include "point.h"
class Vector {
public:
	float x;
	float y;
	float z;

public:
	Vector() {};
	Vector(float a, float b, float c);
	Vector(Point a, Point b);

	/* Returns the vector length of V */
	float getLength();

	/* Normalize vector V */
	void normalize();

	/* Returns the dot product of the vector V1 */
	float dot(Vector v1);

	/* Returns the cross product of the vector V1 */
	Vector cross(Vector v1);

	/* Multiply vector by scalar float F */
	Vector operator*(float f);

	/* Add vector and V1, returning V */
	Vector operator+(Vector v1);

	/* Subtract v1 from vector, returning V */
	Vector operator-(Vector v1);

	/* Assigns vector to v */
	void operator=(Vector v);
	
};

#endif