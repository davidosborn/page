#ifndef    page_local_phys_attrib_Attenuation_hpp
#   define page_local_phys_attrib_Attenuation_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class Attenuation
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Attenuation(float = 0);

		/*----------+
		| accessors |
		+----------*/

		float GetAttenuation() const;
		void SetAttenuation(float);

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
