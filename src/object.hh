#ifndef _OBJECT_HH
#define _OBJECT_HH

#include "common.hh"
#include <glm/glm.hpp>

class Material {
public:
	glm::vec3 color;
};


class Shape {
public:
	class Face {
	public:
		Material  *mat  = nullptr;
		glm::vec3 *v[3] = { nullptr, nullptr, nullptr };

	public:
		Face(glm::vec3 *a, glm::vec3 *b, glm::vec3 *c);
	};

public:
	virtual std::vector<glm::vec3> *getVertices() = 0;

	virtual std::vector<Shape::Face> &getFaces() = 0;
};


class Cube : public Shape {
private:
	std::vector<glm::vec3> vertices;
	std::vector<Face>      surfaces;

public:
	Cube(float size);

	Cube(const Cube &cube) = default;

	std::vector<glm::vec3> *getVertices() {
		return &this->vertices;
	}

	std::vector<Shape::Face> &getFaces() {
		return this->surfaces;
	}
};

#endif /* _OBJECT_HH */
