extern "C"
{
	#include <libavformat/avformat.h>
}

#include "../../err/Exception.hpp"
#include "../../math/Vector.hpp"
#include "../../util/cpp.hpp" // STRINGIZE
#include "../../util/gcc/init_priority.hpp" // REG_INIT_PRIORITY
#include "FfmpegEncoder.hpp"

namespace page { namespace res { namespace clip
{
	// construct/destroy
	FfmpegEncoder::FfmpegEncoder(const Callback &cb, const math::Vec2u &size, float frameRate, float quality) :
		Encoder(cb, Content(size) * 3)
	{
		av_register_all();
		// FIXME: implement
	}
	FfmpegEncoder::~FfmpegEncoder()
	{
		// FIXME: implement
	}

	// encoding
	void FfmpegEncoder::Encode(const void *s)
	{
		// FIXME: implement
	}
	void FfmpegEncoder::EncodeBuffer(bool lastFrame)
	{
		// FIXME: implement
	}

	/*-------------+
	| registration |
	+-------------*/

	// FIXME: consider adding names and extensions for all formats supported by FFmpeg
	REGISTER_ENCODER(FfmpegEncoder,
		STRINGIZE(FFMPEG_NAME),
		{"ffmpeg"},
		{"mpg"})
}}}
