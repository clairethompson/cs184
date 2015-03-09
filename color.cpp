#include "color.h"

Color::Color(float r, float g, float b) {
  this->r = r;
  this->g = g;
  this->b = b;
}
Color::Color() {
  this->r = 0;
  this->g = 0;
  this->b = 0;
}

/* Compute the dot product between c1 and c2 */
float Color::dotProduct(Color c1, Color c2) {
	return (c1.r*c2.r) + (c1.g*c2.g) + (c1.b*c2.b);
}

/* Add color with c1 */
Color Color::operator+(Color c1) {
  Color c = Color(this->r + c1.r, this->g + c1.g, this->b + c1.b);
  return c;
} 

/* Multiply color by scalar f*/
Color Color::operator*(float f) {
  Color c = Color(this->r * f, this->g * f, this->b * f);
  return c;
}

/* Multiply color by other color */
Color Color::operator*(Color c1) {
  Color c = Color(this->r * c1.r, this->g * c1.g, this->b * c1.b);
  return c;
}

/* Assign color to c1 */
void Color::operator=(Color c1) {
  this->r = c1.r;
  this->g = c1.g;
  this->b = c1.b;
}
