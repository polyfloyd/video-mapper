#ifndef _SCENE_HH
#define _SCENE_HH

#include "common.hh"
#include "shape/shape.hh"

/**
 * An implementation of the Scene transforms its shapes to match real-world
 * objects.
 */
class Scene {
protected:
	std::list<std::unique_ptr<Shape>> shapes;

public:
	virtual void update() = 0;

	virtual const std::list<Renderable*> getRenderables() const;

	virtual void addShape(std::unique_ptr<Shape> &shape) {
		this->shapes.push_back(std::move(shape));
	}

	virtual void removeShape(Shape *shape);

	void addShape(Shape *shape);
};

#endif /* _SCENE_HH */
