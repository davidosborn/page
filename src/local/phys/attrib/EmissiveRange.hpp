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

#ifndef    page_phys_attrib_EmissiveRange_hpp
#   define page_phys_attrib_EmissiveRange_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct EmissiveRange
			{
				// construct
				explicit EmissiveRange(const math::RgbColor<> & = 0);
				EmissiveRange(
					const math::RgbColor<> &min,
					const math::RgbColor<> &max);

				// access
				const math::RgbColor<> &GetMinEmissive() const;
				const math::RgbColor<> &GetMaxEmissive() const;
				void SetMinEmissive(const math::RgbColor<> &);
				void SetMaxEmissive(const math::RgbColor<> &);
				void SetEmissiveRange(
					const math::RgbColor<> &min,
					const math::RgbColor<> &max);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				math::RgbColor<> min, max;
			};
		}
	}
}

#endif
