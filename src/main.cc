#define GLM_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "common.hh"
#include "object.hh"

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
	glOrtho(-ratio * 2, ratio * 2, -2, 2, 1.0f, 640.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glm::mat4 model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10)) *
		glm::rotate(glm::mat4(1.0f), 32.0f, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0, 1, 0));
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 mvp = projection * view * model;

	Cube cube(1.0f);
	Material colors[6];
	for (int i = 0; i < 6; i++) {
		colors[i].color = glm::vec3(
			(i + 1) & 0x1 ? 1.0f : 0.0f,
			(i + 1) & 0x2 ? 1.0f : 0.0f,
			(i + 1) & 0x4 ? 1.0f : 0.0f
		);
		cube.getFaces()[i * 2].mat = cube.getFaces()[i * 2 + 1].mat = &colors[i];
	}

	for (auto &vert : *cube.getVertices()) {
		vert = (mvp * glm::vec4(vert, 1)).xyz();
	}

	while (!glfwWindowShouldClose(window)) {
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		glViewport(0, 0, fbWidth, fbHeight);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		auto glVertexVec3 = [](const glm::vec3 &vec) {
			glVertex3f(vec.x, vec.y, vec.z);
		};
		for (auto &face : cube.getFaces()) {
			if (face.mat) {
				glColor3f(face.mat->color[0], face.mat->color[1], face.mat->color[2]);
			}
			glVertexVec3(*face.v[0]);
			glVertexVec3(*face.v[1]);
			glVertexVec3(*face.v[2]);
		}
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
