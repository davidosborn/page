#include "DiffuseRange.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			DiffuseRange::DiffuseRange(const math::RgbColor<> &val) :
				min(val), max(val) {}
			DiffuseRange::DiffuseRange(const math::RgbColor<> &min, const math::RgbColor<> &max) :
				min(min), max(max) {}

			// access
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
			void DiffuseRange::SetDiffuseRange(const math::RgbColor<> &min, const math::RgbColor<> &max)
			{
				this->min = min;
				this->max = max;
			}

			// frame serialization
			Frame DiffuseRange::GetFrame() const
			{
				Frame frame;
				frame.diffuseRange = Frame::Range<math::RgbColor<>>(min, max);
				return frame;
			}
			void DiffuseRange::Update(const Frame &frame)
			{
				if (frame.diffuseRange)
				{
					min = frame.diffuseRange->min;
					max = frame.diffuseRange->max;
				}
			}
		}
	}
}
