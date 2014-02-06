#include "SizeRange.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	SizeRange::SizeRange(float value) :
		min(value), max(value) {}

	SizeRange::SizeRange(float min, float max) :
		min(min), max(max) {}

	/*----------+
	| accessors |
	+----------*/

	float SizeRange::GetMinSize() const
	{
		return min;
	}

	float SizeRange::GetMaxSize() const
	{
		return max;
	}

	void SizeRange::SetMinSize(float min)
	{
		this->min = min;
	}

	void SizeRange::SetMaxSize(float max)
	{
		this->max = max;
	}

	void SizeRange::SetSizeRange(float value)
	{
		this->min = value;
		this->max = value;
	}

	void SizeRange::SetSizeRange(float min, float max)
	{
		this->min = min;
		this->max = max;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame SizeRange::GetFrame() const
	{
		Frame frame;
		frame.sizeRange = Frame::Range<float>(min, max);
		return frame;
	}

	void SizeRange::SetFrame(const Frame &frame)
	{
		if (frame.sizeRange)
		{
			min = frame.sizeRange->min;
			max = frame.sizeRange->max;
		}
	}
}}}
