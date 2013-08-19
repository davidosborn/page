#ifndef    page_local_res_type_CameraSet_hpp
#   define page_local_res_type_CameraSet_hpp

#	include <string>
#	include <vector>

#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"

namespace page
{
	namespace res
	{
		struct CameraSet
		{
			struct Camera
			{
				std::string name;
				math::Vec3 position;
				math::Quat<> orientation;
				float aspect;
				float fov; // degrees
				enum Tracking
				{
					noTracking,
					positionTracking,
					orientationTracking
				} tracking;
				float trackingDistance;
			};
			typedef std::vector<Camera> Cameras;
			Cameras cameras;
			struct TrackFace
			{
				typedef std::vector<Cameras::const_iterator> CameraPointers;
				CameraPointers cameras;
			};
			typedef std::vector<TrackFace> TrackFaces;
			TrackFaces trackFaces;
		};
	}
}

#endif
