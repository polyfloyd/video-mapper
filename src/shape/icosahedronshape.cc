#include "icosahedronshape.hh"
#include <glm/gtx/polar_coordinates.hpp>

glm::vec2 euclideanToUV(const glm::vec3 &vert) {
	auto p = glm::polar(vert);
	return glm::vec2(p.x/360 + 0.5, p.y/360 + 0.5);
}

IcosahedronShape::IcosahedronShape(int tessLevel) {
	// Allocate enough space to hold all vertices. A realloc will break all
	// pointers.
	int cap = 12;
	for (int i = 0; i < tessLevel; i++) {
		cap = cap * 4 - 6;
	}
	this->vertices.reserve(cap);

	float ico = float(1 + sqrt(5)) / 4;
	this->vertices = {
		glm::vec3(-0.5,  ico,  0.0),
		glm::vec3( 0.5,  ico,  0.0),
		glm::vec3(-0.5, -ico,  0.0),
		glm::vec3( 0.5, -ico,  0.0),
		glm::vec3( 0.0, -0.5,  ico),
		glm::vec3( 0.0,  0.5,  ico),
		glm::vec3( 0.0, -0.5, -ico),
		glm::vec3( 0.0,  0.5, -ico),
		glm::vec3( ico,  0.0, -0.5),
		glm::vec3( ico,  0.0,  0.5),
		glm::vec3(-ico,  0.0, -0.5),
		glm::vec3(-ico,  0.0,  0.5),
	};

	auto mkFace = [&](size_t a, size_t b, size_t c){
		return Shape::Face(
			Shape::Vertex(&this->vertices.at(a), euclideanToUV(this->vertices.at(a))),
			Shape::Vertex(&this->vertices.at(b), euclideanToUV(this->vertices.at(b))),
			Shape::Vertex(&this->vertices.at(c), euclideanToUV(this->vertices.at(c)))
		);
	};
	this->surfaces = {
		mkFace(0,  11, 5 ),
		mkFace(0,  5,  1 ),
		mkFace(0,  1,  7 ),
		mkFace(0,  7,  10),
		mkFace(0,  10, 11),
		mkFace(1,  5,  9 ),
		mkFace(5,  11, 4 ),
		mkFace(11, 10, 2 ),
		mkFace(10, 7,  6 ),
		mkFace(7,  1,  8 ),
		mkFace(3,  9,  4 ),
		mkFace(3,  4,  2 ),
		mkFace(3,  2,  6 ),
		mkFace(3,  6,  8 ),
		mkFace(3,  8,  9 ),
		mkFace(4,  9,  5 ),
		mkFace(2,  4,  11),
		mkFace(6,  2,  10),
		mkFace(8,  6,  7 ),
		mkFace(9,  8,  1 ),
	};

	std::map<std::array<glm::vec3*, 2>, glm::vec3*> existingVerts;
	tessellate(tessLevel, &existingVerts);
}

void IcosahedronShape::tessellate(int tessLevel, std::map<std::array<glm::vec3*, 2>, glm::vec3*> *existingVerts) {
	if (tessLevel < 1) {
		return;
	}

	std::vector<Face> newSurfaces;
	for (auto &face : this->surfaces) {
		Shape::Vertex newV[3];
		for (int i = 0; i < 3; i++) {
			glm::vec3 *a = face.v.at(i).vec;
			glm::vec3 *b = face.v.at((i+1) % 3).vec;

			glm::vec3 *v = (*existingVerts)[std::array<glm::vec3*, 2>({ { b,  a } })];
			if (!v) {
				this->vertices.push_back(glm::normalize(glm::vec3(
					a->x - (a->x - b->x) / 2,
					a->y - (a->y - b->y) / 2,
					a->z - (a->z - b->z) / 2
				)));
				v = (*existingVerts)[std::array<glm::vec3*, 2>({ { a,  b } })] = &this->vertices.at(this->vertices.size()-1);
			}

			newV[i] = Vertex(v, euclideanToUV(*v));
		}
		newSurfaces.push_back(Shape::Face(face.v[0], newV[0], newV[2]));
		newSurfaces.push_back(Shape::Face(face.v[1], newV[1], newV[0]));
		newSurfaces.push_back(Shape::Face(face.v[2], newV[2], newV[1]));
		newSurfaces.push_back(Shape::Face(newV[0], newV[1], newV[2]));
	}
	this->surfaces = newSurfaces;
	this->tessellate(tessLevel - 1, existingVerts);
}
