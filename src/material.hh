#ifndef _MATERIAL_HH
#define _MATERIAL_HH

#include <glm/glm.hpp>
#include "common.hh"

class Material {
public:
	class Texture {
	public:
		static std::shared_ptr<Material::Texture> fromImage(const std::string &src);

		virtual ~Texture() = default;

		virtual int getWidth() const = 0;

		virtual int getHeight() const = 0;

		virtual const float *getImage() const = 0;
	};

public:
	glm::vec3                color;
	glm::mat3                rotation;
	std::shared_ptr<Texture> tex = nullptr;
};

#endif /* _MATERIAL_HH */
