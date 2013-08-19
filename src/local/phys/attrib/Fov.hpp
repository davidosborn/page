#ifndef    page_local_phys_attrib_Fov_hpp
#   define page_local_phys_attrib_Fov_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Fov // field-of-view
			{
				// construct
				explicit Fov(float = 75);

				// access
				float GetFov() const;
				void SetFov(float);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float fov;
			};
		}
	}
}

#endif
