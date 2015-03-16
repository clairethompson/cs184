#include "point.h"
#include "vector.h"
#include "color.h"
#include "light.h"

/* Ambient Light */
Light::Light(float r, float g, float b) {
	this->intensity = Color(r, g, b);
	this->falloff = 0.0;
	this->type = 0;
}

/* Point Light */
Light::Light(float x, float y, float z, float r, float g, float b, float falloff) {
	this->p = Point(x, y, z);
	this->intensity = Color(r, g, b);
	this->falloff = falloff;
	this->type = -1;

}

/* Directional Light */
Light::Light(float x, float y, float z, float r, float g, float b) {
	this->p = Point(0.0 - x, 0.0 - y, 0.0 - z);
	this->intensity = Color(r, g, b);
	this->falloff = 0.0;
	this->type = -2;
}

/* Getter Functions */
Color Light::getIntensity() { return this->intensity; }
Point Light::getPoint() { return this->p; }
float Light::getFalloff() { return this->falloff; }
int Light::getType() { return this->type; }

