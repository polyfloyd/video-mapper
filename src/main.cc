#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "common.hh"
#include "manualscene.hh"
#include "material/material.hh"
#include "material/mediamaterial.hh"
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

	auto mat = std::shared_ptr<Material>(new MediaMaterial("res/test.png"));
	for (int i = 0; i < 6; i++) {
		cube1->getFaces()->at(i * 2).mat = cube1->getFaces()->at(i * 2 + 1).mat = mat;
		cube2->getFaces()->at(i * 2).mat = cube2->getFaces()->at(i * 2 + 1).mat = mat;
	}

	scene->addShape(cube1);
//	scene->addShape(cube2);
}

int main(int argc, char **argv) {
	OpenGLRenderer renderer(selectMonitor);
	ManualScene scene(&renderer);
	populateScene(&scene);

	while (renderer.isAlive()) {
		scene.update();
		renderer.render(&scene);
	}
	debugf("Bye!");
}
