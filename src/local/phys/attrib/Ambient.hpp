#ifndef    page_local_phys_attrib_Ambient_hpp
#   define page_local_phys_attrib_Ambient_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Ambient
			{
				// construct
				explicit Ambient(const math::RgbColor<> & = 1);

				// access
				const math::RgbColor<> &GetAmbient() const;
				void SetAmbient(const math::RgbColor<> &);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				math::RgbColor<> ambient;
			};
		}
	}
}

#endif
