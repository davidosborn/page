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
