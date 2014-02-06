#include "SpecularRange.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	SpecularRange::SpecularRange(const math::RgbColor<> &value) :
		min(value), max(value) {}

	SpecularRange::SpecularRange(const math::RgbColor<> &min, const math::RgbColor<> &max) :
		min(min), max(max) {}

	/*----------+
	| accessors |
	+----------*/

	const math::RgbColor<> &SpecularRange::GetMinSpecular() const
	{
		return min;
	}

	const math::RgbColor<> &SpecularRange::GetMaxSpecular() const
	{
		return max;
	}

	void SpecularRange::SetMinSpecular(const math::RgbColor<> &min)
	{
		this->min = min;
	}

	void SpecularRange::SetMaxSpecular(const math::RgbColor<> &max)
	{
		this->max = max;
	}

	void SpecularRange::SetSpecularRange(const math::RgbColor<> &value)
	{
		this->min = value;
		this->max = value;
	}

	void SpecularRange::SetSpecularRange(const math::RgbColor<> &min, const math::RgbColor<> &max)
	{
		this->min = min;
		this->max = max;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame SpecularRange::GetFrame() const
	{
		Frame frame;
		frame.specularRange = Frame::Range<math::RgbColor<>>(min, max);
		return frame;
	}

	void SpecularRange::SetFrame(const Frame &frame)
	{
		if (frame.specularRange)
		{
			min = frame.specularRange->min;
			max = frame.specularRange->max;
		}
	}
}}}
