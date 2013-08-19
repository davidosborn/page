#ifndef    page_local_phys_attrib_Exposure_hpp
#   define page_local_phys_attrib_Exposure_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Exposure
			{
				// construct
				explicit Exposure(float = 0);

				// access
				float GetExposure() const;
				void SetExposure(float);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float exposure;
			};
		}
	}
}

#endif
