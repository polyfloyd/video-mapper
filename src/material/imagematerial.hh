#ifndef _IMAGEMATERIAL_HH
#define _IMAGEMATERIAL_HH

#include <CImg.h>
#include "common.hh"
#include "material.hh"

class ImageMaterial : public Material {
protected:
	std::unique_ptr<uint8_t> data;
	int width, height;
	bool alpha;

public:
	ImageMaterial(const std::string &path);

	int getWidth() const;

	int getHeight() const;

	bool hasAlpha() const;

	bool isDirty() const;

	void update() { };

	const uint8_t *getImage() const;
};

#endif /* _IMAGEMATERIAL_HH */
