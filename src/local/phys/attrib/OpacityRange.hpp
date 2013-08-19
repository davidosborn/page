#ifndef    page_local_phys_attrib_OpacityRange_hpp
#   define page_local_phys_attrib_OpacityRange_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct OpacityRange
			{
				// construct
				explicit OpacityRange(float = 1);
				OpacityRange(float min, float max);

				// access
				float GetMinOpacity() const;
				float GetMaxOpacity() const;
				void SetMinOpacity(float);
				void SetMaxOpacity(float);
				void SetOpacityRange(float min, float max);

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
