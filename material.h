#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

class Material {
public:
	std::string name;
	float ka;
	float ks;
	float kd;
	float ns;
	float d;
	int illum;
	float ni;

	Material();

};
#endif