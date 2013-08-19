#ifndef    page_local_phys_attrib_Diffuse_hpp
#   define page_local_phys_attrib_Diffuse_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct Diffuse
			{
				// construct
				explicit Diffuse(const math::RgbColor<> & = 1);

				// access
				const math::RgbColor<> &GetDiffuse() const;
				void SetDiffuse(const math::RgbColor<> &);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				math::RgbColor<> diffuse;
			};
		}
	}
}

#endif
