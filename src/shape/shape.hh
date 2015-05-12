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
		glm::vec2  tex;

	public:
		Vertex() = default;

		Vertex(glm::vec3 *vec, float u, float v);
	};

	class Face {
	public:
		std::shared_ptr<Material> mat;
		Vertex                    v[3];

	public:
		Face(const Vertex &a, const Vertex &b, const Vertex &c);
	};

public:
	virtual std::vector<glm::vec3> *getVertices() = 0;

	virtual std::vector<Shape::Face> *getFaces() = 0;
};

#endif /* _SHAPE_HH */