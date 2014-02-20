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

#include "SpeedRange.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	SpeedRange::SpeedRange(float value) :
		min(value), max(value) {}

	SpeedRange::SpeedRange(float min, float max) :
		min(min), max(max) {}

	/*----------+
	| accessors |
	+----------*/

	float SpeedRange::GetMinSpeed() const
	{
		return min;
	}

	float SpeedRange::GetMaxSpeed() const
	{
		return max;
	}

	void SpeedRange::SetMinSpeed(float min)
	{
		this->min = min;
	}

	void SpeedRange::SetMaxSpeed(float max)
	{
		this->max = max;
	}

	void SpeedRange::SetSpeedRange(float value)
	{
		this->min = value;
		this->max = value;
	}

	void SpeedRange::SetSpeedRange(float min, float max)
	{
		this->min = min;
		this->max = max;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame SpeedRange::GetFrame() const
	{
		Frame frame;
		frame.speedRange = Frame::Range<float>(min, max);
		return frame;
	}

	void SpeedRange::SetFrame(const Frame &frame)
	{
		if (frame.speedRange)
		{
			min = frame.speedRange->min;
			max = frame.speedRange->max;
		}
	}
}}}
