#ifndef _SHAPE_HH
#define _SHAPE_HH

#include <glm/glm.hpp>
#include "common.hh"
#include "material/material.hh"

class Shape {
public:
	class Vertex {
	public:
		glm::vec3 *vec;
		glm::vec3  normal;
		glm::vec2  tex;

	public:
		Vertex(glm::vec3 *vec);

		Vertex(glm::vec3 *vec, float u, float v);
	};

	class Face {
	public:
		std::shared_ptr<Material> mat;
		Vertex                    v[3] = { nullptr, nullptr, nullptr };

	public:
		Face(glm::vec3 *a, glm::vec3 *b, glm::vec3 *c);

		Face(Vertex a, Vertex b, Vertex c);
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

#endif /* _SHAPE_HH */
