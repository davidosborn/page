#ifndef    page_local_phys_attrib_Size_hpp
#   define page_local_phys_attrib_Size_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Size
			{
				// construct
				explicit Size(float = 1);

				// access
				float GetSize() const;
				void SetSize(float);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float size;
			};
		}
	}
}

#endif
