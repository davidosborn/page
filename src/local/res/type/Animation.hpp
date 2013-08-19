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
			typedef Channel<math::Vec3>  Normal;      Normal      normal;
			typedef Channel<float>            Opacity;     Opacity     opacity;
			typedef Channel<math::Quat<>>     Orientation; Orientation orientation;
			typedef Channel<math::Vec3>  Position;    Position    position;
			typedef Channel<float>            Range;       Range       range;
			typedef Channel<math::Vec3>  Scale;       Scale       scale;
			typedef Channel<float>            Size;        Size        size;
			typedef Channel<math::RgbColor<>> Specular;    Specular    specular;
			typedef Channel<math::Vec2>  TexCoord;    TexCoord    texCoord;
			typedef Channel<float>            Volume;      Volume      volume;
			// bone channels
			struct Bone
			{
				typedef Channel<math::Vec3> Position;    Position    position;
				typedef Channel<math::Quat<>>    Orientation; Orientation orientation;
				typedef Channel<math::Vec3> Scale;       Scale       scale;
			};
			typedef std::unordered_map<std::string, Bone> Bones;
			Bones bones;
			// vertex channels
			struct Vertex
			{
				typedef Channel<math::Vec3> Position; Position position;
				typedef Channel<math::Vec3> Normal;   Normal   normal;
				typedef Channel<math::Vec2> TexCoord; TexCoord texCoord;
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
