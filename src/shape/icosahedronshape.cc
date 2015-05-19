#include "icosahedronshape.hh"
#include <glm/gtx/polar_coordinates.hpp>

glm::vec2 euclideanToUV(const glm::vec3 &vert) {
	auto p = glm::polar(vert);
	return glm::vec2(p.x/360 + 0.5, p.y/360 + 0.5);
}

IcosahedronShape::IcosahedronShape() {
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
			Shape::Vertex(&this->vertices[a], euclideanToUV(this->vertices[a])),
			Shape::Vertex(&this->vertices[b], euclideanToUV(this->vertices[b])),
			Shape::Vertex(&this->vertices[c], euclideanToUV(this->vertices[c]))
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
}
