#ifndef _ICOSAHEDRONSHAPE_HH
#define _ICOSAHEDRONSHAPE_HH

#include "shape.hh"

class IcosahedronShape : public Shape {
private:
	std::vector<glm::vec3> vertices;
	std::vector<Face>      surfaces;

public:
	IcosahedronShape() : IcosahedronShape(0) { };

	IcosahedronShape(int tessLevel);

	IcosahedronShape(const IcosahedronShape &cube) = default;

	std::vector<glm::vec3> *getVertices() {
		return &this->vertices;
	}

	std::vector<Shape::Face> *getFaces() {
		return &this->surfaces;
	}

protected:
	/**
	 * Splits all polygons into 4 smaller polygons. Called by the constructor.
	 */
	void tessellate(int tessLevel, std::map<std::array<glm::vec3*, 2>, glm::vec3*> *existingVerts);

};

#endif /* _ICOSAHEDRONSHAPE_HH */
