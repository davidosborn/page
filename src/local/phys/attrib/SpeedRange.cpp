#include "SpeedRange.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			SpeedRange::SpeedRange(float val) : min(val), max(val) {}
			SpeedRange::SpeedRange(float min, float max) : min(min), max(max) {}

			// access
			float SpeedRange::GetMinSpeed() const
			{
				return min;
			}
			float SpeedRange::GetMaxSpeed() const
			{
				return max;
			}
			void SpeedRange::SetMinSpeed(float min)
			{
				this->min = min;
			}
			void SpeedRange::SetMaxSpeed(float max)
			{
				this->max = max;
			}
			void SpeedRange::SetSpeedRange(float min, float max)
			{
				this->min = min;
				this->max = max;
			}

			// frame serialization
			Frame SpeedRange::GetFrame() const
			{
				Frame frame;
				frame.speedRange = Frame::Range<float>(min, max);
				return frame;
			}
			void SpeedRange::Update(const Frame &frame)
			{
				if (frame.speedRange)
				{
					min = frame.speedRange->min;
					max = frame.speedRange->max;
				}
			}
		}
	}
}
