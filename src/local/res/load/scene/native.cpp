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
#include <memory> // {shared,unique}_ptr

#include "../../adapt/text.hpp" // CheckSig
#include "../../adapt/text/extract.hpp" // Extract
#include "../../adapt/text/Parser.hpp"
#include "../../fmt/native/scene.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::~Stream
#include "../../type/Scene.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		namespace
		{
			// handlers
			void OnCameraset(Scene &scene, const std::string &value)
			{
				scene.cameraSet = cache::Resource<CameraSet>(value);
			}
			void OnForm(Scene &scene, const std::string &value)
			{
				scene.forms.push_back(Scene::Form());
				if (!value.empty()) scene.forms.back().name = value;
			}
			void OnFormModel(Scene &scene, const std::string &value)
			{
				scene.forms.back().model = cache::Resource<Model>(value);
			}
			void OnFormOrientation(Scene &scene, const std::string &value)
			{
				Extract(value, scene.forms.back().orientation);
			}
			void OnFormPosition(Scene &scene, const std::string &value)
			{
				Extract(value, scene.forms.back().position);
			}
			void OnFormScale(Scene &scene, const std::string &value)
			{
				Extract(value, scene.forms.back().scale);
			}
			void OnMusic(Scene &scene, const std::string &value)
			{
				scene.music = cache::Resource<Sound>(value);
			}
			void OnScript(Scene &scene, const std::string &value)
			{
				scene.script = cache::Resource<Script>(value);
			}
			void OnTrack(Scene &scene, const std::string &value)
			{
				scene.track = cache::Resource<Track>(value);
			}

			// parser factory
			inline Parser GetParser(Scene &scene)
			{
				Parser parser;
				parser.Register("cameraset",        Bind(OnCameraset,       scene), Parser::needValue);
				parser.Register("form",             Bind(OnForm,            scene));
				parser.Register("form.model",       Bind(OnFormModel,       scene), Parser::needValue);
				parser.Register("form.orientation", Bind(OnFormOrientation, scene), Parser::needValue);
				parser.Register("form.position",    Bind(OnFormPosition,    scene), Parser::needValue);
				parser.Register("form.scale",       Bind(OnFormScale,       scene), Parser::needValue);
				parser.Register("music",            Bind(OnMusic,           scene), Parser::needValue);
				parser.Register("script",           Bind(OnScript,          scene), Parser::needValue);
				parser.Register("track",            Bind(OnTrack,           scene), Parser::needValue);
				return parser;
			}
		}

		Scene *LoadNativeScene(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			if (!CheckSig(*stream, fmt::shebang)) return 0;
			const std::unique_ptr<Scene> scene(new Scene);
			GetParser(*scene).Parse(*stream);
			return scene.release();
		}

		LoadFunction GetNativeSceneLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			return CheckSig(*stream, fmt::shebang) ? LoadNativeScene : LoadFunction();
		}

		REGISTER_LOADER(Scene, GetNativeSceneLoader,
			"pagescene,scene", "", "", true)
	}
}
