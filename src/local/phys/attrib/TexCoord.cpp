#include "TexCoord.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	TexCoord::TexCoord(const math::Vec2 &value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	const math::Vec2 &TexCoord::GetTexCoord() const
	{
		return value;
	}

	void TexCoord::SetTexCoord(const math::Vec2 &value)
	{
		this->value = value;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame TexCoord::GetFrame() const
	{
		Frame frame;
		frame.texCoord = value;
		return frame;
	}

	void TexCoord::SetFrame(const Frame &frame)
	{
		if (frame.texCoord) value = *frame.texCoord;
	}
}}}
