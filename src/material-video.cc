#include "material.hh"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

class VideoTexture : public Material::Texture {
protected:
	int                streamIndex;
	bool               dirtyFlag;
	uint8_t           *buffer;
	struct SwsContext *swsContext;
	AVCodecContext    *codecContext  = NULL;
	AVFormatContext   *formatContext = NULL;
	AVFrame           *frame         = NULL;
	AVFrame           *frameRGB      = NULL;

public:
	VideoTexture(const std::string &src) {
		static bool avInitialized = false;
		if (!avInitialized) {
			avInitialized = true;
			av_register_all();
		}

		if (avformat_open_input(&this->formatContext, src.c_str(), NULL, NULL) != 0) {
			throw std::runtime_error("Could not open video file");
		}

		if (avformat_find_stream_info(this->formatContext, NULL) < 0) {
			throw std::runtime_error("Couldn't find stream information");
		}

		// Find the first video stream and grab the codec
		for (unsigned int i = 0; i < this->formatContext->nb_streams; i++) {
			if (this->formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
				this->streamIndex = i;
				break;
			}
		}
		this->codecContext = this->formatContext->streams[this->streamIndex]->codec;
		if (!this->codecContext) {
			throw std::runtime_error("Didn't find a video stream");
		}

		// Find the decoder for the video stream
		AVCodec *codec = avcodec_find_decoder(this->codecContext->codec_id);
		if (!codec) {
			throw std::runtime_error("Codec not found");
		}
		// Open codec
		AVDictionary *codecOptions = NULL;
		if (avcodec_open2(this->codecContext, codec, &codecOptions) < 0) {
			throw std::runtime_error("Could not open codec");
		}
		av_dict_free(&codecOptions);

		// Allocate AVFrame structures
		this->frame = avcodec_alloc_frame();
		if (!this->frame) {
			throw std::runtime_error("Could allocate frame");
		}
		this->frameRGB = avcodec_alloc_frame();
		if (!this->frameRGB) {
			throw std::runtime_error("Could allocate target frame");
		}

		// Determine required nuffer size and allocate buffer
		int bufferSize = avpicture_get_size(PIX_FMT_RGB24, this->codecContext->width, this->codecContext->height);
		this->buffer = (uint8_t*)av_malloc(bufferSize * sizeof(uint8_t));

		this->swsContext = sws_getContext(
			this->codecContext->width,
			this->codecContext->height,
			this->codecContext->pix_fmt,
			this->codecContext->width,
			this->codecContext->height,
			PIX_FMT_RGB24,
			SWS_BILINEAR,
			NULL,
			NULL,
			NULL
		);

		// Assign appropriate parts of buffer to image planes in frameRGB
		// Note that prameRGB is an AVFrame, but AVFrame is a superset
		// of AVPicture
		avpicture_fill((AVPicture*)this->frameRGB, this->buffer, PIX_FMT_RGB24, this->codecContext->width, this->codecContext->height);
	};

	~VideoTexture() {
		av_free(this->buffer);
		avcodec_free_frame(&this->frame);
		avcodec_free_frame(&this->frameRGB);
	}

	int getWidth() const {
		return this->codecContext->width;
	};

	int getHeight() const {
		return this->codecContext->height;
	};

	bool hasAlpha() const {
		return false;
	};

	void update() {
		this->dirtyFlag = true; // TODO: follow the framerate fo the videostream
		if (!this->dirtyFlag) {
			return;
		}

		AVPacket packet;
		while (av_read_frame(this->formatContext, &packet) >= 0) {
			if (packet.stream_index == this->streamIndex) {
				int finished;
				avcodec_decode_video2(this->codecContext, this->frame, &finished, &packet);
				if (finished) {
					// Convert the image from its native format to RGB
					sws_scale(
						this->swsContext,
						(uint8_t const* const*)this->frame->data,
						this->frame->linesize,
						0,
						this->codecContext->height,
						this->frameRGB->data,
						this->frameRGB->linesize
					);

					break;
				}
			}
			av_free_packet(&packet);
		}
	}

	bool isDirty() const {
		return this->dirtyFlag;
	}

	const uint8_t *getImage() const {
		return this->frameRGB->data[0];
	};
};


std::shared_ptr<Material::Texture> Material::Texture::fromVideo(const std::string &src) {
	return std::shared_ptr<Material::Texture>(new VideoTexture(src));
}
