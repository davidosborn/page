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
