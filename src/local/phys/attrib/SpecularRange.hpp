#ifndef    page_local_phys_attrib_SpecularRange_hpp
#   define page_local_phys_attrib_SpecularRange_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class SpecularRange
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit SpecularRange(const math::RgbColor<> & = 1);

		SpecularRange(
			const math::RgbColor<> &min,
			const math::RgbColor<> &max);

		/*----------+
		| accessors |
		+----------*/

		const math::RgbColor<> &GetMinSpecular() const;
		const math::RgbColor<> &GetMaxSpecular() const;
		void SetMinSpecular(const math::RgbColor<> &);
		void SetMaxSpecular(const math::RgbColor<> &);
		void SetSpecularRange(const math::RgbColor<> &);
		void SetSpecularRange(
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
