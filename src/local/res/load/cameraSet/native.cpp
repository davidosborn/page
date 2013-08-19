#include <cassert>
#include <cstring> // memcmp
#include <memory> // {shared,unique}_ptr
#include <vector>

#include "../../../err/Exception.hpp"
#include "../../../util/cpp.hpp" // STRINGIZE
#include "../../../util/endian.hpp" // TransformEndian{,Array}
#include "../../format/native/cameraSet.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp"
#include "../../type/CameraSet.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page { namespace res
{
	std::unique_ptr<CameraSet> LoadNativeCameraSet(const std::shared_ptr<const Pipe> &pipe)
	{
		assert(pipe);
		const std::unique_ptr<Stream> stream(pipe->Open());
		// check signature
		char sig[sizeof fmt::sig];
		if (stream->ReadSome(sig, sizeof sig) != sizeof sig ||
			std::memcmp(sig, fmt::sig, sizeof sig)) return 0;
		// read header
		fmt::Header header;
		stream->Read(&header, sizeof header);
		util::TransformEndian(&header, fmt::headerFormat, util::littleEndian);
		// create camera set
		const std::unique_ptr<CameraSet> cs(new CameraSet);
		// read cameras
		cs->cameras.reserve(header.cameras);
		for (unsigned i = 0; i < header.cameras; ++i)
		{
			fmt::Camera fmtCamera;
			stream->Read(&fmtCamera, sizeof fmtCamera);
			util::TransformEndian(&fmtCamera, fmt::cameraFormat, util::littleEndian);
			// insert camera
			CameraSet::Camera camera =
			{
				"", // FIXME: support camera names
				math::Vec3(
					fmtCamera.position[0],
					fmtCamera.position[1],
					fmtCamera.position[2]),
				math::Quat<>(
					fmtCamera.orientation[0],
					fmtCamera.orientation[1],
					fmtCamera.orientation[2],
					fmtCamera.orientation[3]),
				4.f / 3, // FIXME: support camera aspect
				fmtCamera.fov,
				static_cast<CameraSet::Camera::Tracking>(fmtCamera.tracking),
				fmtCamera.trackingDistance
			};
			cs->cameras.push_back(camera);
		}
		// read track faces
		cs->trackFaces.reserve(header.trackFaces);
		for (unsigned i = 0; i < header.trackFaces; ++i)
		{
			fmt::TrackFace fmtTrackFace;
			stream->Read(&fmtTrackFace, sizeof fmtTrackFace);
			util::TransformEndian(&fmtTrackFace, fmt::trackFaceFormat, util::littleEndian);
			// read track face cameras
			typedef std::vector<fmt::TrackFaceCamera> FmtCameras;
			FmtCameras fmtCameras(fmtTrackFace.cameras);
			stream->Read(&*fmtCameras.begin(), sizeof *fmtCameras.begin() * fmtCameras.size());
			util::TransformEndianArray(&*fmtCameras.begin(), fmtCameras.size(), fmt::trackFaceCameraFormat, util::littleEndian);
			// insert track face cameras
			CameraSet::TrackFace trackFace;
			trackFace.cameras.reserve(fmtTrackFace.cameras);
			for (FmtCameras::iterator fmtCamera(fmtCameras.begin()); fmtCamera != fmtCameras.end(); ++fmtCamera)
			{
				if (*fmtCamera >= cs->cameras.size())
					THROW((err::Exception<err::ResModuleTag, err::FormatTag, err::RangeTag>("camera index out of range")))
				trackFace.cameras.push_back(cs->cameras.begin() + *fmtCamera);
			}
			// insert track face
			cs->trackFaces.push_back(trackFace);
		}
		return cs;
	}

	bool CheckNativeCameraSet(const Pipe &pipe)
	{
		const std::unique_ptr<Stream> stream(pipe.Open());
		char sig[sizeof fmt::sig];
		return
			stream->ReadSome(sig, sizeof sig) == sizeof sig &&
			!std::memcmp(sig, fmt::sig, sizeof sig);
	}

	REGISTER_LOADER(
		CameraSet,
		STRINGIZE(NAME) " camera-set",
		LoadNativeCameraSet,
		CheckNativeCameraSet,
		{"application/x-page-camera-set"},
		{"cam", "pagecam"})
}}
