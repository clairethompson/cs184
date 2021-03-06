#ifndef LIBRARIES_H
#define LIBRARIES_H

#include <vector>

#include "triangle.h"
#include "point.h"
#include "material.h"
#include "vector.h"

class Libraries {
public:
	std::vector<Material> materials;
	std::vector<Point> vertices;
	std::vector<Triangle *> faces;
	std::vector<Vector> normals;

public:
	Libraries();

};

#endif
