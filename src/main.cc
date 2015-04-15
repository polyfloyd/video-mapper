#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "common.hh"
#include "manualscene.hh"
#include "renderer.hh"
#include "scene.hh"
#include "shape.hh"

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

void populateScene(Scene *scene) {
	auto cube1 = std::unique_ptr<Shape>(new Cube(1.0f));
	auto cube2 = std::unique_ptr<Shape>(new Cube(1.0f));

	auto texture = Material::Texture::fromImage("res/test.png");
	for (int i = 0; i < 6; i++) {
		std::shared_ptr<Material> mat(new Material);
		mat->color = glm::vec3(
			(i + 1) & 0x1 ? 1.0f : 0.0f,
			(i + 1) & 0x2 ? 1.0f : 0.0f,
			(i + 1) & 0x4 ? 1.0f : 0.0f
		);
		mat->tex = texture;
		cube1->getFaces()[i * 2].mat = cube1->getFaces()[i * 2 + 1].mat = mat;
		cube2->getFaces()[i * 2].mat = cube2->getFaces()[i * 2 + 1].mat = mat;
	}

	scene->addShape(cube1);
//	scene->addShape(cube2);
}

int main(int argc, char **argv) {
	bool wireframe         = false;
	bool wireframeDebounce = false;

	OpenGLRenderer renderer(selectMonitor);
	ManualScene scene(&renderer);
	populateScene(&scene);

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

		scene.update();
		renderer.render(&scene);
	}
	debugf("Bye!");
}
