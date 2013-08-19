#ifndef    page_local_phys_attrib_Attenuation_hpp
#   define page_local_phys_attrib_Attenuation_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Attenuation
			{
				// construct
				explicit Attenuation(float = 0);

				// access
				float GetAttenuation() const;
				void SetAttenuation(float);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float attenuation;
			};
		}
	}
}

#endif
