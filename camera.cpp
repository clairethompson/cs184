#include "camera.h"

Camera::Camera(Point e, Point llp, Point lrp, Point ulp, Point urp) {
  this->eye = e;
  this->vp = ViewPlane(llp, lrp, ulp, urp);
}

Camera::Camera() {
  this->eye = Point();
  this->vp = ViewPlane(Point(), Point(), Point(), Point());
}

float Camera::getHeight() {
  return this->vp.getHeight();
}

float Camera::getWidth() {
  return this->vp.getWidth();
}

/* Return focal distance */
float Camera::getFocalLength() {
	return 0.0;
}

Point Camera::getEye() {
	return this->eye;
}

ViewPlane Camera::getViewPlane() {
	return this->vp;
}

Point Camera::getLL() {
  return this->vp.getLL();
}
Point Camera::getUL() {
  return this->vp.getUL();
}
Point Camera::getLR() {
  return this->vp.getLR();
}
Point Camera::getUR() {
  return this->vp.getUR();
}