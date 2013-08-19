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
			Channel<Range<math::RgbColor<>>>::Type  ambientRange;
			Channel<math::DefaultType>::Type        aspect;
			Channel<math::DefaultType>::Type        attenuation;
			Channel<math::DefaultType>::Type        cutoff;
			Channel<math::DefaultType>::Type        depth;
			Channel<math::RgbColor<>>::Type         diffuse;
			Channel<Range<math::RgbColor<>>>::Type  diffuseRange;
			Channel<math::RgbColor<>>::Type         emissive;
			Channel<Range<math::RgbColor<>>>::Type  emissiveRange;
			Channel<math::DefaultType>::Type        exposure;
			Channel<math::DefaultType>::Type        falloff;
			Channel<math::DefaultType>::Type        fov;
			Channel<Range<math::DefaultType>>::Type lifetimeRange;
			Channel<math::Vec3>::Type               normal;
			Channel<math::DefaultType>::Type        opacity;
			Channel<Range<math::DefaultType>>::Type opacityRange;
			Channel<math::Quat<>>::Type             orientation;
			Channel<math::Vec3>::Type               position;
			Channel<math::DefaultType>::Type        range;
			Channel<math::Vec3>::Type               scale;
			Channel<math::DefaultType>::Type        size;
			Channel<Range<math::DefaultType>>::Type sizeRange;
			Channel<math::RgbColor<>>::Type         specular;
			Channel<Range<math::RgbColor<>>>::Type  specularRange;
			Channel<Range<math::DefaultType>>::Type speedRange;
			Channel<math::Vec2>::Type               texCoord;
			Channel<math::DefaultType>::Type        volume;
			// bone channels
			struct Bone
			{
				Channel<math::Vec3>::Type   position;
				Channel<math::Quat<>>::Type orientation;
				Channel<math::Vec3>::Type   scale;
			};
			typedef std::unordered_map<std::string, Bone> Bones;
			Bones bones;
			// vertex channels
			struct Vertex
			{
				Channel<math::Vec3>::Type position;
				Channel<math::Vec3>::Type normal;
				Channel<math::Vec2>::Type texCoord;
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
