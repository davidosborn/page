#include "Specular.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Specular::Specular(const math::RgbColor<> &value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	const math::RgbColor<> &Specular::GetSpecular() const
	{
		return value;
	}

	void Specular::SetSpecular(const math::RgbColor<> &value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Specular::GetFrame() const
	{
		Frame frame;
		frame.specular = value;
		return frame;
	}

	void Specular::SetFrame(const Frame &frame)
	{
		if (frame.specular) value = *frame.specular;
	}
}}}
