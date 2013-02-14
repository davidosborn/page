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

// C++
#include <cassert>
#include <cstdio> // remove
#include <functional> // bind
#include <iostream> // cout
#include <utility> // tie

// local
#include "../cfg/vars.hpp"
#include "../err/Exception.hpp"
#include "../res/type/Image.hpp"
#include "Encoder.hpp" // Encoder::{~Encoder,Write}
#include "encoder/EncoderFactory.hpp" // EncoderFactory, GetRegisteredEncoder
#include "Stream.hpp"

namespace page
{
	namespace clip
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		Stream::Stream(
			const boost::filesystem::path &path, 
			const std::string &format, 
			const math::Vec2u &size, 
			float frameRate, 
			float quality) :
				size(size)
		{
			auto product(EncoderFactory::GetGlobalInstance().Produce(path, format));

			// open file
			fs.open(product.path, std::ios_base::binary | std::ios_base::out);
			if (!fs)
				THROW((err::Exception<err::ClipModuleTag, err::FileAccessTag>()))

			// create encoder
			try
			{
				using namespace std::placeholders;
				encoder.reset(product.function(
					std::bind(&Stream::WriteEncoded, this, _1, _2),
					size, frameRate, quality));
				assert(encoder);
				std::cout << "opened clip stream to " << path << std::endl;
			}
			catch (...)
			{
				fs.close();
				std::remove(path);
				throw;
			}
		}
		
		Stream::~Stream()
		{
			// print statistics
			if (CVAR(logVerbose))
			{
				// FIXME: implement
			}
		}

		/*-----------+
		| attributes |
		+-----------*/

		const math::Vector<2, unsigned> &Stream::GetSize() const
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
	}
}
