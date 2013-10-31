#include "DiffuseRange.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	DiffuseRange::DiffuseRange(const math::RgbColor<> &value) :
		min(value), max(value) {}

	DiffuseRange::DiffuseRange(const math::RgbColor<> &min, const math::RgbColor<> &max) :
		min(min), max(max) {}

	/*----------+
	| accessors |
	+----------*/

	const math::RgbColor<> &DiffuseRange::GetMinDiffuse() const
	{
		return min;
	}

	const math::RgbColor<> &DiffuseRange::GetMaxDiffuse() const
	{
		return max;
	}

	void DiffuseRange::SetMinDiffuse(const math::RgbColor<> &min)
	{
		this->min = min;
	}

	void DiffuseRange::SetMaxDiffuse(const math::RgbColor<> &max)
	{
		this->max = max;
	}

	void DiffuseRange::SetDiffuseRange(const math::RgbColor<> &value)
	{
		this->min = value;
		this->max = value;
	}

	void DiffuseRange::SetDiffuseRange(const math::RgbColor<> &min, const math::RgbColor<> &max)
	{
		this->min = min;
		this->max = max;
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame DiffuseRange::GetFrame() const
	{
		Frame frame;
		frame.diffuseRange = Frame::Range<math::RgbColor<>>(min, max);
		return frame;
	}

	void DiffuseRange::SetFrame(const Frame &frame)
	{
		if (frame.diffuseRange)
		{
			min = frame.diffuseRange->min;
			max = frame.diffuseRange->max;
		}
	}
}}}
