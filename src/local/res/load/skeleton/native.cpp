#include <cassert>
#include <cstring> // memcmp
#include <memory> // {shared,unique}_ptr
#include <vector>

#include "../../../err/Exception.hpp"
#include "../../../util/endian.hpp" // TransformEndian
#include "../../fmt/native/skeleton.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp"
#include "../../type/Skeleton.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		Skeleton *LoadNativeSkeleton(const std::shared_ptr<const Pipe> &pipe)
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
			// create skeleton
			const std::unique_ptr<Skeleton> skel(new Skeleton);
			// read and fill bones
			std::vector<int> parents;
			parents.reserve(header.bones);
			skel->bones.reserve(header.bones);
			for (unsigned i = 0; i < header.bones; ++i)
			{
				// read bone name
				fmt::BoneName fmtBoneName;
				stream->Read(&fmtBoneName, sizeof fmtBoneName);
				util::TransformEndian(&fmtBoneName, fmt::boneNameFormat, util::littleEndian);
				std::vector<char> nameBuffer(fmtBoneName.size);
				stream->Read(&*nameBuffer.begin(), nameBuffer.size());
				// read bone
				fmt::Bone fmtBone;
				stream->Read(&fmtBone, sizeof fmtBone);
				util::TransformEndian(&fmtBone, fmt::boneFormat, util::littleEndian);
				// fill bone
				Skeleton::Bone bone =
				{
					std::string(nameBuffer.begin(), nameBuffer.end()),
					math::Vec3(
						fmtBone.position[0],
						fmtBone.position[1],
						fmtBone.position[2]),
					math::Quat<>(math::Vec3(
						fmtBone.orientation[0],
						fmtBone.orientation[1],
						fmtBone.orientation[2])),
					math::ScaleVector<3>()
				};
				skel->bones.push_back(bone);
				parents.push_back(fmtBone.parent);
			}
			// validate and fill bone parents
			std::vector<int>::const_iterator parent(parents.begin());
			for (Skeleton::Bones::iterator bone(skel->bones.begin()); bone != skel->bones.end(); ++bone, ++parent)
			{
				if (*parent < -1 || *parent + 1 > skel->bones.size())
					THROW((err::Exception<err::ResModuleTag, err::FormatTag, err::RangeTag>("bone index out of range")))
				if (*parent != -1) bone->parent = &skel->bones[*parent];
			}
			// done reading
			stream.reset();
			return skel.release();
		}

		LoadFunction GetNativeSkeletonLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			char sig[sizeof fmt::sig];
			return stream->ReadSome(sig, sizeof sig) == sizeof sig &&
				!std::memcmp(sig, fmt::sig, sizeof sig) ? LoadNativeSkeleton : LoadFunction();
		}

		REGISTER_LOADER(Skeleton, GetNativeSkeletonLoader,
			"pageskel,pageskeleton,skel,skeleton", "", "", true)
	}
}
