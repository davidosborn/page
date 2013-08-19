#include <cassert>
#include <cstring> // memcmp
#include <memory> // {shared,unique}_ptr
#include <vector>

#include "../../../err/Exception.hpp"
#include "../../../util/endian.hpp" // TransformEndian
#include "../../fmt/native/track.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp"
#include "../../type/Track.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		Track *LoadNativeTrack(const std::shared_ptr<const Pipe> &pipe)
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
			// read faces
			std::vector<fmt::Face> faces(header.faces);
			stream->Read(&*faces.begin(), sizeof *faces.begin() * faces.size());
			for (std::vector<fmt::Face>::iterator iter(faces.begin()); iter != faces.end(); ++iter)
				util::TransformEndian(&*iter, fmt::faceFormat, util::littleEndian);
			// done reading
			stream.reset();
			// validate ranges
			for (std::vector<fmt::Face>::const_iterator face(faces.begin()); face != faces.end(); ++face)
				for (const std::int32_t *neighbour = face->neighbours; neighbour != face->neighbours + 3; ++neighbour)
					if (*neighbour + 1 > faces.size())
						THROW((err::Exception<err::ResModuleTag, err::FormatTag, err::RangeTag>("face index out of range")))
			// create track
			const std::unique_ptr<Track> track(new Track);
			// fill faces
			track->faces.resize(faces.size());
			Track::Faces::iterator face(track->faces.begin());
			for (std::vector<fmt::Face>::const_iterator fmtFace(faces.begin()); fmtFace != faces.end(); ++fmtFace, ++face)
			{
				for (unsigned i = 0; i < 3; ++i)
				{
					face->vertices[i] = math::Vec3(
						fmtFace->vertices[i][0],
						fmtFace->vertices[i][1],
						fmtFace->vertices[i][2]);
					face->neighbours[i] = fmtFace->neighbours[i] != -1 ?
						&*track->faces.begin() + fmtFace->neighbours[i] : 0;
				}
			}
			return track.release();
		}

		LoadFunction GetNativeTrackLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			char sig[sizeof fmt::sig];
			return stream->ReadSome(sig, sizeof sig) == sizeof sig &&
				!std::memcmp(sig, fmt::sig, sizeof sig) ? LoadNativeTrack : LoadFunction();
		}

		REGISTER_LOADER(Track, GetNativeTrackLoader,
			"pagetrack,track", "", "", true)
	}
}
