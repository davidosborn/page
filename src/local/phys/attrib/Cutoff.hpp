#ifndef    page_local_phys_attrib_Cutoff_hpp
#   define page_local_phys_attrib_Cutoff_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	/**
	 * The cutoff angle for spotlights.
	 */
	class Cutoff
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Cutoff(float = 0);

		/*----------+
		| accessors |
		+----------*/

		float GetCutoff() const;
		void SetCutoff(float);

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
