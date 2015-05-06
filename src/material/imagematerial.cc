#include "imagematerial.hh"

ImageMaterial::ImageMaterial(const std::string &src) {
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

int ImageMaterial::getWidth() const {
	return this->width;
};

int ImageMaterial::getHeight() const {
	return this->height;
};

bool ImageMaterial::hasAlpha() const {
	return this->alpha;
};

bool ImageMaterial::isDirty() const {
	return false;
}

const uint8_t *ImageMaterial::getImage() const {
	return this->data.get();
};
