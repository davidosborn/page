#include <algorithm> // min

#include "../math/Euler.hpp"
#include "../math/float.hpp" // DegToRad
#include "../math/interp.hpp" // HermiteScale
#include "Camera.hpp"

namespace page
{
	namespace phys
	{
		const float
			near = .05,
			far  = 50,
			fadeInDuration = 1,
			fadeExponent   = 1.5;

		// construct
		Camera::Camera(const math::Vec3 &position, const math::Quat<> &orientation, float depth) :
			PositionOrientation(position, orientation), Depth(depth) {}
		Camera::Camera(const math::Vec3 &position, const math::Vec3 &focus) :
			PositionOrientation(position, math::Quat<>(math::Euler<>(
				-math::NormVector<3>(), Norm(focus - position)))),
			Depth(Len(focus - position)) {}
		Camera::Camera(const res::CameraSet::Camera &other) :
			PositionOrientation(other.position, other.orientation),
			Aspect(other.aspect), Fov(other.fov) {}

		// focus
		void Camera::SetFocus(const math::Vec3 &focus)
		{
			math::Vec3 vector(focus - GetPosition());
			SetOrientation(math::Quat<>(math::Euler<>(
				-math::NormVector<3>(), Norm(vector))));
			SetDepth(Len(vector));
		}

		// opacity
		float Camera::GetOpacity() const
		{
			return math::HermiteScale(Opacity::GetOpacity(), fadeExponent);
		}

		// update
		void Camera::Update(float deltaTime)
		{
			float opacity = Opacity::GetOpacity();
			if (opacity < 1)
				SetOpacity(std::min(opacity + deltaTime / fadeInDuration, 1.f));
		}

		// frame serialization
		Frame Camera::GetFrame() const
		{
			return
				Aspect::GetFrame() +
				Depth::GetFrame() +
				Exposure::GetFrame() +
				Fov::GetFrame() +
				Opacity::GetFrame() +
				PositionOrientation::GetFrame();
		}
		void Camera::Update(const Frame &frame)
		{
			Aspect::Update(frame);
			Depth::Update(frame);
			Exposure::Update(frame);
			Fov::Update(frame);
			Opacity::Update(frame);
			PositionOrientation::Update(frame);
		}

		// frustum/matrix
		math::ViewFrustum<> GetViewFrustum(const Camera &camera)
		{
			return math::ViewFrustum<>(near, far,
				math::DegToRad(camera.GetFov()), camera.GetAspect(),
				camera.GetPosition(), camera.GetOrientation());
		}
		math::Frustum<> GetFrustum(const Camera &camera)
		{
			return math::GetFrustum(GetViewFrustum(camera));
		}
		math::Mat4 GetMatrix(const Camera &camera)
		{
			return math::GetMatrix(GetViewFrustum(camera));
		}
		math::Mat4 GetProjMatrix(const Camera &camera)
		{
			return math::GetProjMatrix(GetViewFrustum(camera));
		}
		math::Mat4 GetInvProjMatrix(const Camera &camera)
		{
			return math::GetInvProjMatrix(GetViewFrustum(camera));
		}
		math::Matrix<3, 4> GetViewMatrix(const Camera &camera)
		{
			return camera.GetInvMatrix();
		}
		math::Matrix<3, 4> GetInvViewMatrix(const Camera &camera)
		{
			return camera.GetMatrix();
		}
	}
}
