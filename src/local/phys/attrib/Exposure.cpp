#include "Exposure.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Exposure::Exposure(float value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	float Exposure::GetExposure() const
	{
		return value;
	}

	void Exposure::SetExposure(float value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Exposure::GetFrame() const
	{
		Frame frame;
		frame.exposure = value;
		return frame;
	}

	void Exposure::SetFrame(const Frame &frame)
	{
		if (frame.exposure) value = *frame.exposure;
	}
}}}
