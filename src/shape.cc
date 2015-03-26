#include "shape.hh"

Shape::Vertex::Vertex(glm::vec3 *vec) {
	this->vec = vec;
}

Shape::Vertex::Vertex(glm::vec3 *vec, float u, float v) {
	this->vec = vec;
	this->tex = glm::vec2(u, v);
}

Shape::Face::Face(Vertex a, Vertex b, Vertex c) {
	this->v[0] = a;
	this->v[1] = b;
	this->v[2] = c;
}

Shape::Face::Face(glm::vec3 *a, glm::vec3 *b, glm::vec3 *c) {
	this->v[0] = Vertex(a);
	this->v[1] = Vertex(b);
	this->v[2] = Vertex(c);
}

Cube::Cube(float size) {
	for (int i = 0; i < 8; i++) {
		this->vertices.push_back(glm::vec3(
			size * (i & 0x1 ? 1 : -1) / 2,
			size * (i & 0x2 ? 1 : -1) / 2,
			size * (i & 0x4 ? 1 : -1) / 2
		));
	}

	this->surfaces.push_back(Face(
		Vertex(&this->vertices[0b000], 1, 0),
		Vertex(&this->vertices[0b010], 0, 0),
		Vertex(&this->vertices[0b100], 1, 1)
	));
	this->surfaces.push_back(Face(
		Vertex(&this->vertices[0b010], 0, 0),
		Vertex(&this->vertices[0b100], 1, 1),
		Vertex(&this->vertices[0b110], 0, 1)
	));

	this->surfaces.push_back(Face(
		Vertex(&this->vertices[0b000], 0, 0),
		Vertex(&this->vertices[0b010], 0, 0),
		Vertex(&this->vertices[0b001], 0, 0)
	));
	this->surfaces.push_back(Face(
		Vertex(&this->vertices[0b010], 0, 0),
		Vertex(&this->vertices[0b001], 0, 0),
		Vertex(&this->vertices[0b011], 0, 0)
	));

	this->surfaces.push_back(Face(
		Vertex(&this->vertices[0b000], 0, 0),
		Vertex(&this->vertices[0b100], 0, 0),
		Vertex(&this->vertices[0b001], 0, 0)
	));
	this->surfaces.push_back(Face(
		Vertex(&this->vertices[0b100], 0, 0),
		Vertex(&this->vertices[0b001], 0, 0),
		Vertex(&this->vertices[0b101], 0, 0)
	));

	this->surfaces.push_back(Face(
		Vertex(&this->vertices[0b001], 0, 0),
		Vertex(&this->vertices[0b011], 0, 0),
		Vertex(&this->vertices[0b101], 0, 0)
	));
	this->surfaces.push_back(Face(
		Vertex(&this->vertices[0b011], 0, 0),
		Vertex(&this->vertices[0b101], 0, 0),
		Vertex(&this->vertices[0b111], 0, 0)
	));

	this->surfaces.push_back(Face(
		Vertex(&this->vertices[0b100], 1, 0),
		Vertex(&this->vertices[0b110], 0, 0),
		Vertex(&this->vertices[0b101], 1, 1)
	));
	this->surfaces.push_back(Face(
		Vertex(&this->vertices[0b110], 0, 0),
		Vertex(&this->vertices[0b101], 1, 1),
		Vertex(&this->vertices[0b111], 0, 1)
	));

	this->surfaces.push_back(Face(
		Vertex(&this->vertices[0b010], 1, 0),
		Vertex(&this->vertices[0b110], 1, 1),
		Vertex(&this->vertices[0b011], 0, 0)
	));
	this->surfaces.push_back(Face(
		Vertex(&this->vertices[0b110], 1, 1),
		Vertex(&this->vertices[0b011], 0, 0),
		Vertex(&this->vertices[0b111], 0, 1)
	));
}
