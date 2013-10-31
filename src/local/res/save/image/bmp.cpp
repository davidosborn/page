#include <climits> // CHAR_BIT
#include <ostream>

#include "../../../err/Exception.hpp"
#include "../../../util/endian.hpp" // TransformEndian
#include "../../fmt/bmp.hpp"
#include "../../type/Image.hpp"
#include "../register.hpp" // REGISTER_SAVER

namespace page { namespace res
{
	namespace
	{
		inline void Write(std::ostream &os, const void *s, unsigned n)
		{
			if (!os.write(static_cast<const char *>(s), n))
				THROW((err::Exception<err::ResModuleTag, err::FileWriteTag>()))
		}
	}

	void SaveBmpImage(const Image &img, std::ostream &os)
	{
		// convert image to compatible format
		// FIXME: use closest compatible format
		Image scratchImg;
		const Image *compatImg = &img;
		Image::Channels channels;
		channels.reserve(3);
		channels.push_back(Image::Channel(Image::Channel::blue, CHAR_BIT));
		channels.push_back(Image::Channel(Image::Channel::green, CHAR_BIT));
		channels.push_back(Image::Channel(Image::Channel::red, CHAR_BIT));
		if (img.channels != channels)
		{
			scratchImg = Convert(*compatImg, channels, 4);
			compatImg = &scratchImg;
		}
		else if (!IsAligned(*compatImg, 4))
		{
			scratchImg = Align(*compatImg, 4);
			compatImg = &scratchImg;
		}
		// write bmp file
		BITMAPFILEHEADER bmfh =
		{
			fmt::type,
			sizeof bmfh + sizeof(BITMAPINFOHEADER) + compatImg->data.size(), 0, 0,
			sizeof bmfh + sizeof(BITMAPINFOHEADER)
		};
		util::TransformEndian(&bmfh, fmt::fileHeaderFormat, util::nativeEndian, util::littleEndian);
		Write(os, &bmfh, sizeof bmfh);
		BITMAPINFOHEADER bmih =
		{
			sizeof bmih,
			compatImg->size.x, -compatImg->size.y,
			1, static_cast<std::uint16_t>(GetDepth(compatImg->channels))
		};
		util::TransformEndian(&bmih, fmt::infoHeaderFormat, util::nativeEndian, util::littleEndian);
		Write(os, &bmih, sizeof bmih);
		Write(os, &*compatImg->data.begin(), compatImg->data.size());
	}

#if HOST_OS == win32
#	define SAVER_PRIORITY 80
#else
#	define SAVER_PRIORITY 20
#endif

	REGISTER_SAVER(Image, SaveBmpImage, "bmp", "bmp", SAVER_RANK)
}}
