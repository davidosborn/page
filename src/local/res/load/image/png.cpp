/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <cassert>
#include <climits> // CHAR_BIT
#include <memory> // {shared,unique}_ptr
#include <vector>

#include <png.h>

#include "../../../err/Exception.hpp"
#include "../../adapt/png.hpp" // ReadInfo
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp"
#include "../../type/Image.hpp"
#include "../Registry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		Image *LoadPngImage(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			png_byte sig[8];
			if (stream->ReadSome(sig, sizeof sig) != sizeof sig ||
				png_sig_cmp(sig, 0, sizeof sig)) return 0;
			ReadInfo ri(*stream);
			png_set_sig_bytes(ri.png, sizeof sig);
			png_read_info(ri.png, ri.info);
			// convert palette to RGB
			if (png_get_color_type(ri.png, ri.info) == PNG_COLOR_TYPE_PALETTE)
				png_set_palette_to_rgb(ri.png);
			// convert tRNS transparency to full alpha channel
			if (png_get_valid(ri.png, ri.info, PNG_INFO_tRNS))
				png_set_tRNS_to_alpha(ri.png);
			// finalize transformations
			png_read_update_info(ri.png, ri.info);
			// create image
			const std::unique_ptr<Image> img(new Image);
			// set image size
			if (!(img->size.x = png_get_image_width(ri.png, ri.info)) ||
				!(img->size.y = png_get_image_height(ri.png, ri.info)))
				THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag, err::FormatTag>("invalid image size")))
			// set image channels
			Image::Channels::size_type numChannels;
			if (!(numChannels = png_get_channels(ri.png, ri.info)))
				THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag, err::FormatTag>("image has no channels")))
			img->channels.resize(numChannels);
			unsigned depth = png_get_bit_depth(ri.png, ri.info);
			for (Image::Channels::iterator channel(img->channels.begin()); channel != img->channels.end(); ++channel)
				channel->depth = depth;
			switch (png_get_color_type(ri.png, ri.info))
			{
				case PNG_COLOR_TYPE_GRAY:
				if (img->channels.size() != 1)
					THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag, err::FormatTag>("channels do not match format")))
				img->channels[0].type = Image::Channel::mono;
				break;
				case PNG_COLOR_TYPE_GRAY_ALPHA:
				if (img->channels.size() != 2)
					THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag, err::FormatTag>("channels do not match format")))
				img->channels[0].type = Image::Channel::gray;
				img->channels[1].type = Image::Channel::alpha;
				break;
				case PNG_COLOR_TYPE_PALETTE:
				case PNG_COLOR_TYPE_RGB:
				if (img->channels.size() != 3)
					THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag, err::FormatTag>("channels do not match format")))
				img->channels[0].type = Image::Channel::red;
				img->channels[1].type = Image::Channel::green;
				img->channels[2].type = Image::Channel::blue;
				break;
				case PNG_COLOR_TYPE_RGB_ALPHA:
				if (img->channels.size() != 4)
					THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag, err::FormatTag>("channels do not match format")))
				img->channels[0].type = Image::Channel::red;
				img->channels[1].type = Image::Channel::green;
				img->channels[2].type = Image::Channel::blue;
				img->channels[3].type = Image::Channel::alpha;
				break;
				default: THROW((err::Exception<err::ResModuleTag, err::PngPlatformTag, err::FormatTag>("invalid color type")))
			}
			// set image alignment
			img->alignment = 1;
			// allocate data storage
			unsigned pitch = (img->size.x * img->channels.size() * depth + CHAR_BIT - 1) / CHAR_BIT;
			img->data.resize(img->size.y * pitch);
			// generate row pointers
			std::vector<png_bytep> rows;
			rows.reserve(img->size.y);
			for (Image::Data::iterator row(img->data.begin()); row != img->data.end(); row += pitch)
				rows.push_back(reinterpret_cast<png_bytep>(&*row));
			// read image data
			png_read_image(ri.png, &*rows.begin());
			png_read_end(ri.png, 0);
			return img.release();
		}

		LoadFunction GetPngImageLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			png_byte sig[8];
			return stream->ReadSome(sig, sizeof sig) == sizeof sig &&
				!png_sig_cmp(sig, 0, sizeof sig) ? LoadPngImage : LoadFunction();
		}

		REGISTER_LOADER(Image, GetPngImageLoader, "png", "image/png", "", true)
	}
}
