#ifndef    page_local_phys_attrib_Falloff_hpp
#   define page_local_phys_attrib_Falloff_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Falloff // exponent
			{
				// construct
				explicit Falloff(float = 1);

				// access
				float GetFalloff() const;
				void SetFalloff(float);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float falloff;
			};
		}
	}
}

#endif
