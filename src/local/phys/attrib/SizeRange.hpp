#ifndef    page_local_phys_attrib_SizeRange_hpp
#   define page_local_phys_attrib_SizeRange_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct SizeRange
			{
				// construct
				explicit SizeRange(float = 1);
				SizeRange(float min, float max);

				// access
				float GetMinSize() const;
				float GetMaxSize() const;
				void SetMinSize(float);
				void SetMaxSize(float);
				void SetSizeRange(float min, float max);

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
