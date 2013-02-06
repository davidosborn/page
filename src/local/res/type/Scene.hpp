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
				math::Vector<3> position;
				math::Quat<> orientation;
				math::Vector<3> scale;
			};
			typedef std::vector<Form> Forms;
			Forms forms;
			cache::Resource<CameraSet> cameraSet;
			cache::Resource<Script> script;
			cache::Resource<Sound> music;
			cache::Resource<Track> track;
/*			struct Instance
			{
				std::string model;
				math::Vector<3> position;
				math::Quat<> orientation;
				math::Vector<3> scale;
				typedef std::vector<std::string> Animations;
				Animations animations;
			};
			typedef std::vector<Instance> Instances;
			Instances instances;*/
/*			struct Light
			{
				math::Vector<3> position;
				math::Quat<> orientation;
				math::RgbColor<> ambient, diffuse, specular;
				float attenuation, cutoff, exponent, range;
				typedef std::vector<std::string> Animations;
				Animations animations;
			};
			typedef std::vector<Light> Lights;
			Lights lights;*/
			// FIXME: these are not finished
/*			struct Sound
			{
				std::string sound;
				float volume;
				math::Vector<3> position;
				bool ambient;
			};
			typedef std::vector<Sound> Sounds;
			Sounds sounds;*/
/*			struct Water
			{
			};*/
/*			struct Terrain
			{
			};*/
		};
	}
}

#endif
