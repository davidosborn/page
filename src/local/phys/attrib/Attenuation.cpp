#include "Attenuation.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Attenuation::Attenuation(float value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	float Attenuation::GetAttenuation() const
	{
		return value;
	}

	void Attenuation::SetAttenuation(float value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Attenuation::GetFrame() const
	{
		Frame frame;
		frame.attenuation = value;
		return frame;
	}

	void Attenuation::SetFrame(const Frame &frame)
	{
		if (frame.attenuation) value = *frame.attenuation;
	}
}}}
