#ifndef    page_local_phys_attrib_EmissiveRange_hpp
#   define page_local_phys_attrib_EmissiveRange_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct EmissiveRange
			{
				// construct
				explicit EmissiveRange(const math::RgbColor<> & = 0);
				EmissiveRange(
					const math::RgbColor<> &min,
					const math::RgbColor<> &max);

				// access
				const math::RgbColor<> &GetMinEmissive() const;
				const math::RgbColor<> &GetMaxEmissive() const;
				void SetMinEmissive(const math::RgbColor<> &);
				void SetMaxEmissive(const math::RgbColor<> &);
				void SetEmissiveRange(
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
