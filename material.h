#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include "color.h"

class Material {
public:
	std::string name;
	Color ka;
	Color ks;
	Color kd;
	float ns;
	float d;
	int illum;
	float ni;

public:
	Material();

};
#endif