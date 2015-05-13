#include "scene.hh"

void Scene::addShape(Shape *shape) {
	std::unique_ptr<Shape> p(shape);
	this->addShape(p);
}

void Scene::removeShape(Shape *shape) {
	this->shapes.remove_if([shape](const std::unique_ptr<Shape> &s) {
		return shape == s.get();
	});
}

const std::list<Renderable*> Scene::getRenderables() const {
	std::list<Renderable*> s;
	for (auto &shape : this->shapes) {
		for (auto &face : *shape->getFaces()) {
			s.push_back(&face);
		}
	}
	return s;
}
