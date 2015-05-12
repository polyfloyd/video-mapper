#include "shape.hh"

Shape::Vertex::Vertex(glm::vec3 *vec, float u, float v) {
	this->vec = vec;
	this->tex = glm::vec2(u, v);
}

Shape::Face::Face(const Vertex &a, const Vertex &b, const Vertex &c) {
	this->v[0] = a;
	this->v[1] = b;
	this->v[2] = c;
}
