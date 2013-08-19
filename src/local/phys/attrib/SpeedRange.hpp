#ifndef    page_local_phys_attrib_SpeedRange_hpp
#   define page_local_phys_attrib_SpeedRange_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct SpeedRange
			{
				// construct
				explicit SpeedRange(float = 0);
				SpeedRange(float min, float max);

				// access
				float GetMinSpeed() const;
				float GetMaxSpeed() const;
				void SetMinSpeed(float);
				void SetMaxSpeed(float);
				void SetSpeedRange(float min, float max);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float min, max;
			};
		}
	}
}

#endif
