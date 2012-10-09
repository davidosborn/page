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

#ifndef    page_local_phys_Light_hpp
#   define page_local_phys_Light_hpp

#	include "attrib/Ambient.hpp"
#	include "attrib/Attenuation.hpp"
#	include "attrib/Cutoff.hpp"
#	include "attrib/Diffuse.hpp"
#	include "attrib/Falloff.hpp"
#	include "attrib/PositionOrientation.hpp"
#	include "attrib/Range.hpp"
#	include "attrib/Specular.hpp"
#	include "mixin/Controllable.hpp"

namespace page
{
	namespace phys
	{
		struct Light : Controllable,
			attrib::Ambient,
			attrib::Attenuation,
			attrib::Cutoff,
			attrib::Diffuse,
			attrib::Falloff,
			attrib::PositionOrientation,
			attrib::Range,
			attrib::Specular
		{
			// attenuation state
			float GetMinRange() const;
			float GetMaxRange() const;

			protected:
			// frame serialization
			Frame GetFrame() const;
			void Update(const Frame &);
		};
	}
}

#endif
