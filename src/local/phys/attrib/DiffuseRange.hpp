#ifndef    page_local_phys_attrib_DiffuseRange_hpp
#   define page_local_phys_attrib_DiffuseRange_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class DiffuseRange
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit DiffuseRange(const math::RgbColor<> & = 1);

		DiffuseRange(
			const math::RgbColor<> &min,
			const math::RgbColor<> &max);

		/*----------+
		| accessors |
		+----------*/

		const math::RgbColor<> &GetMinDiffuse() const;
		const math::RgbColor<> &GetMaxDiffuse() const;
		void SetMinDiffuse(const math::RgbColor<> &);
		void SetMaxDiffuse(const math::RgbColor<> &);
		void SetDiffuseRange(const math::RgbColor<> &);
		void SetDiffuseRange(
			const math::RgbColor<> &min,
			const math::RgbColor<> &max);

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const;
		void SetFrame(const Frame &);

		/*-------------+
		| data members |
		+-------------*/

		private:
		math::RgbColor<> min, max;
	};
}}}

#endif
