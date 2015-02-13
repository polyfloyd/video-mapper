#include "material.hh"
#include <CImg.h>

class CImgTexture : public Material::Texture {
public:
	cimg_library::CImg<float> img;

public:
	CImgTexture(const std::string &src) : img(src.c_str()) {};

	int getWidth() const {
		return img.width();
	};

	int getHeight() const {
		return img.height();
	};

	const float *getImage() const {
		return this->img.data();
	};
};


std::shared_ptr<Material::Texture> Material::Texture::fromImage(const std::string &src) {
	return std::shared_ptr<Material::Texture>(new CImgTexture(src));
}
