#ifndef _MANUALSCENE_HH
#define _MANUALSCENE_HH

#define GLM_SWIZZLE

#include "common.hh"
#include <glm/glm.hpp>
#include "scene.hh"
#include "renderer.hh"

class ManualScene : public Scene {
protected:
	OpenGLRenderer *renderer;
	glm::vec3      *draggedVertex;

public:
	ManualScene(OpenGLRenderer *renderer)
		: renderer(renderer), draggedVertex(nullptr) {};

	void addShape(std::unique_ptr<Shape> &shape);

	void update();
};

#endif /* _MANUALSCENE_HH */
