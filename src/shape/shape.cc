#include "shape.hh"

Shape::Vertex::Vertex(glm::vec3 *vec, glm::vec2 tex) {
	assert(vec);

	this->vec = vec;
	this->tex = tex;
}

Shape::Face::Face(const Vertex &a, const Vertex &b, const Vertex &c) {
	this->v[0] = a;
	this->v[1] = b;
	this->v[2] = c;
}

Material *Shape::Face::getMaterial() const {
	return this->mat.get();
}

const glm::vec3 &Shape::Face::getVertex(int vertex) const {
	return *this->v[vertex].vec;
}

const glm::vec2 &Shape::Face::getTexCoord(int vertex) const {
	return this->v[vertex].tex;
}
