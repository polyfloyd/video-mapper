#define GLM_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "common.hh"
#include "shape.hh"
#include "renderer.hh"

GLFWmonitor *selectMonitor(std::vector<GLFWmonitor*> monitors) {
	GLFWmonitor *mon = nullptr;
	while (!mon) {
		printf("Pick a monitor:\n");
		for (unsigned int i = 0; i < monitors.size(); i++) {
			printf("\t%i: %s\n", i + 1, glfwGetMonitorName(monitors[i]));
		}

		unsigned int pick = 0;
		scanf("%i", &pick);
		if (pick > 0 && pick <= monitors.size()) {
			mon = monitors[pick - 1];
		}
	}
	return mon;
}

int main(int argc, char **argv) {
	glm::mat4 model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10)) *
		glm::rotate(glm::mat4(1.0f), 32.0f, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0, 1, 0));
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 mvp = projection * view * model;

	Cube cube1(1.0f);
	Cube cube2(1.0f);
	auto texture = Material::Texture::fromImage("res/test.png");
	Material colors[6];
	for (int i = 0; i < 6; i++) {
		colors[i].color = glm::vec3(
			(i + 1) & 0x1 ? 1.0f : 0.0f,
			(i + 1) & 0x2 ? 1.0f : 0.0f,
			(i + 1) & 0x4 ? 1.0f : 0.0f
		);
		colors[i].tex = texture;
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

	OpenGLRenderer renderer(selectMonitor);

	while (renderer.isAlive()) {
		if (glfwGetKey(renderer.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
			if (!wireframeDebounce) {
				wireframe = !wireframe;
				glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
				wireframeDebounce = true;
			}
		}
		if (glfwGetKey(renderer.getWindow(), GLFW_KEY_W) == GLFW_RELEASE) {
			wireframeDebounce = false;
		}

		if (glfwGetMouseButton(renderer.getWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
			int windowWidth, windowHeight;
			double cursorPosX, cursorPosY;
			glfwGetWindowSize(renderer.getWindow(), &windowWidth, &windowHeight);
			glfwGetCursorPos(renderer.getWindow(), &cursorPosX, &cursorPosY);
			glm::vec2 cursor(
				(cursorPosX / windowWidth  * 2 - 1) * renderer.getAspectRatio(),
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

		renderer.render(shapes);
	}
	debugf("Bye!");
}
