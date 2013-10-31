#ifndef    page_local_phys_attrib_Ambient_hpp
#   define page_local_phys_attrib_Ambient_hpp

#	include "../../math/Color.hpp" // RgbColor
#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class Ambient
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Ambient(const math::RgbColor<> & = 1);

		/*----------+
		| accessors |
		+----------*/

		const math::RgbColor<> &GetAmbient() const;
		void SetAmbient(const math::RgbColor<> &);

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
