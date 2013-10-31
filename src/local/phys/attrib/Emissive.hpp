#ifndef    page_local_phys_attrib_Emissive_hpp
#   define page_local_phys_attrib_Emissive_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class Emissive
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Emissive(const math::RgbColor<> & = 0);

		/*----------+
		| accessors |
		+----------*/

		const math::RgbColor<> &GetEmissive() const;
		void SetEmissive(const math::RgbColor<> &);

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
		math::RgbColor<> value;
	};
}}}

#endif
