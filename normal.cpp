#include "math.h"
#include "normal.h"

/* Init normal */
Normal::Normal(float x, float y, float z) {
  float length = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
  if (length == 0) {
    return;
  }
  this->x = x/length;
  this->y = y/length;
  this->z = z/length;
}

Normal::Normal() {
  this->x = 1.0;
  this->y = 0.0;
  this->z = 0.0;
}

/* Getter Functions*/
float Normal::getX() { return this->x; }
float Normal::getY() { return this->y; }
float Normal::getZ() { return this->z; }

/* Add two normals */
Normal Normal::operator+(Normal n1) {
  Normal n(this->x + n1.x, this->y + n1.y, this->z + n1.z);
  return n;
}

/* Subtract two normals */
Normal Normal::operator-(Normal n1) {
  Normal n(this->x - n1.x, this->y - n1.y, this->z - n1.z);
  return n;
}

/* Assign normal */
void Normal::operator=(Normal n) {
  this->x = n.x;
  this->y = n.y;
  this->z = n.z;
}
