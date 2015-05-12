#ifndef _MEDIAMATERIAL_HH
#define _MEDIAMATERIAL_HH

#include <mutex>
#include <vlc/vlc.h>
#include "common.hh"
#include "material.hh"

class MediaMaterial : public Material {
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
	MediaMaterial(const std::string &src);

	~MediaMaterial();

	int getWidth() const;

	int getHeight() const;

	bool hasAlpha() const;

	bool isDirty() const;

	void update();

	const uint8_t *getImage() const;
};

#endif /* _MEDIAMATERIAL_HH */
