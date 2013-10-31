#ifndef    page_local_phys_attrib_Orientation_hpp
#   define page_local_phys_attrib_Orientation_hpp

#	include "../../math/Matrix.hpp"
#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"
#	include "../Frame.hpp"
#	include "../mixin/Transformable.hpp"

namespace page { namespace phys { namespace attrib
{
	class Orientation :
		public virtual Transformable
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Orientation(const math::Quat<> & = math::Quat<>());
		explicit Orientation(const math::Vec3 &normal);
		explicit Orientation(const math::Mat3 &);

		/*----------+
		| accessors |
		+----------*/

		const math::Quat<> &GetOrientation() const;
		void SetOrientation(const math::Quat<> &);

		// normal view
		math::Vec3 GetNormal() const;
		void SetNormal(const math::Vec3 &);

		// matrix view
		math::Mat3 GetMatrix() const;
		math::Mat3 GetInvMatrix() const;
		void SetMatrix(const math::Mat3 &);

		// transformation observers
		const math::Quat<> &GetLastOrientation() const;
		math::Vec3 GetLastNormal() const;
		const math::Quat<> &GetTorque() const;
		const math::Quat<> &GetSpin() const;

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const;
		void SetFrame(const Frame &);

		/*-----------------------------+
		| Transformable implementation |
		+-----------------------------*/

		public:
		void BakeTransform() override;
		void UpdateForce() override;
		void UpdateDelta() override;

		/*-------------+
		| data members |
		+-------------*/

		private:
		math::Quat<> value, lastValue = value;
		math::Quat<> torque, spin;
	};
}}}

#endif
