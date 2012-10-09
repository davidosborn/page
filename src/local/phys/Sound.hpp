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

#ifndef    page_local_phys_Sound_hpp
#   define page_local_phys_Sound_hpp

#	include "../cache/fwd.hpp" // Proxy
#	include "../util/copy_ptr.hpp"
#	include "../util/Identifiable.hpp"
#	include "attrib/Position.hpp"
#	include "attrib/Volume.hpp"
#	include "mixin/Controllable.hpp"

namespace page
{
	namespace res { class Sound; }

	namespace phys
	{
		struct Sound : Controllable, util::Identifiable,
			attrib::Position,
			attrib::Volume
		{
			// construct
			explicit Sound(const cache::Proxy<res::Sound> &, bool loop = false);

			// state
			bool IsPlaying() const;
			bool IsLooping() const;
			float GetPlayPosition() const;

			// sound access
			const cache::Proxy<res::Sound> &GetSound() const;

			// update
			void Update(float deltaTime);

			protected:
			// frame serialization
			Frame GetFrame() const;
			void Update(const Frame &);

			private:
			util::copy_ptr<cache::Proxy<res::Sound>> sound;
			bool playing, loop;
			float playPosition;
		};
	}
}

#endif
