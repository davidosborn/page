#include "Falloff.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Falloff::Falloff(float value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	float Falloff::GetFalloff() const
	{
		return value;
	}

	void Falloff::SetFalloff(float value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Falloff::GetFrame() const
	{
		Frame frame;
		frame.falloff = value;
		return frame;
	}

	void Falloff::SetFrame(const Frame &frame)
	{
		if (frame.falloff) value = *frame.falloff;
	}
}}}
