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

#include "OpacityRange.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	OpacityRange::OpacityRange(float value) :
		min(value), max(value) {}

	OpacityRange::OpacityRange(float min, float max) :
		min(min), max(max) {}

	/*----------+
	| accessors |
	+----------*/

	float OpacityRange::GetMinOpacity() const
	{
		return min;
	}

	float OpacityRange::GetMaxOpacity() const
	{
		return max;
	}

	void OpacityRange::SetMinOpacity(float min)
	{
		this->min = min;
	}

	void OpacityRange::SetMaxOpacity(float max)
	{
		this->max = max;
	}

	void OpacityRange::SetOpacityRange(float value)
	{
		this->min = value;
		this->max = value;
	}

	void OpacityRange::SetOpacityRange(float min, float max)
	{
		this->min = min;
		this->max = max;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame OpacityRange::GetFrame() const
	{
		Frame frame;
		frame.opacityRange = Frame::Range<float>(min, max);
		return frame;
	}

	void OpacityRange::SetFrame(const Frame &frame)
	{
		if (frame.opacityRange)
		{
			min = frame.opacityRange->min;
			max = frame.opacityRange->max;
		}
	}
}}}
