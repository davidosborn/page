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

#include <memory> // unique_ptr

#include "../../util/cpp.hpp" // STRINGIZE
#include "../node/Node.hpp"
#include "../pipe/Pipe.hpp" // Pipe::Open
#include "../pipe/Stream.hpp"
#include "Registry.hpp" // REGISTER_SCANNER

namespace page
{
	namespace res
	{
		bool ScanOgg(const std::shared_ptr<const Pipe> &pipe, const ScanCallback &cb)
		{
			const std::unique_ptr<Stream> stream(pipe->Open());
			// FIXME: implement
			return false;
		}

		// FIXME: waiting for implementation
		/*REGISTER_SCANNER(
			STRINGIZE(OGG_NAME),
			ScanOgg,
			{"application/ogg", "audio/ogg", "video/ogg"},
			{"ogg"})*/
	}
}
