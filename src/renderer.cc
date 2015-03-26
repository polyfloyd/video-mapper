#include "renderer.hh"

const float ZFAR  = 640.0f;
const float ZNEAR = 1.0f;

void glNormalVec3(const glm::vec3 &vec) {
	glNormal3f(vec.x, vec.y, vec.z);
};

void glTexCoordVec2(const glm::vec2 &vec) {
	glTexCoord2f(vec.x, vec.y);
};

void glVertexVec3(const glm::vec3 &vec) {
	glVertex3f(vec.x, vec.y, vec.z);
};

OpenGLRenderer::OpenGLRenderer(std::function<GLFWmonitor*(std::vector<GLFWmonitor*>)> monSelector) {
	glfwSetErrorCallback([](int code, const char *message) {
		fatalf("GLFW: %s (%i)", message, code);
	});
	if (!glfwInit()) {
		fatalf("Failed to initialize GLFW");
	}
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	int monitorCount;
	GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
	if (monitorCount == 1) {
		monitor = monitors[0];
	} else {
		std::vector<GLFWmonitor*> monList;
		for (int i = 0; i < monitorCount; i++) {
			monList.push_back(monitors[i]);
		}
		this->monitor = monSelector(monList);
	}
	assert(this->monitor);

	debugf("Using monitor \"%s\"", glfwGetMonitorName(this->monitor));

	const GLFWvidmode *vidmode = glfwGetVideoMode(this->monitor);
	this->window = glfwCreateWindow(vidmode->width, vidmode->height, "Projection", this->monitor, nullptr);
	if (!this->window) {
		fatalf("Failed to open window");
	}
	glfwMakeContextCurrent(this->window);
	glfwSetWindowUserPointer(this->window, this);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) {
		fatalf("Could not initialize GLEW");
	}

	debugf("GLFW Version:        %s", glfwGetVersionString());
	debugf("GLEW Version:        %s", glewGetString(GLEW_VERSION));
	debugf("OpenGL Version:      %s", glGetString(GL_VERSION));
	debugf("OpenGL Vendor:       %s", glGetString(GL_VENDOR));
	debugf("OpenGL Renderer:     %s", glGetString(GL_RENDERER));
	debugf("OpenGL GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (GLEW_ARB_debug_output) {
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
		glDebugMessageCallbackARB([](GLenum src, GLenum type, GLuint id, GLenum sev, GLsizei len, const GLchar* cmsg, GLvoid* userp) {
			std::string msg(cmsg);
			if (msg[len - 1] == '\n') {
				msg = msg.substr(0, len - 1);
			}

			switch (sev) {
			case GL_DEBUG_SEVERITY_HIGH_ARB:
				fatalf("OpenGL: %s", msg.c_str());
				break;
			case GL_DEBUG_SEVERITY_MEDIUM_ARB:
				warnf("OpenGL: %s", msg.c_str());
				break;
			case GL_DEBUG_SEVERITY_LOW_ARB:
				debugf("OpenGL: %s", msg.c_str());
				break;
			}
		}, nullptr);
	} else {
		debugf("GL_ARB_debug_output is not available");
	}

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	float ratio = this->getAspectRatio();
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1, 1, ZNEAR, ZFAR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

OpenGLRenderer::~OpenGLRenderer() {
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

void OpenGLRenderer::render(const std::list<Shape*> shapes) {
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(this->window, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	for (auto &shape : shapes) {
		for (auto &face : shape->getFaces()) {
			if (face.mat) {
				glColor3f(face.mat->color[0], face.mat->color[1], face.mat->color[2]);
			}
			for (int i = 0; i < 3; i++) {
				glNormalVec3(face.v[i].normal);
				glTexCoordVec2(face.v[i].tex);
				glVertexVec3(*face.v[i].vec);
			}
		}
	}
	glEnd();

	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

bool OpenGLRenderer::isAlive() const {
	return !glfwWindowShouldClose(this->window);
}

float OpenGLRenderer::getAspectRatio() const {
	const GLFWvidmode *vidmode = glfwGetVideoMode(this->monitor);
	return vidmode->width / (float)vidmode->height;
}
