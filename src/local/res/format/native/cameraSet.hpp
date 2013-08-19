#ifndef    page_local_res_format_native_cameraSet_hpp
#   define page_local_res_format_native_cameraSet_hpp

#	include <cstdint> // uint32_t

namespace page { namespace res { namespace format
{
	namespace native { namespace cameraSet
	{
		const char sig[] = {'P', 'A', 'G', 'E', 'c', 'a', 'm'};

#	pragma pack(push, 1)
		struct Header
		{
			std::uint32_t cameras, trackFaces;
		};
		struct Camera
		{
			float position[3];
			float orientation[4];
			float fov; // degrees
			std::uint32_t tracking;
			float trackingDistance;
		};
		struct TrackFace
		{
			std::uint32_t cameras;
		};
		typedef std::uint32_t TrackFaceCamera;
#	pragma pack(pop)

		const char headerFormat[] = "dd";
		const char cameraFormat[] = "dddddddddd";
		const char trackFaceFormat[] = "d";
		const char trackFaceCameraFormat[] = "d";
	}}

	using namespace native::cameraSet;
}}}

#endif
