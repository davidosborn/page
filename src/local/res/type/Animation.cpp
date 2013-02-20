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

#include <algorithm> // sort

#include "Animation.hpp"
#include "register.hpp" // REGISTER_TYPE

namespace page
{
	namespace res
	{
		namespace
		{
			// channel comparison
			template <typename T> bool CompareFrames(
				const typename Animation::Channel<T>::Frame &a,
				const typename Animation::Channel<T>::Frame &b)
			{
				return a.time < b.time;
			}

			// channel sorting
			template <typename T> inline void Sort(Animation::Channel<T> &channel)
			{
				std::sort(channel.frames.begin(), channel.frames.end(), CompareFrames<T>);
			}
			template <typename T> inline bool IsSorted(const Animation::Channel<T> &channel)
			{
				float time = 0;
				for (typename Animation::Channel<T>::Frames::const_iterator frame(channel.frames.begin()); frame != channel.frames.end(); ++frame)
				{
					if (frame->time < time) return false;
					time = frame->time;
				}
				return true;
			}
			template <typename T> inline void EnsureSorted(Animation::Channel<T> &channel)
			{
				if (!IsSorted(channel)) Sort(channel);
			}
		}

		// sorting
		void Sort(Animation &anim)
		{
			Sort(anim.ambient);
			Sort(anim.aspect);
			Sort(anim.attenuation);
			Sort(anim.cutoff);
			Sort(anim.depth);
			Sort(anim.diffuse);
			Sort(anim.exposure);
			Sort(anim.falloff);
			Sort(anim.fov);
			Sort(anim.normal);
			Sort(anim.opacity);
			Sort(anim.orientation);
			Sort(anim.position);
			Sort(anim.range);
			Sort(anim.scale);
			Sort(anim.size);
			Sort(anim.specular);
			Sort(anim.texCoord);
			Sort(anim.volume);
			for (Animation::Bones::iterator iter(anim.bones.begin()); iter != anim.bones.end(); ++iter)
			{
				Animation::Bone &bone(iter->second);
				Sort(bone.position);
				Sort(bone.orientation);
				Sort(bone.scale);
			}
			for (Animation::Vertices::iterator iter(anim.vertices.begin()); iter != anim.vertices.end(); ++iter)
			{
				Animation::Vertex &vertex(iter->second);
				Sort(vertex.position);
				Sort(vertex.normal);
				Sort(vertex.texCoord);
			}
		}
		void EnsureSorted(Animation &anim)
		{
			EnsureSorted(anim.ambient);
			EnsureSorted(anim.aspect);
			EnsureSorted(anim.attenuation);
			EnsureSorted(anim.cutoff);
			EnsureSorted(anim.depth);
			EnsureSorted(anim.diffuse);
			EnsureSorted(anim.exposure);
			EnsureSorted(anim.falloff);
			EnsureSorted(anim.fov);
			EnsureSorted(anim.normal);
			EnsureSorted(anim.opacity);
			EnsureSorted(anim.orientation);
			EnsureSorted(anim.position);
			EnsureSorted(anim.range);
			EnsureSorted(anim.scale);
			EnsureSorted(anim.size);
			EnsureSorted(anim.specular);
			EnsureSorted(anim.texCoord);
			EnsureSorted(anim.volume);
			for (Animation::Bones::iterator iter(anim.bones.begin()); iter != anim.bones.end(); ++iter)
			{
				Animation::Bone &bone(iter->second);
				EnsureSorted(bone.position);
				EnsureSorted(bone.orientation);
				EnsureSorted(bone.scale);
			}
			for (Animation::Vertices::iterator iter(anim.vertices.begin()); iter != anim.vertices.end(); ++iter)
			{
				Animation::Vertex &vertex(iter->second);
				EnsureSorted(vertex.position);
				EnsureSorted(vertex.normal);
				EnsureSorted(vertex.texCoord);
			}
		}

		void PostLoadAnimation(Animation &anim)
		{
			EnsureSorted(anim);
		}

		REGISTER_TYPE(Animation, "animation", PostLoadAnimation)
	}
}
