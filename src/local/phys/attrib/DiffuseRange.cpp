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

#include "DiffuseRange.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	DiffuseRange::DiffuseRange(const math::RgbColor<> &value) :
		min(value), max(value) {}

	DiffuseRange::DiffuseRange(const math::RgbColor<> &min, const math::RgbColor<> &max) :
		min(min), max(max) {}

	/*----------+
	| accessors |
	+----------*/

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

	void DiffuseRange::SetDiffuseRange(const math::RgbColor<> &value)
	{
		this->min = value;
		this->max = value;
	}

	void DiffuseRange::SetDiffuseRange(const math::RgbColor<> &min, const math::RgbColor<> &max)
	{
		this->min = min;
		this->max = max;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame DiffuseRange::GetFrame() const
	{
		Frame frame;
		frame.diffuseRange = Frame::Range<math::RgbColor<>>(min, max);
		return frame;
	}

	void DiffuseRange::SetFrame(const Frame &frame)
	{
		if (frame.diffuseRange)
		{
			min = frame.diffuseRange->min;
			max = frame.diffuseRange->max;
		}
	}
}}}
