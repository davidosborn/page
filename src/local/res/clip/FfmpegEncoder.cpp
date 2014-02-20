/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
