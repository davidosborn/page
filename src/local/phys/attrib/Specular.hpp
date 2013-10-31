#ifndef    page_local_phys_attrib_Specular_hpp
#   define page_local_phys_attrib_Specular_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class Specular
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Specular(const math::RgbColor<> & = 1);

		/*----------+
		| accessors |
		+----------*/

		const math::RgbColor<> &GetSpecular() const;
		void SetSpecular(const math::RgbColor<> &);

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
