#include "cubeshape.hh"

CubeShape::CubeShape(float size) {
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
