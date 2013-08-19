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
