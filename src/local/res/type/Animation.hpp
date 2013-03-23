/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_local_res_type_Animation_hpp
#   define page_local_res_type_Animation_hpp

#	include <string>
#	include <unordered_map>
#	include <vector>

#	include "../../math/Color.hpp" // RgbColor
#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"

namespace page
{
	namespace res
	{
		struct Animation
		{
			float duration;

			template <typename T> struct Channel
			{
				struct Frame
				{
					// construct
					Frame(float time, const T &value) :
						time(time), value(value) {}

					float time;
					T value;
				};
				typedef std::vector<Frame> Frames;
				Frames frames;
			};

			// channels
			typedef Channel<math::RgbColor<>> Ambient;     Ambient     ambient;
			typedef Channel<math::RgbColor<>> Aspect;      Aspect      aspect;
			typedef Channel<float>            Attenuation; Attenuation attenuation;
			typedef Channel<float>            Cutoff;      Cutoff      cutoff;
			typedef Channel<float>            Depth;       Depth       depth;
			typedef Channel<math::RgbColor<>> Diffuse;     Diffuse     diffuse;
			typedef Channel<float>            Exposure;    Exposure    exposure;
			typedef Channel<float>            Falloff;     Falloff     falloff;
			typedef Channel<float>            Fov;         Fov         fov;
			typedef Channel<math::Vector<3>>  Normal;      Normal      normal;
			typedef Channel<float>            Opacity;     Opacity     opacity;
			typedef Channel<math::Quat<>>     Orientation; Orientation orientation;
			typedef Channel<math::Vector<3>>  Position;    Position    position;
			typedef Channel<float>            Range;       Range       range;
			typedef Channel<math::Vector<3>>  Scale;       Scale       scale;
			typedef Channel<float>            Size;        Size        size;
			typedef Channel<math::RgbColor<>> Specular;    Specular    specular;
			typedef Channel<math::Vector<2>>  TexCoord;    TexCoord    texCoord;
			typedef Channel<float>            Volume;      Volume      volume;
			// bone channels
			struct Bone
			{
				typedef Channel<math::Vector<3>> Position;    Position    position;
				typedef Channel<math::Quat<>>    Orientation; Orientation orientation;
				typedef Channel<math::Vector<3>> Scale;       Scale       scale;
			};
			typedef std::unordered_map<std::string, Bone> Bones;
			Bones bones;
			// vertex channels
			struct Vertex
			{
				typedef Channel<math::Vector<3>> Position; Position position;
				typedef Channel<math::Vector<3>> Normal;   Normal   normal;
				typedef Channel<math::Vector<2>> TexCoord; TexCoord texCoord;
			};
			typedef std::unordered_map<unsigned, Vertex> Vertices;
			Vertices vertices;
		};

		// sorting
		void Sort(Animation &);
		void EnsureSorted(Animation &);
	}
}

#endif
