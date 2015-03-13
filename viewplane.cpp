#include "ViewPlane.h"

ViewPlane::Viewplane(Camera c) {    
  this->width = c.ur - c.ul;
  this->height = c.ur - c.ul;
  if (this->width < this->height) {
    scale = 3000. / this->height;
    this->height = (this->height * scale);
    this->width = (this->width * scale);
    if (this->width < 1000) {
      // THROW ERROR;
      exit(0);
    }
  } else if (this->height < this->width) {
    scale = 3000. / this->width;
    this->width = (this->width * scale);
    this->height = t=(this->height * scale);
    if (this->height < 500) {
      // THROW ERROR;
      exit(0);
    }
  }
}