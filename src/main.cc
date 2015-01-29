#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "common.hh"

const float FOVY   = 32.0f; // Optoma WD188QZ
const float SIZE   = 1.0f;
const float DIST   = 4.2;
const float YANGLE = 31;

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
	glm::mat4 persp = glm::perspective(FOVY, (float)vidmode->width / (float)vidmode->height, 1.0f, 640.0f);
	glLoadMatrixf(glm::value_ptr(persp));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-SIZE / 2, 0, -SIZE / 2);
	glTranslatef(0, 0, -DIST);
	glRotatef(YANGLE, 1, 0, 0);
	glRotatef(45, 0, 1, 0);


	while (!glfwWindowShouldClose(window)) {
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		glViewport(0, 0, fbWidth, fbHeight);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glBegin(GL_QUADS);

		glColor3f(1, 0, 0);
		glVertex3f(0,    0,    0);
		glVertex3f(0,    SIZE, 0);
		glVertex3f(SIZE, SIZE, 0);
		glVertex3f(SIZE, 0,    0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0,    0);
		glVertex3f(0, 0,    SIZE);
		glVertex3f(0, SIZE, SIZE);
		glVertex3f(0, SIZE, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0,    0, 0);
		glVertex3f(0,    0, SIZE);
		glVertex3f(SIZE, 0, SIZE);
		glVertex3f(SIZE, 0, 0);

		glColor3f(1, 1, 0);
		glVertex3f(0,    0,    SIZE);
		glVertex3f(0,    SIZE, SIZE);
		glVertex3f(SIZE, SIZE, SIZE);
		glVertex3f(SIZE, 0,    SIZE);

		glColor3f(1, 0, 1);
		glVertex3f(SIZE, 0,    0);
		glVertex3f(SIZE, 0,    SIZE);
		glVertex3f(SIZE, SIZE, SIZE);
		glVertex3f(SIZE, SIZE, 0);

		glColor3f(0, 1, 1);
		glVertex3f(0,    SIZE, 0);
		glVertex3f(0,    SIZE, SIZE);
		glVertex3f(SIZE, SIZE, SIZE);
		glVertex3f(SIZE, SIZE, 0);

		glEnd();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
