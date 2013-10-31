#ifndef    page_local_phys_attrib_AmbientRange_hpp
#   define page_local_phys_attrib_AmbientRange_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class AmbientRange
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit AmbientRange(const math::RgbColor<> & = 1);

		AmbientRange(
			const math::RgbColor<> &min,
			const math::RgbColor<> &max);

		/*----------+
		| accessors |
		+----------*/

		const math::RgbColor<> &GetMinAmbient() const;
		const math::RgbColor<> &GetMaxAmbient() const;
		void SetMinAmbient(const math::RgbColor<> &);
		void SetMaxAmbient(const math::RgbColor<> &);
		void SetAmbientRange(const math::RgbColor<> &);
		void SetAmbientRange(
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
