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

#include <ft2build.h>
#include FT_FREETYPE_H // FT_Done_Face, FT_Face, FT_Open_Face

#include <boost/lexical_cast.hpp>

#include "../../util/cpp.hpp" // STRINGIZE
#include "../adapt/freetype.hpp" // GetLib, OpenArgs
#include "../fmt/freetype/sub.hpp"
#include "../node/Node.hpp"
#include "../pipe/CatPipe.hpp"
#include "../pipe/MemPipe.hpp"
#include "Registry.hpp" // REGISTER_SCANNER

namespace page
{
	namespace res
	{
		bool ScanFreetype(const std::shared_ptr<const Pipe> &pipe, const ScanCallback &cb)
		{
			FT_Face face;
			OpenArgs args(*pipe);
			if (!FT_Open_Face(GetLib(), args.Get(), -1, &face))
			{
				for (unsigned i = 1; i < face->num_faces; ++i)
				{
					fmt::Header header = {i};
					std::shared_ptr<Pipe> headerPipe(new MemPipe(&header, sizeof header));
					cb(Node(std::shared_ptr<Pipe>(new CatPipe(headerPipe, pipe)),
						boost::lexical_cast<std::string>(i), fmt::mimeTypeForSub, "", false));
				}
				FT_Done_Face(face);
				return true;
			}
			return false;
		}

		REGISTER_SCANNER(
			STRINGIZE(FREETYPE_NAME),
			ScanFreetype,
			{"application/x-font-bdf", "application/x-font-opentype", "application/x-font-truetype"},
			{"bdf", "cff", "fnt", "fon", "otf", "pcf", "pfa", "pfb", "pfr", "ttc", "ttf"})
	}
}
