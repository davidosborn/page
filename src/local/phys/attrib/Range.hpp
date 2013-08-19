#ifndef    page_local_phys_attrib_Range_hpp
#   define page_local_phys_attrib_Range_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Range
			{
				// construct
				explicit Range(float = 1);

				// access
				float GetRange() const;
				void SetRange(float);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float range;
			};
		}
	}
}

#endif
