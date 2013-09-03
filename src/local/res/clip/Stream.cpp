#include <cassert>
#include <functional> // bind
#include <iostream> // cout

#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../type/Image.hpp"
#include "Encoder.hpp"
#include "Stream.hpp"

namespace page { namespace res { namespace clip
{
	/*-------------+
	| constructors |
	+-------------*/

	Stream::Stream(
		const std::string &path,
		const std::string &format,
		const math::Vec2u &size,
		float frameRate,
		float quality) :
			size(size)
	{
		// select the closest-matching encoder
		std::string encoderPath(path);
		auto encoderBlueprint(GLOBAL(Encoder::Factory).SelectBest(encoderPath, format));

		// open file
		fs.open(encoderPath, std::ios_base::binary);
		if (!fs)
			THROW((err::Exception<err::ClipModuleTag, err::FileAccessTag>()))

		// create encoder
		try
		{
			using namespace std::placeholders;
			encoder = encoderBlueprint.function(
				std::bind(&Stream::WriteEncoded, this, _1, _2),
				size, frameRate, quality);
			assert(encoder);
			std::cout << "opened clip stream to " << encoderPath << std::endl;
		}
		catch (...)
		{
			fs.close();
			remove(encoderPath);
			throw;
		}
	}

	Stream::~Stream()
	{
		// print statistics
		if (*CVAR(logVerbose))
		{
			// FIXME: implement
		}
	}

	/*-----------+
	| attributes |
	+-----------*/

	const math::Vec2u &Stream::GetSize() const
	{
		return size;
	}

	/*------------------+
	| audio/video input |
	+------------------*/

	void Stream::Write(const res::Image &img)
	{
		// FIXME: crop to size
		// FIXME: convert to packed RGB8 format
		encoder->Write(&*img.data.begin(), img.data.size());
	}

	/*---------------+
	| encoded output |
	+---------------*/

	void Stream::WriteEncoded(const void *s, unsigned n)
	{
		if (!fs.write(static_cast<const char *>(s), n))
			THROW((err::Exception<err::ClipModuleTag, err::FileWriteTag>()))
	}
}}}
