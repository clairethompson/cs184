#include "point.h"
#include "vector.h"
#include "color.h"
#include "light.h"

Light::Light(float r, float g, float b) {
	this->intensity = Color(r, g, b);
	this->falloff = 0.0;
}

Light::Light(float x, float y, float z, float r, float g, float b, float falloff) {
	this->p = Point(x, y, z);
	this->intensity = Color(r, g, b);
	this->falloff = falloff;
}

Light::Light(float x, float y, float z, float r, float g, float b) {
	this->p = Point(0.0 - x, 0.0 - y, 0.0 - z);
	this->intensity = Color(r, g, b);
	this->falloff = 0.0;
}

Color Light::getIntensity() { return this->intensity; }
Point Light::getPoint() { return this->p; }

