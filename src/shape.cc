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

	for (int s = 0; s < 2; s++) {
		for (int b = 0; b < 3; b++) {
			Vertex verts[4];
			for (int u = 0; u < 2; u++) {
				for (int v = 0; v < 2; v++) {
					int corner = u<<1 | v;
					int i =
						(corner<<b & 0b111) |
						(corner<<b >> 3) |
						(s << (b+2) % 3);
					verts[corner] = Vertex(&this->vertices[i], -u, v);
				}
			}
			this->surfaces.push_back(Face(verts[0], verts[1], verts[2]));
			this->surfaces.push_back(Face(verts[1], verts[2], verts[3]));
		}
	}
}
