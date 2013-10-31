#ifndef    page_local_phys_attrib_SpeedRange_hpp
#   define page_local_phys_attrib_SpeedRange_hpp

#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class SpeedRange
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit SpeedRange(float = 0);
		SpeedRange(float min, float max);

		/*----------+
		| accessors |
		+----------*/

		float GetMinSpeed() const;
		float GetMaxSpeed() const;
		void SetMinSpeed(float);
		void SetMaxSpeed(float);
		void SetSpeedRange(float);
		void SetSpeedRange(float min, float max);

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
		float min, max;
	};
}}}

#endif
