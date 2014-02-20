/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <algorithm> // min

#include "../../math/Euler.hpp"
#include "../../math/float.hpp" // DegToRad
#include "../../math/interp.hpp" // HermiteScale
#include "Camera.hpp"

namespace page { namespace phys
{
	namespace
	{
		/**
		 * The distance to the near frustum plane.
		 */
		const float near = .05;

		/**
		 * The distance to the far frustum plane.
		 */
		const float far = 50;

		/**
		 * The duration for the camera to fade in.
		 */
		const float fadeInDuration = 1;

		/**
		 * The exponent for the interpolation function.
		 */
		const float fadeExponent = 1.5;
	}

	/*-------------+
	| constructors |
	+-------------*/

	Camera::Camera(const math::Vec3 &position, const math::Quat<> &orientation, float depth) :
		PositionOrientation(position, orientation) {}

	Camera::Camera(const math::Vec3 &position, const math::Vec3 &focus) :
		Camera(
			position,
			math::Quat<>(math::Euler<>(-math::NormVector<3>(), Norm(focus - position))),
			Len(focus - position)) {}

	Camera::Camera(const res::CameraSet::Camera &other) :
		Aspect(other.aspect), Fov(other.fov),
		PositionOrientation(other.position, other.orientation) {}

	/*--------------------+
	| Node implementation |
	+--------------------*/

	NodeType Camera::GetType() const
	{
		return NodeType::camera;
	}

	/*----------+
	| accessors |
	+----------*/

	void Camera::SetFocus(const math::Vec3 &focus)
	{
		math::Vec3 vector(focus - GetPosition());
		SetOrientation(math::Quat<>(math::Euler<>(-math::NormVector<3>(), Norm(vector))));
		SetDepth(Len(vector));
	}

	float Camera::GetOpacity() const
	{
		return math::HermiteScale(GetOpacity(), fadeExponent);
	}

	/*-------+
	| update |
	+-------*/

	void Camera::Update(float deltaTime)
	{
		auto opacity(Opacity::GetOpacity());
		if (opacity < 1)
			SetOpacity(std::min(opacity + deltaTime / fadeInDuration, 1.f));
	}

	/*--------------------+
	| frame serialization |
	+--------------------*/

	Frame Camera::GetFrame() const
	{
		return
			Aspect             ::GetFrame() +
			Depth              ::GetFrame() +
			Exposure           ::GetFrame() +
			Fov                ::GetFrame() +
			Opacity            ::GetFrame() +
			PositionOrientation::GetFrame();
	}

	void Camera::SetFrame(const Frame &frame)
	{
		Aspect             ::SetFrame(frame);
		Depth              ::SetFrame(frame);
		Exposure           ::SetFrame(frame);
		Fov                ::SetFrame(frame);
		Opacity            ::SetFrame(frame);
		PositionOrientation::SetFrame(frame);
	}

	/*-------------------------+
	| frustum/matrix accessors |
	+-------------------------*/

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
}}
