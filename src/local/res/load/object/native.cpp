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

#include "../../../util/lexical_cast.hpp"
#include "../../adapt/text.hpp" // CheckSig
#include "../../adapt/text/Parser.hpp"
#include "../../fmt/native/object.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../Stream.hpp" // Stream::~Stream
#include "../../type/Object.hpp"
#include "../register.hpp" // LoadFunction, REGISTER_LOADER

namespace page
{
	namespace res
	{
		namespace
		{
			// handlers
			void OnAnimationAmbient(Object &object, const std::string &value)
			{
				object.animation.ambient = cache::Resource<Animation>(value);
			}
			void OnModel(Object &object, const std::string &value)
			{
				object.model = cache::Resource<Model>(value);
			}
			void OnName(Object &object, const std::string &value)
			{
				object.name = value;
			}
			void OnRadius(Object &object, const std::string &value)
			{
				object.radius = util::lexical_cast<float>(value);
			}
			void OnScale(Object &object, const std::string &value)
			{
				object.scale = util::lexical_cast<float>(value);
			}

			// parser factory
			inline Parser GetParser(Object &object)
			{
				Parser parser;
				parser.Register("animation.ambient", Bind(OnAnimationAmbient, object), Parser::needValue);
				parser.Register("model",             Bind(OnModel,            object), Parser::needValue);
				parser.Register("name",              Bind(OnName,             object), Parser::needValue);
				parser.Register("radius",            Bind(OnRadius,           object), Parser::needValue);
				parser.Register("scale",             Bind(OnScale,            object), Parser::needValue);
				return parser;
			}
		}

		Object *LoadNativeObject(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			if (!CheckSig(*stream, fmt::shebang)) return 0;
			const std::unique_ptr<Object> object(new Object);
			GetParser(*object).Parse(*stream);
			return object.release();
		}

		LoadFunction GetNativeObjectLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			return CheckSig(*stream, fmt::shebang) ? LoadNativeObject : LoadFunction();
		}

		REGISTER_LOADER(Object, GetNativeObjectLoader,
			"obj,object,pageobj,pageobject", "", "", true)
	}
}
