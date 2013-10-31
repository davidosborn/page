#ifndef    page_local_phys_Frame_hpp
#   define page_local_phys_Frame_hpp

#	include <string>
#	include <unordered_map>

#	include <boost/optional.hpp>

#	include "../math/Color.hpp" // RgbColor
#	include "../math/Quat.hpp"
#	include "../math/Vector.hpp"

namespace page { namespace phys
{
	/**
	 * A data structure containing the controllable state of a node.
	 */
	struct Frame
	{
		/**
		 * A channel, which is an optional portion of the node's state.
		 */
		template <typename T>
			using Channel = boost::optional<T>;

		/**
		 * A data structure for ranged state, with a minimum and maximum value.
		 */
		template <typename T> struct Range
		{
			Range() : min(T()), max(T()) {}
			Range(const T &min, const T &max) : min(min), max(max) {}

			T min, max;
		};

		/*---------------+
		| basic channels |
		+---------------*/

		Channel<math::RgbColor<>>         ambient;
		Channel<Range<math::RgbColor<>>>  ambientRange;
		Channel<math::DefaultType>        aspect;
		Channel<math::DefaultType>        attenuation;
		Channel<math::DefaultType>        cutoff;
		Channel<math::DefaultType>        depth;
		Channel<math::RgbColor<>>         diffuse;
		Channel<Range<math::RgbColor<>>>  diffuseRange;
		Channel<math::RgbColor<>>         emissive;
		Channel<Range<math::RgbColor<>>>  emissiveRange;
		Channel<math::DefaultType>        exposure;
		Channel<math::DefaultType>        falloff;
		Channel<math::DefaultType>        fov;
		Channel<Range<math::DefaultType>> lifetimeRange;
		Channel<math::Vec3>               normal;
		Channel<math::DefaultType>        opacity;
		Channel<Range<math::DefaultType>> opacityRange;
		Channel<math::Quat<>>             orientation;
		Channel<math::Vec3>               position;
		Channel<math::DefaultType>        range;
		Channel<math::Vec3>               scale;
		Channel<math::DefaultType>        size;
		Channel<Range<math::DefaultType>> sizeRange;
		Channel<math::RgbColor<>>         specular;
		Channel<Range<math::RgbColor<>>>  specularRange;
		Channel<Range<math::DefaultType>> speedRange;
		Channel<math::Vec2>               texCoord;
		Channel<math::DefaultType>        volume;

		/*--------------+
		| bone channels |
		+--------------*/

		struct Bone
		{
			Channel<math::Vec3>   position;
			Channel<math::Quat<>> orientation;
			Channel<math::Vec3>   scale;
		};
		std::unordered_map<std::string, Bone> bones;

		/*--------------+
		| part channels |
		+--------------*/

		struct Part
		{
			Channel<math::Vec3>   position;
			Channel<math::Quat<>> orientation;
			Channel<math::Vec3>   scale;
		};
		std::unordered_map<unsigned, Part> parts;

		/*----------------+
		| vertex channels |
		+----------------*/

		struct Vertex
		{
			Channel<math::Vec3> position;
			Channel<math::Vec3> normal;
			Channel<math::Vec2> texCoord;
		};
		std::unordered_map<unsigned, Vertex> vertices;
	};

	/*-----------+
	| operations |
	+-----------*/

	/**
	 * Merges two frames, with the second frame overriding any channels in the
	 * first frame that have been set.
	 */
	Frame operator +(Frame, const Frame &);

	/**
	 * Merges two frames, with the second frame overriding any channels in the
	 * first frame that have been set.
	 */
	Frame &operator +=(Frame &, const Frame &);

	/**
	 * Blends two frames by an alpha value.
	 */
	void Blend(Frame &dest, const Frame &src, float alpha);

	/**
	 * Applies a mask to a frame.  If a channel is not set in the masking frame,
	 * that channel will not be set in the destination frame either.
	 */
	void Mask(Frame &dest, const Frame &mask);
}}

#endif
