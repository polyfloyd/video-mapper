#ifndef _ICOSAHEDRONSHAPE_HH
#define _ICOSAHEDRONSHAPE_HH

#include "shape.hh"

class IcosahedronShape : public Shape {
private:
	std::vector<glm::vec3> vertices;
	std::vector<Face>      surfaces;

public:
	IcosahedronShape();

	IcosahedronShape(const IcosahedronShape &cube) = default;

	std::vector<glm::vec3> *getVertices() {
		return &this->vertices;
	}

	std::vector<Shape::Face> *getFaces() {
		return &this->surfaces;
	}
};

#endif /* _ICOSAHEDRONSHAPE_HH */
