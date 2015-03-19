#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

class Material {
public:
	string name;
	float ka;
	float ks;
	float ks;
	float ns;
	float d;
	int illum;
	float ni;

public:
	Material();

};
#endif