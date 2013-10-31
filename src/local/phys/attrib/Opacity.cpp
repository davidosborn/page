#include "Opacity.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Opacity::Opacity(float value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	float Opacity::GetOpacity() const
	{
		return value;
	}

	void Opacity::SetOpacity(float value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Opacity::GetFrame() const
	{
		Frame frame;
		frame.opacity = value;
		return frame;
	}

	void Opacity::SetFrame(const Frame &frame)
	{
		if (frame.opacity) value = *frame.opacity;
	}
}}}
