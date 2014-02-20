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

#ifndef    page_local_aud_openal_Channel_hpp
#   define page_local_aud_openal_Channel_hpp

#	include <memory> // shared_ptr

#	include <AL/al.h> // ALuint

#	include "../../cache/proxy/fwd.hpp" // Proxy
#	include "../Channel.hpp"

namespace page { namespace res { class Sound; }}

namespace page { namespace aud { namespace openal
{
	class Buffer;

	struct Channel : virtual aud::Channel
	{
		// construct/destroy
		Channel(const cache::Proxy<res::Sound> &, bool loop, float playPosition);
		~Channel();

		protected:
		// source access
		ALuint GetSource() const;

		private:
		// update
		void DoUpdate2();
		virtual void DoUpdate3() = 0;

		ALuint source;
		std::shared_ptr<Buffer> buffer;
	};
}}}

#endif
