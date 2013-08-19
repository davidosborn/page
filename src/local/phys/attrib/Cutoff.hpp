#ifndef    page_local_phys_attrib_Cutoff_hpp
#   define page_local_phys_attrib_Cutoff_hpp

#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Cutoff // angle
			{
				// construct
				explicit Cutoff(float = 0);

				// access
				float GetCutoff() const;
				void SetCutoff(float);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				float cutoff;
			};
		}
	}
}

#endif
