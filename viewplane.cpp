#include "ViewPlane.h"
#include "vector.h"

ViewPlane::ViewPlane(Point ll, Point lr, Point ul, Point ur) {
  this->ll = ll;
  this->lr = lr;
  this->ul = ul;
  this->ur = ur;
  this->width = Vector(ur, ul).getLength();
  this->height = Vector(ur, lr).getLength();
  if (this->width < this->height) {
    float scale = 3000. / this->height;
    this->height = (this->height * scale);
    this->width = (this->width * scale);
    if (this->width < 1000) {
      // TODO: THROW ERROR;
    }
  } else if (this->height < this->width) {
    float scale = 3000. / this->width;
    this->width = (this->width * scale);
    this->height = (this->height * scale);
    if (this->height < 500) {
      // TODO: THROW ERROR;
    }
  }
};

ViewPlane::ViewPlane() {
  ViewPlane(Point(), Point(), Point(), Point());
};

float ViewPlane::getHeight() {
  return this->height;
}

float ViewPlane::getWidth() {
  return this->width;
}