#ifndef _VIDEOMATERIAL_HH
#define _VIDEOMATERIAL_HH

#include <mutex>
#include <vlc/vlc.h>
#include "common.hh"
#include "material.hh"

class VideoMaterial : public Material {
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
	VideoMaterial(const std::string &src);

	~VideoMaterial();

	int getWidth() const;

	int getHeight() const;

	bool hasAlpha() const;

	bool isDirty() const;

	void update();

	const uint8_t *getImage() const;
};

#endif /* _VIDEOMATERIAL_HH */
