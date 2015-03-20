#include "libraries.h"
#include <vector>
#include "triangle.h"
#include "point.h"
#include "material.h"

Libraries::Libraries() {
	this->materials = std::vector<Material>;
	this->vertices = std::vector<Point>;
	this->faces = std::vector<Triangle *>;
	this->normals = std::vector<Vector>
};