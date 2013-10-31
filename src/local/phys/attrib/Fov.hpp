#ifndef    page_local_phys_attrib_Fov_hpp
#   define page_local_phys_attrib_Fov_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	/**
	 * The field-of-view for cameras, measured in degrees.
	 */
	class Fov
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Fov(float = 75);

		/*----------+
		| accessors |
		+----------*/

		float GetFov() const;
		void SetFov(float);

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
