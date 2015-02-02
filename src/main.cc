#define GLM_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "common.hh"
#include "object.hh"

const float ZFAR  = 640.0f;
const float ZNEAR = 1.0f;

void glVertexVec3(const glm::vec3 &vec) {
	glVertex3f(vec.x, vec.y, vec.z);
};

int main(int argc, char **argv) {
	glfwSetErrorCallback([](int code, const char *message) {
		fatalf("GLFW: %s (%i)", message, code);
	});
	if (!glfwInit()) {
		fatalf("Failed to initialize GLFW");
	}
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	int monitorCount;
	GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
	GLFWmonitor *monitor = nullptr;
	if (monitorCount == 1) {
		monitor = monitors[0];
	}
	while (!monitor) {
		printf("Pick a monitor:\n");
		for (int i = 0; i < monitorCount; i++) {
			printf("\t%i: %s\n", i + 1, glfwGetMonitorName(monitors[i]));
		}

		int pick = 0;
		scanf("%i", &pick);
		if (pick > 0 && pick <= monitorCount) {
			monitor = monitors[pick - 1];
		}
	}
	logf("Using monitor \"%s\"\n", glfwGetMonitorName(monitor));

	const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
	GLFWwindow *window = glfwCreateWindow(vidmode->width, vidmode->height, "Projection", monitor, nullptr);
	if (!window) {
		fatalf("Failed to open window");
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) {
		fatalf("Could not initialize GLEW");
	}

	if (GLEW_ARB_debug_output) {
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
		glDebugMessageCallbackARB([](GLenum src, GLenum type, GLuint id, GLenum sev, GLsizei len, const GLchar* msg, GLvoid* userp) {
			switch (sev) {
			case GL_DEBUG_SEVERITY_HIGH_ARB:
				fatalf("OpenGL: %s", msg);
				break;
			case GL_DEBUG_SEVERITY_MEDIUM_ARB:
				warnf("OpenGL: %s", msg);
				break;
			case GL_DEBUG_SEVERITY_LOW_ARB:
				debugf("OpenGL: %s", msg);
				break;
			}
		}, nullptr);
	} else {
		logf("GL_ARB_debug_output is not available");
	}

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	float ratio = (float)vidmode->width / (float)vidmode->height;
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1, 1, ZNEAR, ZFAR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glm::mat4 model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10)) *
		glm::rotate(glm::mat4(1.0f), 32.0f, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0, 1, 0));
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 mvp = projection * view * model;

	Cube cube1(1.0f);
	Cube cube2(1.0f);
	Material colors[6];
	for (int i = 0; i < 6; i++) {
		colors[i].color = glm::vec3(
			(i + 1) & 0x1 ? 1.0f : 0.0f,
			(i + 1) & 0x2 ? 1.0f : 0.0f,
			(i + 1) & 0x4 ? 1.0f : 0.0f
		);
		cube1.getFaces()[i * 2].mat = cube1.getFaces()[i * 2 + 1].mat = &colors[i];
		cube2.getFaces()[i * 2].mat = cube2.getFaces()[i * 2 + 1].mat = &colors[i];
	}

	std::list<Shape*> shapes;
	shapes.push_back(&cube1);
//	shapes.push_back(&cube2);

	for (auto &shape : shapes) {
		for (auto &vert : *shape->getVertices()) {
			vert = (mvp * glm::vec4(vert, 1)).xyz();
		}
	}

	glm::vec3 *draggedVertex = nullptr;
	bool wireframe         = false;
	bool wireframeDebounce = false;

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			if (!wireframeDebounce) {
				wireframe = !wireframe;
				glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
				wireframeDebounce = true;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
			wireframeDebounce = false;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
			int windowWidth, windowHeight;
			double cursorPosX, cursorPosY;
			glfwGetWindowSize(window, &windowWidth, &windowHeight);
			glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
			glm::vec2 cursor(
				(cursorPosX / windowWidth  * 2 - 1) * ratio,
				-(cursorPosY / windowHeight * 2 - 1)
			);
			if (!draggedVertex) {
				for (auto &shape : shapes) {
					for (auto &vert : *shape->getVertices()) {
						if (glm::distance(cursor, vert.xy()) < 0.1f) {
							draggedVertex = &vert;
							break;
						}
					}
				}
			} else {
				draggedVertex->x = cursor.x;
				draggedVertex->y = cursor.y;
			}
		} else {
			draggedVertex = nullptr;
		}

		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		glViewport(0, 0, fbWidth, fbHeight);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		for (auto &shape : shapes) {
			for (auto &face : shape->getFaces()) {
				if (face.mat) {
					glColor3f(face.mat->color[0], face.mat->color[1], face.mat->color[2]);
				}
				glVertexVec3(*face.v[0].vec);
				glVertexVec3(*face.v[1].vec);
				glVertexVec3(*face.v[2].vec);
			}
		}
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
