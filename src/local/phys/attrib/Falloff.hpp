#ifndef    page_local_phys_attrib_Falloff_hpp
#   define page_local_phys_attrib_Falloff_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	/**
	 * The falloff exponent for spotlights.
	 */
	class Falloff
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Falloff(float = 1);

		/*----------+
		| accessors |
		+----------*/

		float GetFalloff() const;
		void SetFalloff(float);

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
		float value;
	};
}}}

#endif
