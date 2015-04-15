#include "manualscene.hh"

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
