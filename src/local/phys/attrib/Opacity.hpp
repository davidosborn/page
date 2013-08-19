#ifndef    page_local_phys_attrib_Opacity_hpp
#   define page_local_phys_attrib_Opacity_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Opacity
			{
				// construct
				explicit Opacity(float = 1);

				// access
				float GetOpacity() const;
				void SetOpacity(float);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float opacity;
			};
		}
	}
}

#endif
