#ifndef    page_local_phys_attrib_DiffuseRange_hpp
#   define page_local_phys_attrib_DiffuseRange_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct DiffuseRange
			{
				// construct
				explicit DiffuseRange(const math::RgbColor<> & = 1);
				DiffuseRange(
					const math::RgbColor<> &min,
					const math::RgbColor<> &max);

				// access
				const math::RgbColor<> &GetMinDiffuse() const;
				const math::RgbColor<> &GetMaxDiffuse() const;
				void SetMinDiffuse(const math::RgbColor<> &);
				void SetMaxDiffuse(const math::RgbColor<> &);
				void SetDiffuseRange(
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
