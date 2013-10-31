#include "Range.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Range::Range(float value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	float Range::GetRange() const
	{
		return value;
	}

	void Range::SetRange(float value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Range::GetFrame() const
	{
		Frame frame;
		frame.range = value;
		return frame;
	}

	void Range::SetFrame(const Frame &frame)
	{
		if (frame.range) value = *frame.range;
	}
}}}
