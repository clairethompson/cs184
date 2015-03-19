
#include "vector.h"
#include "point.h"
#include "math.h"

Vector::Vector(float a,  float b,  float c) {
	x = a;
	y = b;
	z = c;
}

Vector::Vector(Point a,  Point b) {
	x = a.x - b.x;
	y = a.y - b.y;
	z = a.z - b.z;
}

Vector::Vector(Normal n) {
	this->x = n.getX();
	this->y = n.getY();
	this->z = n.getZ();
}

/* Returns the vector length of V */
float Vector::getLength() {
	return sqrt(pow(x,2)+pow(y,2)+pow(z,2));
}

/* Normalize vector V */
void Vector::normalize() {
	float v_len = getLength();
	x = x/v_len;
	y = y/v_len;
	z = z/v_len;
}

/* Returns the dot product of V1 and V2 */
float Vector::dot(Vector v1) {
	return this->x*v1.x + this->y*v1.y + this->z*v1.z;
}

/* Returns the cross product of V1 and V2 */
Vector Vector::cross(Vector v1) {
	float a = this->y * v1.z - this->z * v1.y;
	float b = this->z * v1.x - this->x * v1.z;
	float c = this->x * v1.y - this->y * v1.x;
	return Vector(a, b, c);
}

/* Multiply vector by scalar float F */
Vector Vector::operator*(float f) {
	Vector v;
	v.x = this->x * f;
	v.y = this->y * f;
	v.z = this->z * f;
	return v;
}

/* Add vector and V1, returning V */
Vector Vector::operator+(Vector v1) {
	Vector v; 
	v.x = v1.x + this->x;
	v.y = v1.y + this->y;
	v.z = v1.z + this->z;
	return v;
}

/* Add vector and point, returning V */
Vector Vector::operator+(Point p) {
	Vector v; 
	v.x = p.x + this->x;
	v.y = p.y + this->y;
	v.z = p.z + this->z;
	return v;
}

/* Subtract v1 from vector, returning V */
Vector Vector::operator-(Vector v1) {
	Vector v; 
	v.x = this->x - v1.x;
	v.y = this->y - v1.y;
	v.z = this->z - v1.z;
	return v;
}

/* Assigns vector to v */
void Vector::operator=(Vector v) {
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}
