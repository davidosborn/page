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

#include "SpecularRange.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			SpecularRange::SpecularRange(const math::RgbColor<> &val) :
				min(val), max(val) {}
			SpecularRange::SpecularRange(const math::RgbColor<> &min, const math::RgbColor<> &max) :
				min(min), max(max) {}

			// access
			const math::RgbColor<> &SpecularRange::GetMinSpecular() const
			{
				return min;
			}
			const math::RgbColor<> &SpecularRange::GetMaxSpecular() const
			{
				return max;
			}
			void SpecularRange::SetMinSpecular(const math::RgbColor<> &min)
			{
				this->min = min;
			}
			void SpecularRange::SetMaxSpecular(const math::RgbColor<> &max)
			{
				this->max = max;
			}
			void SpecularRange::SetSpecularRange(const math::RgbColor<> &min, const math::RgbColor<> &max)
			{
				this->min = min;
				this->max = max;
			}

			// frame serialization
			Frame SpecularRange::GetFrame() const
			{
				Frame frame;
				frame.specularRange = Frame::Range<math::RgbColor<>>(min, max);
				return frame;
			}
			void SpecularRange::Update(const Frame &frame)
			{
				if (frame.specularRange)
				{
					min = frame.specularRange->min;
					max = frame.specularRange->max;
				}
			}
		}
	}
}
