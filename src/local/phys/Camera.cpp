/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
		Camera::Camera(const math::Vector<3> &position, const math::Quat<> &orientation, float depth) :
			PositionOrientation(position, orientation), Depth(depth) {}
		Camera::Camera(const math::Vector<3> &position, const math::Vector<3> &focus) :
			PositionOrientation(position, math::Quat<>(math::Euler<>(
				-math::NormVector<3>(), Norm(focus - position)))),
			Depth(Len(focus - position)) {}
		Camera::Camera(const res::CameraSet::Camera &other) :
			PositionOrientation(other.position, other.orientation),
			Aspect(other.aspect), Fov(other.fov) {}

		// focus
		void Camera::SetFocus(const math::Vector<3> &focus)
		{
			math::Vector<3> vector(focus - GetPosition());
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
		math::Matrix<4> GetMatrix(const Camera &camera)
		{
			return math::GetMatrix(GetViewFrustum(camera));
		}
		math::Matrix<4> GetProjMatrix(const Camera &camera)
		{
			return math::GetProjMatrix(GetViewFrustum(camera));
		}
		math::Matrix<4> GetInvProjMatrix(const Camera &camera)
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
