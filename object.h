#ifndef OBJECT_H
#define OBJECT_H

#include "material.h"
#include "point.h"
#include "triangle.h"

#include <vector>
#include <string>

class Object {
public:
	string name;
	Material mtl;
	std::vector<point> vertices;
	std::vector<Triangle> faces;
	stf::vector<Vector> normals;
};

public:
	Object();

#endif