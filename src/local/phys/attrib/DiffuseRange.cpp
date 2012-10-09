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

#include "DiffuseRange.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			DiffuseRange::DiffuseRange(const math::RgbColor<> &val) :
				min(val), max(val) {}
			DiffuseRange::DiffuseRange(const math::RgbColor<> &min, const math::RgbColor<> &max) :
				min(min), max(max) {}

			// access
			const math::RgbColor<> &DiffuseRange::GetMinDiffuse() const
			{
				return min;
			}
			const math::RgbColor<> &DiffuseRange::GetMaxDiffuse() const
			{
				return max;
			}
			void DiffuseRange::SetMinDiffuse(const math::RgbColor<> &min)
			{
				this->min = min;
			}
			void DiffuseRange::SetMaxDiffuse(const math::RgbColor<> &max)
			{
				this->max = max;
			}
			void DiffuseRange::SetDiffuseRange(const math::RgbColor<> &min, const math::RgbColor<> &max)
			{
				this->min = min;
				this->max = max;
			}

			// frame serialization
			Frame DiffuseRange::GetFrame() const
			{
				Frame frame;
				frame.diffuseRange = Frame::Range<math::RgbColor<>>(min, max);
				return frame;
			}
			void DiffuseRange::Update(const Frame &frame)
			{
				if (frame.diffuseRange)
				{
					min = frame.diffuseRange->min;
					max = frame.diffuseRange->max;
				}
			}
		}
	}
}
