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

#include "../../err/Exception.hpp"
#include "CameraSet.hpp"
#include "Registry.hpp" // REGISTER_TYPE

namespace page
{
	namespace res
	{
		void PostLoadCameraSet(CameraSet &cs)
		{
			// verify camera tracking type
			for (CameraSet::Cameras::const_iterator cam(cs.cameras.begin()); cam != cs.cameras.end(); ++cam)
				switch (cam->tracking)
				{
					case CameraSet::Camera::noTracking:
					case CameraSet::Camera::positionTracking:
					case CameraSet::Camera::orientationTracking: break;
					default: THROW((err::Exception<err::ResModuleTag, err::FormatTag>("invalid tracking type")))
				}
		}

		REGISTER_TYPE(CameraSet, "camera set", PostLoadCameraSet)
	}
}
