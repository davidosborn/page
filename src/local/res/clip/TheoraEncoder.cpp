#include <cstdlib> // {,s}rand
#include <ctime> // time

#include "../../err/Exception.hpp"
#include "../../math/algorithm.hpp" // RgbToYcbcr420
#include "../../util/gcc/init_priority.hpp" // REG_INIT_PRIORITY
#include "TheoraEncoder.hpp"

namespace page { namespace res { namespace clip
{
	/*-------------+
	| constructors |
	+-------------*/

	TheoraEncoder::TheoraEncoder(const Callback &cb, const math::Vec2u &size, float frameRate, float quality) :
		Encoder(cb, Content(size) * 3),
		size(size), size16((size + 15) & ~0xfu), offset((size16 - size) / 2)
	{
		// initialize ogg stream
		std::srand(std::time(0));
		if (ogg_stream_init(&os, std::rand()))
			THROW((err::Exception<err::ClipModuleTag, err::OggPlatformTag>("failed to initialize stream")))

		try
		{
			// initialize Theora encoder
			theora_info ti;
			theora_info_init(&ti);
			ti.width                        = size16.x;
			ti.height                       = size16.y;
			ti.frame_width                  = size.x;
			ti.frame_height                 = size.y;
			ti.offset_x                     = offset.x;
			ti.offset_y                     = offset.y;
			ti.fps_numerator                = frameRate * 10000;
			ti.fps_denominator              = 10000;
			ti.aspect_numerator             = 1;
			ti.aspect_denominator           = 1;
			ti.colorspace                   = OC_CS_UNSPECIFIED;
			ti.target_bitrate               = 45000;
			ti.quality                      = quality * 63;
			ti.quick_p                      = 1;
			ti.keyframe_auto_p              = 1;
			ti.keyframe_frequency           = 64;
			ti.keyframe_frequency_force     = 64;
			ti.keyframe_data_target_bitrate = 67500;
			ti.keyframe_auto_threshold      = 80;
			ti.keyframe_mindistance         = 8;
			ti.noise_sensitivity            = 1;
			ti.pixelformat                  = OC_PF_420;
			if (theora_encode_init(&ts, &ti))
			{
				theora_info_clear(&ti);
				THROW((err::Exception<err::ClipModuleTag, err::TheoraPlatformTag>("failed to initialize encoder")))
			}
			theora_info_clear(&ti);

			try
			{
				// write header
				ogg_packet op;
				theora_encode_header(&ts, &op);
				ogg_stream_packetin(&os, &op);
				for (ogg_page og; ogg_stream_pageout(&os, &og);)
				{
					Return(og.header, og.header_len);
					Return(og.body, og.body_len);
				}

				// write comments
				theora_comment tc;
				theora_comment_init(&tc);
				// FIXME: add comments
				theora_encode_comment(&tc, &op);
				ogg_stream_packetin(&os, &op);
				// HACK: theora_encode_comment allocates; we have to free
				// see libtheora's encoder_example.c
				free(op.packet);

				// write codebooks
				theora_encode_tables(&ts, &op);
				ogg_stream_packetin(&os, &op);

				// flush remaining headers
				for (ogg_page og; ogg_stream_flush(&os, &og);)
				{
					Return(og.header, og.header_len);
					Return(og.body, og.body_len);
				}
			}
			catch (...)
			{
				theora_clear(&ts);
				throw;
			}
		}
		catch (...)
		{
			ogg_stream_destroy(&os);
			throw;
		}
	}

	TheoraEncoder::~TheoraEncoder()
	{
		if (!tbData.empty()) EncodeBuffer(true);
		theora_clear(&ts);
		ogg_stream_destroy(&os);
	}

	/*---------+
	| encoding |
	+---------*/

	void TheoraEncoder::Encode(const void *s)
	{
		if (tbData.empty())
		{
			// initialize Y'CbCr buffer
			tb.y_width   = size16.x;
			tb.y_height  = size16.y;
			tb.y_stride  = tb.y_width;
			tb.uv_width  = tb.y_width  / 2;
			tb.uv_height = tb.y_height / 2;
			tb.uv_stride = tb.uv_width;

			// initialize Y'CbCr buffer data
			unsigned
				lumaSize = tb.y_stride * tb.y_height,
				chromaSize = tb.uv_stride * tb.uv_height;
			tbData.assign(lumaSize, 16);
			tbData.insert(tbData.end(), chromaSize * 2, 128);
			tb.y = &*tbData.begin();
			tb.u = tb.y + lumaSize;
			tb.v = tb.u + chromaSize;
		}
		else EncodeBuffer();

		// convert RGB input to Y'CbCr 4:2:0
		math::Vec2u
			halfSize16(size16 / 2),
			halfOffset(offset / 2);
		math::RgbToYcbcr420(
			static_cast<const unsigned char *>(s),
			tb.y + offset.y * size16.x + offset.x,
			tb.u + halfOffset.y * halfSize16.x + halfOffset.x,
			tb.v + halfOffset.y * halfSize16.x + halfOffset.x,
			size, size.x * 3, size16.x);
	}

	void TheoraEncoder::EncodeBuffer(bool lastFrame)
	{
		theora_encode_YUVin(&ts, &tb);
		ogg_packet op;
		theora_encode_packetout(&ts, lastFrame, &op);
		ogg_stream_packetin(&os, &op);
		for (ogg_page og; ogg_stream_pageout(&os, &og);)
		{
			Return(og.header, og.header_len);
			Return(og.body, og.body_len);
		}
	}

	/*-------------+
	| registration |
	+-------------*/

	namespace
	{
		/**
		 * A static initializer which registers @c TheoraEncoder with
		 * @c Encoder::Factory.
		 */
		struct Initializer
		{
			Initializer()
			{
				GLOBAL(Encoder::Factory).Register<TheoraEncoder>(
					50,
					Encoder::Factory::Criteria{
						{"theora"},
						{"video/ogg", "video/theora"},
						{"ogv", "ogx"}},
					Encoder::Factory::Data{"ogv"});
			}
		} initializer __attribute__((init_priority(REG_INIT_PRIORITY)));
	}
}}}
