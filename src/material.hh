#ifndef _MATERIAL_HH
#define _MATERIAL_HH

#include <glm/glm.hpp>
#include "common.hh"

class Material {
public:
	glm::mat3 rotation;

	static std::shared_ptr<Material> fromImage(const std::string &src);
	static std::shared_ptr<Material> fromVideo(const std::string &src);

	virtual ~Material() = default;

	virtual int getWidth() const = 0;

	virtual int getHeight() const = 0;

	virtual bool hasAlpha() const = 0;

	virtual bool isDirty() const = 0;

	virtual void update() = 0;

	virtual const uint8_t *getImage() const = 0;
};

#endif /* _MATERIAL_HH */
