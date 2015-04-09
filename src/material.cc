#include "material.hh"
#include <CImg.h>

class CImgTexture : public Material::Texture {
public:
	std::unique_ptr<uint8_t> data;
	int width, height;
	bool alpha;

public:
	CImgTexture(const std::string &src) {
		cimg_library::CImg<uint8_t> img(src.c_str());

		this->alpha  = img.spectrum() > 3;
		this->width  = img.width();
		this->height = img.height();
		this->data  = std::unique_ptr<uint8_t>(new uint8_t[img.width() * img.height() * img.spectrum()]);

		for (int x = 0; x < this->getWidth(); x++) {
			for (int y = 0; y < this->getHeight(); y++) {
				for (int c = 0; c < img.spectrum(); c++) {
					int i = (x * this->getHeight() + y) * img.spectrum() + c;
					this->data.get()[i] = img.atXY(x, y, 0, c);
				}
			}
		}
	};

	int getWidth() const {
		return this->width;
	};

	int getHeight() const {
		return this->height;
	};

	bool hasAlpha() const {
		return this->alpha;
	};

	bool isDirty() const {
		return false;
	}

	void update() { }

	const uint8_t *getImage() const {
		return this->data.get();
	};
};


std::shared_ptr<Material::Texture> Material::Texture::fromImage(const std::string &src) {
	return std::shared_ptr<Material::Texture>(new CImgTexture(src));
}
