#ifndef    page_local_phys_attrib_PositionNormal_hpp
#   define page_local_phys_attrib_PositionNormal_hpp

#	include "Normal.hpp"
#	include "Position.hpp"

namespace page { namespace phys { namespace attrib
{
	class PositionNormal :
		public Position,
		public Normal
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit PositionNormal(
			const math::Vec3 & = 0,
			const math::Vec3 & = math::NormVector<3>());
		PositionNormal(
			const math::Vec3 &,
			const math::Quat<> &);
		explicit PositionNormal(const math::Mat34 &);

		/*----------+
		| accessors |
		+----------*/

		math::Mat34 GetMatrix() const;
		math::Mat34 GetInvMatrix() const;
		void SetMatrix(const math::Mat34 &);

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
