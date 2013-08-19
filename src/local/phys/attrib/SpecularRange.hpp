#ifndef    page_local_phys_attrib_SpecularRange_hpp
#   define page_local_phys_attrib_SpecularRange_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct SpecularRange
			{
				// construct
				explicit SpecularRange(const math::RgbColor<> & = 1);
				SpecularRange(
					const math::RgbColor<> &min,
					const math::RgbColor<> &max);

				// access
				const math::RgbColor<> &GetMinSpecular() const;
				const math::RgbColor<> &GetMaxSpecular() const;
				void SetMinSpecular(const math::RgbColor<> &);
				void SetMaxSpecular(const math::RgbColor<> &);
				void SetSpecularRange(
					const math::RgbColor<> &min,
					const math::RgbColor<> &max);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				math::RgbColor<> min, max;
			};
		}
	}
}

#endif
