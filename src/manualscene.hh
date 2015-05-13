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
	glm::vec3      *selectedVertex = nullptr;
	glm::vec2       cursor;
	bool            dragging = false;
	bool            wireframe = false;

public:
	ManualScene(OpenGLRenderer *renderer);

	void addShape(std::unique_ptr<Shape> &shape);

	void update();

	void selectVertex();
};

#endif /* _MANUALSCENE_HH */
