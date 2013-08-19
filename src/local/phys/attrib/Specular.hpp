#ifndef    page_local_phys_attrib_Specular_hpp
#   define page_local_phys_attrib_Specular_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Specular
			{
				// construct
				explicit Specular(const math::RgbColor<> & = 1);

				// access
				const math::RgbColor<> &GetSpecular() const;
				void SetSpecular(const math::RgbColor<> &);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				math::RgbColor<> specular;
			};
		}
	}
}

#endif
