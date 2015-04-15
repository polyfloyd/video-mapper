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

const std::list<Shape*> Scene::getShapes() const {
	std::list<Shape*> s;
	for (auto &shape : this->shapes) {
		s.push_back(shape.get());
	}
	return s;
}
