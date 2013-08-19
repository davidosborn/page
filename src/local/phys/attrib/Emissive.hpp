#ifndef    page_local_phys_attrib_Emissive_hpp
#   define page_local_phys_attrib_Emissive_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Emissive
			{
				// construct
				explicit Emissive(const math::RgbColor<> & = 0);

				// access
				const math::RgbColor<> &GetEmissive() const;
				void SetEmissive(const math::RgbColor<> &);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				math::RgbColor<> emissive;
			};
		}
	}
}

#endif
