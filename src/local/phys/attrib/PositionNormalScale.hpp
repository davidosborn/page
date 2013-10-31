#ifndef    page_local_phys_attrib_PositionNormalScale_hpp
#   define page_local_phys_attrib_PositionNormalScale_hpp

#	include "PositionNormal.hpp"
#	include "Scale.hpp"

namespace page { namespace phys { namespace attrib
{
	class PositionNormalScale :
		public PositionNormal,
		public Scale
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit PositionNormalScale(
			const math::Vec3 & = 0,
			const math::Vec3 & = math::NormVector<3>(),
			const math::Vec3 & = 1);
		PositionNormalScale(
			const math::Vec3 &,
			const math::Quat<> &,
			const math::Vec3 & = 1);
		explicit PositionNormalScale(const math::Mat34 &);

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
