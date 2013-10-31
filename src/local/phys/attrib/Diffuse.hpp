#ifndef    page_local_phys_attrib_Diffuse_hpp
#   define page_local_phys_attrib_Diffuse_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class Diffuse
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Diffuse(const math::RgbColor<> & = 1);

		/*----------+
		| accessors |
		+----------*/

		const math::RgbColor<> &GetDiffuse() const;
		void SetDiffuse(const math::RgbColor<> &);

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
