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
#include <cstring> // memcmp
#include <memory> // shared_ptr
#include <vector>
#include "../../../util/endian.hpp" // TransformEndian
#include "../../../util/scoped_ptr.hpp"
#include "../../fmt/native/animation.hpp"
#include "../../Pipe.hpp" // Pipe::Open
#include "../../Stream.hpp"
#include "../../type/Animation.hpp"
#include "../register.hpp" // LoadFunction, REGISTER_LOADER

namespace page
{
	namespace res
	{
		Animation *LoadNativeAnimation(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			util::scoped_ptr<Stream> stream(pipe->Open());
			// check signature
			char sig[sizeof fmt::sig];
			if (stream->ReadSome(sig, sizeof sig) != sizeof sig ||
				std::memcmp(sig, fmt::sig, sizeof sig)) return 0;
			// read header
			fmt::Header header;
			stream->Read(&header, sizeof header);
			util::TransformEndian(&header, fmt::headerFormat, util::littleEndian);
			// create animation
			util::scoped_ptr<Animation> anim(new Animation);
			anim->duration = header.duration;
			// read bones
			for (unsigned i = 0; i < header.bones; ++i)
			{
				// read header
				fmt::Bone fmtBone;
				stream->Read(&fmtBone, sizeof fmtBone);
				util::TransformEndian(&fmtBone, fmt::boneFormat, util::littleEndian);
				// read name
				std::vector<char> nameBuffer(fmtBone.nameSize);
				stream->Read(&*nameBuffer.begin(), nameBuffer.size());
				std::string name(nameBuffer.begin(), nameBuffer.end());
				// read frames
				Animation::Bone bone;
				for (unsigned i = 0; i < fmtBone.frames; ++i)
				{
					fmt::Frame fmtFrame;
					stream->Read(&fmtFrame, sizeof fmtFrame);
					util::TransformEndian(&fmtFrame, fmt::frameFormat, util::littleEndian);
					bone.position.frames.push_back(
						Animation::Bone::Position::Frame(
							fmtFrame.time,
							math::Vector<3>(
								fmtFrame.position[0],
								fmtFrame.position[1],
								fmtFrame.position[2])));
					bone.orientation.frames.push_back(
						Animation::Bone::Orientation::Frame(
							fmtFrame.time,
							math::Quat<>(
								fmtFrame.orientation[0],
								fmtFrame.orientation[1],
								fmtFrame.orientation[2],
								fmtFrame.orientation[3])));
					bone.scale.frames.push_back(
						Animation::Bone::Scale::Frame(
							fmtFrame.time,
							math::Vector<3>(
								fmtFrame.scale[0],
								fmtFrame.scale[1],
								fmtFrame.scale[2])));
				}
				// insert bone
				anim->bones.insert(std::make_pair(name, bone));
			}
			return anim.release();
		}

		LoadFunction GetNativeAnimationLoader(const Pipe &pipe)
		{
			util::scoped_ptr<Stream> stream(pipe.Open());
			char sig[sizeof fmt::sig];
			return stream->ReadSome(sig, sizeof sig) == sizeof sig &&
				!std::memcmp(sig, fmt::sig, sizeof sig) ? LoadNativeAnimation : LoadFunction();
		}

		REGISTER_LOADER(Animation, GetNativeAnimationLoader,
			"anim,pageanim", "", "", true)
	}
}
