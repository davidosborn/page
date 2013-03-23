/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#include <cassert>
#include "Orientation.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Orientation::Orientation(const math::Quat<> &orientation) :
				orientation(orientation), lastOrientation(orientation) {}
			Orientation::Orientation(const math::Vector<3> &normal) :
				orientation(math::NormVector<3>(), normal),
				lastOrientation(orientation) {}
			Orientation::Orientation(const math::Matrix<3> &matrix) :
				orientation(math::GetOrientation(matrix)),
				lastOrientation(orientation) {}

			// access
			const math::Quat<> &Orientation::GetOrientation() const
			{
				return orientation;
			}
			void Orientation::SetOrientation(const math::Quat<> &orientation)
			{
				assert(Near(orientation, Norm(orientation)));
				// NOTE: comparing value to avoid redundant dirty marking
				if (orientation != this->orientation)
				{
					// HACK: normalize to correct floating-point drift
					this->orientation = Norm(orientation);
					dirtyTransformSig();
				}
			}

			// normal access
			math::Vector<3> Orientation::GetNormal() const
			{
				return orientation * math::NormVector<3>();
			}
			void Orientation::SetNormal(const math::Vector<3> &normal)
			{
				assert(All(Near(normal, Norm(normal))));
				SetOrientation(math::Quat<>(math::NormVector<3>(), normal));
			}

			// matrix access
			math::Matrix<3> Orientation::GetMatrix() const
			{
				return RotationMatrix(orientation);
			}
			math::Matrix<3> Orientation::GetInvMatrix() const
			{
				return RotationMatrix(Inv(orientation));
			}
			void Orientation::SetMatrix(const math::Matrix<3> &matrix)
			{
				SetOrientation(math::GetOrientation(matrix));
			}

			// transform state
			const math::Quat<> &Orientation::GetLastOrientation() const
			{
				return lastOrientation;
			}
			math::Vector<3> Orientation::GetLastNormal() const
			{
				return lastOrientation * math::NormVector<3>();
			}
			const math::Quat<> &Orientation::GetTorque() const
			{
				return torque;
			}
			const math::Quat<> &Orientation::GetSpin() const
			{
				return spin;
			}

			// frame serialization
			Frame Orientation::GetFrame() const
			{
				Frame frame;
				frame.orientation = orientation;
				return frame;
			}
			void Orientation::Update(const Frame &frame)
			{
				if (frame.orientation) SetOrientation(*frame.orientation);
				else if (frame.normal) SetNormal(*frame.normal);
			}

			// transform modifiers
			void Orientation::BakeTransform()
			{
				lastOrientation = orientation;
			}
			void Orientation::UpdateForce()
			{
				torque = orientation / lastOrientation;
			}
			void Orientation::UpdateDelta()
			{
				spin = orientation / lastOrientation;
			}
		}
	}
}
