#include "Size.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Size::Size(float value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	float Size::GetSize() const
	{
		return value;
	}

	void Size::SetSize(float value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Size::GetFrame() const
	{
		Frame frame;
		frame.size = value;
		return frame;
	}

	void Size::SetFrame(const Frame &frame)
	{
		if (frame.size) value = *frame.size;
	}
}}}
