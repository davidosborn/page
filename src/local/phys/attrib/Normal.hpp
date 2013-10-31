#ifndef    page_local_phys_attrib_Normal_hpp
#   define page_local_phys_attrib_Normal_hpp

#	include "../../math/Matrix.hpp"
#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"
#	include "../Frame.hpp"
#	include "../mixin/Transformable.hpp"

namespace page { namespace phys { namespace attrib
{
	class Normal :
		public virtual Transformable
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Normal(const math::Vec3 & = math::NormVector<3>());
		explicit Normal(const math::Quat<> &);
		explicit Normal(const math::Mat3 &);

		/*----------+
		| accessors |
		+----------*/

		const math::Vec3 &GetNormal() const;
		void SetNormal(const math::Vec3 &);

		// orientation view
		math::Quat<> GetOrientation() const;
		void SetOrientation(const math::Quat<> &);

		// matrix view
		math::Mat3 GetMatrix() const;
		math::Mat3 GetInvMatrix() const;
		void SetMatrix(const math::Mat3 &);

		// transformation observers
		const math::Vec3 &GetLastNormal() const;
		math::Quat<> GetLastOrientation() const;
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
		math::Vec3 value, lastValue = value;
		math::Quat<> torque, spin;
	};
}}}

#endif
