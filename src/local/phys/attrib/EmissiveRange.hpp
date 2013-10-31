#ifndef    page_local_phys_attrib_EmissiveRange_hpp
#   define page_local_phys_attrib_EmissiveRange_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class EmissiveRange
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit EmissiveRange(const math::RgbColor<> & = 0);

		EmissiveRange(
			const math::RgbColor<> &min,
			const math::RgbColor<> &max);

		/*----------+
		| accessors |
		+----------*/

		const math::RgbColor<> &GetMinEmissive() const;
		const math::RgbColor<> &GetMaxEmissive() const;
		void SetMinEmissive(const math::RgbColor<> &);
		void SetMaxEmissive(const math::RgbColor<> &);
		void SetEmissiveRange(const math::RgbColor<> &);
		void SetEmissiveRange(
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
