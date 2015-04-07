#include "object.h"
#include "point.h"
#include "triangle.h"
#include "material.h"
#include <string>
#include <vector>

Object::Object() {
	this->name = "";
	this->mtl = Material();
	this->vertices = std::vector<Point>();
	this->faces = std::vector<Triangle>();
	this->normals = std::vector<Vector>();
}