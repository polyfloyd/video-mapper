#include "manualscene.hh"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void ManualScene::addShape(std::unique_ptr<Shape> &shape) {
	glm::mat4 model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10)) *
		glm::rotate(glm::mat4(1.0f), 32.0f, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(0, 1, 0));
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 mvp = projection * view * model;

	for (auto &vert : *shape->getVertices()) {
		vert = (mvp * glm::vec4(vert, 1)).xyz();
	}

	Scene::addShape(shape);
}

void ManualScene::update() {
	if (glfwGetMouseButton(this->renderer->getWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		int windowWidth, windowHeight;
		glfwGetWindowSize(this->renderer->getWindow(), &windowWidth, &windowHeight);
		double cursorPosX, cursorPosY;
		glfwGetCursorPos(this->renderer->getWindow(), &cursorPosX, &cursorPosY);
		glm::vec2 cursor(
			(cursorPosX / windowWidth  * 2 - 1) * renderer->getAspectRatio(),
			-(cursorPosY / windowHeight * 2 - 1)
		);
		if (!this->draggedVertex) {
			for (auto &shape : this->getShapes()) {
				for (auto &vert : *shape->getVertices()) {
					if (glm::distance(cursor, vert.xy()) < 0.1f) {
						this->draggedVertex = &vert;
						break;
					}
				}
			}
		}
		if (this->draggedVertex) {
			this->draggedVertex->x = cursor.x;
			this->draggedVertex->y = cursor.y;
		}
	} else {
		this->draggedVertex = nullptr;
	}
}
