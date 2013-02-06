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
#include <cstring> // memcmp
#include <memory> // {shared,unique}_ptr
#include <vector>

#include "../../../err/Exception.hpp"
#include "../../../util/endian.hpp" // TransformEndian
#include "../../fmt/native/skeleton.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../Stream.hpp"
#include "../../type/Skeleton.hpp"
#include "../register.hpp" // LoadFunction, REGISTER_LOADER

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
					math::Vector<3>(
						fmtBone.position[0],
						fmtBone.position[1],
						fmtBone.position[2]),
					math::Quat<>(math::Vector<3>(
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
