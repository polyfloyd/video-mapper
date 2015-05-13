#ifndef _RENDERABLE_HH
#define _RENDERABLE_HH

class Renderable {
public:
	virtual Material *getMaterial() const = 0;

	virtual const glm::vec3 &getVertex(int vertex) const = 0;

	virtual const glm::vec2 &getTexCoord(int vertex) const = 0;
};

#endif /* _RENDERABLE_HH */
