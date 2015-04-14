#include "material.hh"
#include <mutex>
#include <vlc/vlc.h>

class VideoTexture : public Material::Texture {
protected:
	std::vector<std::unique_ptr<uint8_t[]>> buffers;
	std::mutex                              bufferLock;
	int                                     bufferSelect;
	bool                                    shouldSwap;

	libvlc_media_t        *media;
	libvlc_media_player_t *mediaPlayer;

	bool dirty;
	int  width, height;

public:
	VideoTexture(const std::string &src) {
		static char const *vlcArgv[] = {
			"--intf", "dummy",
			"--no-audio",
			"--no-disable-screensaver",
			"--no-inhibit",
			"--no-snapshot-preview",
			"--no-stats",
			"--no-sub-autodetect-file",
			"--no-sub-autodetect-file",
			"--no-video-title-show",
			"--no-xlib",
		};

		static libvlc_instance_t *vlc = nullptr;
		if (!vlc) {
			vlc = libvlc_new(sizeof(vlcArgv) / sizeof(*vlcArgv), vlcArgv);
		}

		this->media       = libvlc_media_new_path(vlc, src.c_str());
		this->mediaPlayer = libvlc_media_player_new_from_media(this->media);
		libvlc_media_parse(this->media);
		libvlc_media_add_option(this->media, "input-repeat=-1");

		unsigned w, h;
		if (libvlc_video_get_size(this->mediaPlayer, 0, &w, &h) != 0) {
			fatalf("Video 0 does not exist");
		}
		this->width  = w;
		this->height = h;

		this->bufferSelect = 0;
		for (int i = 0; i < 2; i++) {
			this->buffers.push_back(std::unique_ptr<uint8_t[]>(new uint8_t[this->width * this->height * 4]));
		}
		libvlc_video_set_format(this->mediaPlayer, "RGBA", this->width, this->height, this->width * 4);

		auto lockCallback = [](void *data, void **pixels) {
			VideoTexture *self = static_cast<VideoTexture*>(data);
			int nextBuffer = (self->bufferSelect + 1) % self->buffers.size();
			*pixels = self->buffers[nextBuffer].get();
			return (void*)nullptr;
		};
		auto unlockCallback = [](void *data, void *id, void *const *pixels) { };
		auto displayCallback = [](void *data, void *id) {
			VideoTexture *self = static_cast<VideoTexture*>(data);
			self->shouldSwap = true;
		};
		libvlc_video_set_callbacks(this->mediaPlayer, lockCallback, unlockCallback, displayCallback, this);

		libvlc_media_player_play(this->mediaPlayer);
	};

	~VideoTexture() {
		libvlc_media_release(this->media);
		libvlc_media_player_release(this->mediaPlayer);
	}

	int getWidth() const {
		return this->width;
	};

	int getHeight() const {
		return this->height;
	};

	bool hasAlpha() const {
		return true;
	};

	bool isDirty() const {
		return this->dirty;
	}

	const uint8_t *getImage() const {
		return this->buffers[this->bufferSelect].get();
	};

	void update() {
		this->dirty = false;
		if (this->shouldSwap) {
			std::lock_guard<std::mutex> _(this->bufferLock);
			this->shouldSwap = false;
			this->dirty      = true;
			this->bufferSelect = (this->bufferSelect + 1) % this->buffers.size();
		}
	}
};


std::shared_ptr<Material::Texture> Material::Texture::fromVideo(const std::string &src) {
	return std::shared_ptr<Material::Texture>(new VideoTexture(src));
}
