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

#ifndef    page_local_phys_Frame_hpp
#   define page_local_phys_Frame_hpp

#	include <string>
#	include <unordered_map>

#	include <boost/optional.hpp>

#	include "../math/Color.hpp" // RgbColor
#	include "../math/Quat.hpp"
#	include "../math/Vector.hpp"

namespace page
{
	namespace phys
	{
		struct Frame
		{
			template <typename T> struct Channel
			{
				typedef boost::optional<T> Type;
			};
			template <typename T> struct Range
			{
				Range() : min(T()), max(T()) {}
				Range(const T &min, const T &max) : min(min), max(max) {}

				T min, max;
			};
			// channels
			Channel<math::RgbColor<>>::Type         ambient;
			Channel<Range<math::RgbColor<>>>::Type ambientRange;
			Channel<float>::Type                     aspect;
			Channel<float>::Type                     attenuation;
			Channel<float>::Type                     cutoff;
			Channel<float>::Type                     depth;
			Channel<math::RgbColor<>>::Type         diffuse;
			Channel<Range<math::RgbColor<>>>::Type diffuseRange;
			Channel<math::RgbColor<>>::Type         emissive;
			Channel<Range<math::RgbColor<>>>::Type emissiveRange;
			Channel<float>::Type                     exposure;
			Channel<float>::Type                     falloff;
			Channel<float>::Type                     fov;
			Channel<Range<float>>::Type             lifetimeRange;
			Channel<math::Vector<3>>::Type          normal;
			Channel<float>::Type                     opacity;
			Channel<Range<float>>::Type             opacityRange;
			Channel<math::Quat<>>::Type             orientation;
			Channel<math::Vector<3>>::Type          position;
			Channel<float>::Type                     range;
			Channel<math::Vector<3>>::Type          scale;
			Channel<float>::Type                     size;
			Channel<Range<float>>::Type             sizeRange;
			Channel<math::RgbColor<>>::Type         specular;
			Channel<Range<math::RgbColor<>>>::Type specularRange;
			Channel<Range<float>>::Type             speedRange;
			Channel<math::Vector<2>>::Type          texCoord;
			Channel<float>::Type                     volume;
			// bone channels
			struct Bone
			{
				Channel<math::Vector<3>>::Type position;
				Channel<math::Quat<>>::Type    orientation;
				Channel<math::Vector<3>>::Type scale;
			};
			typedef std::unordered_map<std::string, Bone> Bones;
			Bones bones;
			// vertex channels
			struct Vertex
			{
				Channel<math::Vector<3>>::Type position;
				Channel<math::Vector<3>>::Type normal;
				Channel<math::Vector<2>>::Type texCoord;
			};
			typedef std::unordered_map<unsigned, Vertex> Vertices;
			Vertices vertices;
		};

		// concatenation
		Frame operator +(const Frame &, const Frame &);
		Frame &operator +=(Frame &, const Frame &);

		// blending
		void Blend(Frame &dest, const Frame &src, float alpha);
		void Blend(Frame &dest, const Frame &src, float alpha, const Frame &base);

		// masking
		void Mask(Frame &dest, const Frame &mask);
	}
}

#endif
