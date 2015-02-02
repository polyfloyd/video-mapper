#include "object.hh"

Shape::Vertex::Vertex(glm::vec3 *vec) {
	this->vec = vec;
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
		&this->vertices[0b000],
		&this->vertices[0b010],
		&this->vertices[0b100]
	));
	this->surfaces.push_back(Face(
		&this->vertices[0b010],
		&this->vertices[0b100],
		&this->vertices[0b110]
	));

	this->surfaces.push_back(Face(
		&this->vertices[0b000],
		&this->vertices[0b010],
		&this->vertices[0b001]
	));
	this->surfaces.push_back(Face(
		&this->vertices[0b010],
		&this->vertices[0b001],
		&this->vertices[0b011]
	));

	this->surfaces.push_back(Face(
		&this->vertices[0b000],
		&this->vertices[0b100],
		&this->vertices[0b001]
	));
	this->surfaces.push_back(Face(
		&this->vertices[0b100],
		&this->vertices[0b001],
		&this->vertices[0b101]
	));

	this->surfaces.push_back(Face(
		&this->vertices[0b001],
		&this->vertices[0b011],
		&this->vertices[0b101]
	));
	this->surfaces.push_back(Face(
		&this->vertices[0b011],
		&this->vertices[0b101],
		&this->vertices[0b111]
	));

	this->surfaces.push_back(Face(
		&this->vertices[0b100],
		&this->vertices[0b110],
		&this->vertices[0b101]
	));
	this->surfaces.push_back(Face(
		&this->vertices[0b110],
		&this->vertices[0b101],
		&this->vertices[0b111]
	));

	this->surfaces.push_back(Face(
		&this->vertices[0b010],
		&this->vertices[0b110],
		&this->vertices[0b011]
	));
	this->surfaces.push_back(Face(
		&this->vertices[0b110],
		&this->vertices[0b011],
		&this->vertices[0b111]
	));
}
