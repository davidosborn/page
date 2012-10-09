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

#include <cassert>
#include "Normal.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Normal::Normal(const math::Vector<3> &normal) :
				normal(normal), lastNormal(normal) {}
			Normal::Normal(const math::Quat<> &orientation) :
				normal(orientation * math::NormVector<3>()),
				lastNormal(normal) {}
			Normal::Normal(const math::Matrix<3> &matrix) :
				normal(GetRotation(matrix) * math::NormVector<3>()),
				lastNormal(normal) {}

			// access
			const math::Vector<3> &Normal::GetNormal() const
			{
				return normal;
			}
			void Normal::SetNormal(const math::Vector<3> &normal)
			{
				assert(All(Near(normal, Norm(normal))));
				// NOTE: comparing value to avoid redundant dirty marking
				if (Any(normal != this->normal))
				{
					// HACK: normalize to correct floating-point drift
					this->normal = Norm(normal);
					dirtyTransformSig();
				}
			}

			// orientation access
			math::Quat<> Normal::GetOrientation() const
			{
				return math::Quat<>(math::NormVector<3>(), normal);
			}
			void Normal::SetOrientation(const math::Quat<> &orientation)
			{
				assert(Near(orientation, Norm(orientation)));
				SetNormal(orientation * math::NormVector<3>());
			}

			// matrix access
			math::Matrix<3> Normal::GetMatrix() const
			{
				return RotationMatrix(math::NormVector<3>(), normal);
			}
			math::Matrix<3> Normal::GetInvMatrix() const
			{
				return Tpos(GetMatrix());
			}
			void Normal::SetMatrix(const math::Matrix<3> &matrix)
			{
				SetNormal(GetRotation(matrix) * math::NormVector<3>());
			}

			// transform state
			const math::Vector<3> &Normal::GetLastNormal() const
			{
				return lastNormal;
			}
			math::Quat<> Normal::GetLastOrientation() const
			{
				return math::Quat<>(math::NormVector<3>(), lastNormal);
			}
			const math::Quat<> &Normal::GetTorque() const
			{
				return torque;
			}
			const math::Quat<> &Normal::GetSpin() const
			{
				return spin;
			}

			// frame serialization
			Frame Normal::GetFrame() const
			{
				Frame frame;
				frame.normal = normal;
				return frame;
			}
			void Normal::Update(const Frame &frame)
			{
				if (frame.normal) SetNormal(*frame.normal);
				else if (frame.orientation) SetOrientation(*frame.orientation);
			}

			// transform modifiers
			void Normal::BakeTransform()
			{
				lastNormal = normal;
			}
			void Normal::UpdateForce()
			{
				torque = math::Quat<>(lastNormal, normal);
			}
			void Normal::UpdateDelta()
			{
				spin = math::Quat<>(lastNormal, normal);
			}
		}
	}
}
