#ifndef    page_local_phys_attrib_PositionOrientationScale_hpp
#   define page_local_phys_attrib_PositionOrientationScale_hpp

#	include "PositionOrientation.hpp"
#	include "Scale.hpp"

namespace page { namespace phys { namespace attrib
{
	class PositionOrientationScale :
		public PositionOrientation,
		public Scale
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit PositionOrientationScale(
			const math::Vec3 & = 0,
			const math::Quat<> & = math::Quat<>(),
			const math::Vec3 & = 1);
		PositionOrientationScale(
			const math::Vec3 &,
			const math::Vec3 &,
			const math::Vec3 & = 1);
		explicit PositionOrientationScale(const math::Mat34 &);

		/*----------+
		| accessors |
		+----------*/

		math::Mat34 GetMatrix() const;
		math::Mat34 GetInvMatrix() const;
		void SetMatrix(const math::Mat34 &);
		void SetMatrix(const math::Mat3 &);

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
	};
}}}

#endif
