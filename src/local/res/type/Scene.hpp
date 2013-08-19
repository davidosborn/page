#ifndef    page_local_res_type_Scene_hpp
#   define page_local_res_type_Scene_hpp

#	include <vector>

#	include "../../cache/proxy/Resource.hpp"
#	include "../../math/Color.hpp" // RgbColor
#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"

namespace page
{
	namespace res
	{
		class CameraSet;
		class Model;
		class Script;
		class Sound;
		class Track;

		struct Scene
		{
			struct Form
			{
				Form();

				std::string name;
				cache::Resource<Model> model;
				math::Vec3 position;
				math::Quat<> orientation;
				math::Vec3 scale;
			};
			typedef std::vector<Form> Forms;
			Forms forms;
			cache::Resource<CameraSet> cameraSet;
			cache::Resource<Script> script;
			cache::Resource<Sound> music;
			cache::Resource<Track> track;
			/*struct Instance
			{
				std::string model;
				math::Vec3 position;
				math::Quat<> orientation;
				math::Vec3 scale;
				typedef std::vector<std::string> Animations;
				Animations animations;
			};
			typedef std::vector<Instance> Instances;
			Instances instances;*/
			/*struct Light
			{
				math::Vec3 position;
				math::Quat<> orientation;
				math::RgbColor<> ambient, diffuse, specular;
				float attenuation, cutoff, exponent, range;
				typedef std::vector<std::string> Animations;
				Animations animations;
			};
			typedef std::vector<Light> Lights;
			Lights lights;*/
			// FIXME: these are not finished
			/*struct Sound
			{
				std::string sound;
				float volume;
				math::Vec3 position;
				bool ambient;
			};
			typedef std::vector<Sound> Sounds;
			Sounds sounds;*/
			/*struct Water
			{
			};*/
			/*struct Terrain
			{
			};*/
		};
	}
}

#endif
