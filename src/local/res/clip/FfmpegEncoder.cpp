extern "C"
{
	#include <libavformat/avformat.h>
}

#include "../../err/Exception.hpp"
#include "../../math/Vector.hpp"
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

	namespace
	{
		/**
		 * A static initializer which registers @c FfmpegEncoder with
		 * @c Encoder::Factory.
		 */
		struct Initializer
		{
			Initializer()
			{
				// FIXME: may need extensions and names for all FFmpeg-supported formats
				GLOBAL(Encoder::Factory).Register<FfmpegEncoder>(
					0,
					Encoder::Factory::Criteria{{"ffmpeg"}},
					Encoder::Factory::Data{};
			}
		} initializer __attribute__((init_priority(REG_INIT_PRIORITY)));
	}
}}}
