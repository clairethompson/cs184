#include "material.h"
#include <string>

Material::Material() {
	this->name = "";
	this->ka = 0.0;
	this->ks = 0.0;
	this->kd = 0.0;
	this->ns = 0.0;
	this->ni = 0.0;
	this->d = 0.0;
	this->illum = 0;	
}
