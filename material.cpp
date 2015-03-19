#include "material.h"
#include <string>

Material::Material() {
	this->name = "";
	this->ka = Color();
	this->ks = Color()
	this->kd = Color();
	this->ns = 0.0;
	this->ni = 0.0;
	this->d = 0.0;
	this->illum = 0;	
}
