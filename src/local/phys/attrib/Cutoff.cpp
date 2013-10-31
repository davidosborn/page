#include "Cutoff.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Cutoff::Cutoff(float value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	float Cutoff::GetCutoff() const
	{
		return value;
	}

	void Cutoff::SetCutoff(float value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Cutoff::GetFrame() const
	{
		Frame frame;
		frame.cutoff = value;
		return frame;
	}

	void Cutoff::SetFrame(const Frame &frame)
	{
		if (frame.cutoff) value = *frame.cutoff;
	}
}}}
