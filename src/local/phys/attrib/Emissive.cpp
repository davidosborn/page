#include "Emissive.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Emissive::Emissive(const math::RgbColor<> &value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	const math::RgbColor<> &Emissive::GetEmissive() const
	{
		return value;
	}

	void Emissive::SetEmissive(const math::RgbColor<> &value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Emissive::GetFrame() const
	{
		Frame frame;
		frame.emissive = value;
		return frame;
	}

	void Emissive::SetFrame(const Frame &frame)
	{
		if (frame.emissive) value = *frame.emissive;
	}
}}}
