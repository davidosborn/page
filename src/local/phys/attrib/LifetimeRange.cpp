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

#include "LifetimeRange.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	LifetimeRange::LifetimeRange(float value) :
		min(value), max(value) {}

	LifetimeRange::LifetimeRange(float min, float max) :
		min(min), max(max) {}

	/*----------+
	| accessors |
	+----------*/

	float LifetimeRange::GetMinLifetime() const
	{
		return min;
	}

	float LifetimeRange::GetMaxLifetime() const
	{
		return max;
	}

	void LifetimeRange::SetMinLifetime(float min)
	{
		this->min = min;
	}

	void LifetimeRange::SetMaxLifetime(float max)
	{
		this->max = max;
	}

	void LifetimeRange::SetLifetimeRange(float value)
	{
		this->min = value;
		this->max = value;
	}

	void LifetimeRange::SetLifetimeRange(float min, float max)
	{
		this->min = min;
		this->max = max;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame LifetimeRange::GetFrame() const
	{
		Frame frame;
		frame.lifetimeRange = Frame::Range<float>(min, max);
		return frame;
	}

	void LifetimeRange::SetFrame(const Frame &frame)
	{
		if (frame.lifetimeRange)
		{
			min = frame.lifetimeRange->min;
			max = frame.lifetimeRange->max;
		}
	}
}}}
