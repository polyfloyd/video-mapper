#include "manualscene.hh"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

ManualScene::ManualScene(OpenGLRenderer *renderer) {
	this->renderer = renderer;

	this->renderer->addMouseButtonCallback([&](GLFWwindow *win, int button, int action, int mods){
		if (button != GLFW_MOUSE_BUTTON_1) {
			return;
		}

		this->dragging = action == GLFW_PRESS;
		if (action == GLFW_PRESS) {
			this->selectedVertex = nullptr;
		}

		if (this->dragging && !this->selectedVertex) {
			this->selectVertex();
		}
	});

	this->renderer->addMouseMoveCallback([&](GLFWwindow *win, double x, double y){
		this->cursor = glm::vec2(
			(x / this->renderer->getWindowSize().x  * 2 - 1) * this->renderer->getAspectRatio(),
			-(y / this->renderer->getWindowSize().y * 2 - 1)
		);

		if (this->dragging) {
			if (!this->selectedVertex) {
				this->selectVertex();
			}
			if (this->selectedVertex) {
				this->selectedVertex->x = this->cursor.x;
				this->selectedVertex->y = this->cursor.y;
			}
		}
	});
}

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

void ManualScene::selectVertex() {
	this->selectedVertex = [&]() {
		for (auto &shape : this->getShapes()) {
			for (auto &vert : *shape->getVertices()) {
				if (glm::distance(this->cursor, vert.xy()) < 0.1f) {
					return &vert;
				}
			}
		}
		return (glm::vec3*)nullptr;
	}();
}
