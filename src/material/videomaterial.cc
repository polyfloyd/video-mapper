#include "videomaterial.hh"

VideoMaterial::VideoMaterial(const std::string &src) {
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

	static const std::regex uriRegex("^([^:]+)://(.*)$", std::regex_constants::extended);
	std::smatch match;
	bool matches = std::regex_match(src, match, uriRegex);
	if (!matches || match[1] == "file") {
		std::string s = matches ? match[2].str() : src;
		this->media = libvlc_media_new_path(vlc, s.c_str());
	} else {
		this->media = libvlc_media_new_location(vlc, src.c_str());
	}

	if (!this->media) {
		fatalf("Unable create media from \"%s\"", src.c_str());
	}
	libvlc_media_add_option(this->media, "input-repeat=-1");
	this->mediaPlayer = libvlc_media_player_new_from_media(this->media);
	if (!this->mediaPlayer) {
		fatalf("Unable create media player from \"%s\"", src.c_str());
	}

	// Using libvlc_media_parse() and then getting the size using
	// libvlc_video_get_size() will fail with some inputs. This is a little
	// more complex but more reliable.
	auto sizeInitializer = [](const struct libvlc_event_t *event, void *userp) {
		VideoMaterial *self = static_cast<VideoMaterial*>(userp);
		unsigned w, h;
		if (libvlc_video_get_size(self->mediaPlayer, 0, &w, &h) == 0) {
			self->width  = w;
			self->height = h;
			self->bufferLock.unlock();
		}
	};
	// A condition variable would be more applicable. But this will suffice.
	this->bufferLock.lock();
	libvlc_event_manager_t *mediaEventMan = libvlc_media_event_manager(this->media);
	libvlc_event_attach(mediaEventMan, libvlc_MediaMetaChanged, sizeInitializer, this);

	libvlc_media_player_play(this->mediaPlayer);

	this->bufferLock.lock();
	this->bufferLock.unlock();
	libvlc_event_detach(mediaEventMan, libvlc_MediaMetaChanged, sizeInitializer, this);

	this->bufferSelect = 0;
	for (int i = 0; i < 2; i++) {
		this->buffers.push_back(std::unique_ptr<uint8_t[]>(new uint8_t[this->width * this->height * 4]));
	}
	libvlc_video_set_format(this->mediaPlayer, "RGBA", this->width, this->height, this->width * 4);

	auto lockCallback = [](void *data, void **pixels) {
		VideoMaterial *self = static_cast<VideoMaterial*>(data);
		int nextBuffer = (self->bufferSelect + 1) % self->buffers.size();
		*pixels = self->buffers[nextBuffer].get();
		return (void*)nullptr;
	};
	auto unlockCallback = [](void *data, void *id, void *const *pixels) { };
	auto displayCallback = [](void *data, void *id) {
		VideoMaterial *self = static_cast<VideoMaterial*>(data);
		self->shouldSwap = true;
	};
	libvlc_video_set_callbacks(this->mediaPlayer, lockCallback, unlockCallback, displayCallback, this);
};

VideoMaterial::~VideoMaterial() {
	libvlc_media_release(this->media);
	libvlc_media_player_release(this->mediaPlayer);
}

int VideoMaterial::getWidth() const {
	return this->width;
};

int VideoMaterial::getHeight() const {
	return this->height;
};

bool VideoMaterial::hasAlpha() const {
	return true;
};

bool VideoMaterial::isDirty() const {
	return this->dirty;
}

const uint8_t *VideoMaterial::getImage() const {
	return this->buffers[this->bufferSelect].get();
};

void VideoMaterial::update() {
	this->dirty = false;
	if (this->shouldSwap) {
		std::lock_guard<std::mutex> _(this->bufferLock);
		this->shouldSwap = false;
		this->dirty      = true;
		this->bufferSelect = (this->bufferSelect + 1) % this->buffers.size();
	}
}
