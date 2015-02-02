#include "material.hh"
#include <CImg.h>

class CImgTexture : public Material::Texture {
public:
	cimg_library::CImg<float> img;

public:
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


std::unique_ptr<Material::Texture> Material::Texture::load(std::string src) {
	std::unique_ptr<CImgTexture> tex(new CImgTexture);
	tex.get()->img = cimg_library::CImg<float>(src.c_str());
	return std::unique_ptr<Material::Texture>(std::move(tex));
}
