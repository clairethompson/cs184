#include "localgeo.h"

LocalGeo::LocalGeo(Point p, Normal n) {
  this->p = p;
  this->n = n;
}

LocalGeo::LocalGeo() {
  LocalGeo(Point(), Normal());
}

void LocalGeo::setPoint(Point hitP) {
  this->p = hitP;
}

void LocalGeo::setNormal(Normal hitN) {
  this->n = hitN;
}

Point LocalGeo::getPoint() { return this->p; }
Normal LocalGeo::getNormal() { return this->n; }
