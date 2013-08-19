#include <array>
#include <climits> // CHAR_BIT
#include <cstring> // strlen
#include <ctime> // localtime, time
#include <vector>

#include <png.h>

#include "../../../util/cpp.hpp" // STRINGIZE
#include "../../adapt/png.hpp" // WriteInfo
#include "../../type/Image.hpp"
#include "../register.hpp" // REGISTER_SAVER

namespace page
{
	namespace res
	{
		void SavePngImage(const Image &img, std::ostream &os)
		{
			// convert image to compatible format
			// FIXME: use closest compatible format
			Image scratchImg;
			const Image *compatImg = &img;
			Image::Channels channels;
			channels.reserve(3);
			channels.push_back(Image::Channel(Image::Channel::red, CHAR_BIT));
			channels.push_back(Image::Channel(Image::Channel::green, CHAR_BIT));
			channels.push_back(Image::Channel(Image::Channel::blue, CHAR_BIT));
			if (img.channels != channels)
			{
				scratchImg = Convert(*compatImg, channels, 1);
				compatImg = &scratchImg;
			}
			else if (!IsAligned(*compatImg, 1))
			{
				scratchImg = Align(*compatImg, 1);
				compatImg = &scratchImg;
			}
			// start writing png file
			WriteInfo wi(os);
			// set format header
			// FIXME: set format based on image conversion results
			png_set_IHDR(wi.png, wi.info,
				compatImg->size.x, compatImg->size.y,
				GetComponentDepth(compatImg->channels),
				PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
				PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
			// set modification time
			png_time pngTime;
			std::time_t time = std::time(0);
			png_convert_from_struct_tm(&pngTime, std::localtime(&time));
			png_set_tIME(wi.png, wi.info, &pngTime);
			// set text information
			std::array<png_text, 6> texts =
			{{
				// FIXME: use game information, such as the player's name
				// and their current location, to give a more detailed title
				// HACK: png_text requires mutable strings, but we can
				// assume they're not going to be modified
				{0, const_cast<png_charp>("Title"),         const_cast<png_charp>(STRINGIZE(Screenshot from NAME))},
				{0, const_cast<png_charp>("Author"),        const_cast<png_charp>(STRINGIZE(AUTHOR))},
				{0, const_cast<png_charp>("Description"),   const_cast<png_charp>(STRINGIZE(LONGNAME))},
				{0, const_cast<png_charp>("Copyright"),     const_cast<png_charp>(STRINGIZE(COPYRIGHT))},
				{0, const_cast<png_charp>("Creation Time"), const_cast<png_charp>(png_convert_to_rfc1123(wi.png, &pngTime))},
				{0, const_cast<png_charp>("Software"),      const_cast<png_charp>(STRINGIZE(NAME VERSION))}
			}};
			for (std::array<png_text, 2>::iterator text(texts.begin()); text != texts.end(); ++text)
			{
				text->compression = PNG_TEXT_COMPRESSION_NONE;
				text->text_length = std::strlen(text->text);
			}
			png_set_text(wi.png, wi.info, texts.begin(), texts.size());
			png_write_info(wi.png, wi.info);
			// generate row pointers
			std::vector<png_bytep> rows;
			rows.reserve(compatImg->size.y);
			unsigned pitch = (compatImg->size.x * GetDepth(compatImg->channels) + CHAR_BIT - 1) / CHAR_BIT;
			for (Image::Data::const_iterator row(compatImg->data.begin()); row != compatImg->data.end(); row += pitch)
				rows.push_back(reinterpret_cast<png_bytep>(const_cast<Image::Data::pointer>(&*row)));
			// write image data
			png_write_image(wi.png, &*rows.begin());
			png_write_end(wi.png, wi.info);
		}

		REGISTER_SAVER(Image, SavePngImage, "png", "png", 90)
	}
}
