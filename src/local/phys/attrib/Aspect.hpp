#ifndef    page_local_phys_attrib_Aspect_hpp
#   define page_local_phys_attrib_Aspect_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Aspect
			{
				// construct
				explicit Aspect(float = 4.f / 3);

				// access
				float GetAspect() const;
				void SetAspect(float);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float aspect;
			};
		}
	}
}

#endif
