#ifndef _RENDERER_HH
#define _RENDERER_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene.hh"

class Renderer {
public:
	virtual ~Renderer() = default;

	virtual void render(const Scene *scene) = 0;

	virtual bool isAlive() const = 0;

	virtual float getAspectRatio() const = 0;
};

class OpenGLRenderer : public Renderer {
protected:
	GLFWwindow  *window;
	GLFWmonitor *monitor;
	std::map<Material*, GLuint> textures;

	std::list<std::function<void(GLFWwindow*, int, int, int, int)>> keyCBs;
	std::list<std::function<void(GLFWwindow*, int, int, int)>>      mouseButtonCBs;
	std::list<std::function<void(GLFWwindow*, double, double)>>     mouseMoveCBs;

public:
	OpenGLRenderer(std::function<GLFWmonitor*(std::vector<GLFWmonitor*>)>);

	~OpenGLRenderer();

	void render(const Scene *scene);

	GLuint getCachedTexture(std::shared_ptr<Material> tex);

	bool isAlive() const;

	float getAspectRatio() const;

	GLFWwindow *getWindow() const {
		return this->window;
	}

	void addKeyCallback(std::function<void(GLFWwindow*, int, int, int, int)> cb) {
		this->keyCBs.push_back(cb);
	}

	void addMouseButtonCallback(std::function<void(GLFWwindow*, int, int, int)> cb) {
		this->mouseButtonCBs.push_back(cb);
	}

	void addMouseMoveCallback(std::function<void(GLFWwindow*, double, double)> cb) {
		this->mouseMoveCBs.push_back(cb);
	}
};

#endif /* _RENDERER_HH */
