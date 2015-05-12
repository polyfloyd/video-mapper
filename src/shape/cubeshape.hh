#ifndef _CUBESHAPE_HH
#define _CUBESHAPE_HH

#include "shape.hh"

class CubeShape : public Shape {
private:
	std::vector<glm::vec3> vertices;
	std::vector<Face>      surfaces;

public:
	CubeShape();

	CubeShape(const CubeShape &cube) = default;

	std::vector<glm::vec3> *getVertices() {
		return &this->vertices;
	}

	std::vector<Shape::Face> *getFaces() {
		return &this->surfaces;
	}
};

#endif /* _CUBESHAPE_HH */
