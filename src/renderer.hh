#ifndef _RENDERER_HH
#define _RENDERER_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shape.hh"

class Renderer {
public:
	virtual ~Renderer() = default;

	virtual void render(std::list<Shape*> shapes) = 0;

	virtual bool isAlive() const = 0;

	virtual float getAspectRatio() const = 0;
};

class OpenGLRenderer : public Renderer {
protected:
	GLFWwindow  *window;
	GLFWmonitor *monitor;

public:
	OpenGLRenderer(std::function<GLFWmonitor*(std::vector<GLFWmonitor*>)>);

	~OpenGLRenderer();

	void render(std::list<Shape*> shapes);

	bool isAlive() const;

	float getAspectRatio() const;

	GLFWwindow *getWindow() const {
		return this->window;
	}
};

#endif /* _RENDERER_HH */
