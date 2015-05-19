#ifndef _MANUALSCENE_HH
#define _MANUALSCENE_HH

#include "common.hh"
#include <glm/glm.hpp>
#include "scene.hh"
#include "renderer.hh"

class ManualScene : public Scene {
protected:
	OpenGLRenderer *renderer;
	Shape          *selectedShape = nullptr;
	glm::vec3      *selectedVertex = nullptr;
	glm::vec2       cursor;
	bool            dragging = false;
	bool            wireframe = false;

protected:
	void updateSelection();

public:
	ManualScene(OpenGLRenderer *renderer);

	void addShape(std::unique_ptr<Shape> &shape);

	void update();
};

#endif /* _MANUALSCENE_HH */
