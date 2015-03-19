#ifndef OBJECT_H
#define OBJECT_H

#include "material.h"
#include "point.h"
#include "triangle.h"

#include <vector>
#include <string>

class Object {
public:
	std::string name;
	Material mtl;
	std::vector<Point> vertices;
	std::vector<Triangle> faces;
	std::vector<Vector> normals;
	Object();
};


#endif