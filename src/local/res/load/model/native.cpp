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
#include "../../adapt/text/Parser.hpp"
#include "../../fmt/native/model.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::~Stream
#include "../../type/Model.hpp"
#include "../Registry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		namespace
		{
			// handlers
			void OnPart(Model &model, const std::string &value)
			{
				model.parts.push_back(Model::Part());
				if (!value.empty()) model.parts.back().name = value;
			}
			void OnPartMaterial(Model &model, const std::string &value)
			{
				model.parts.back().material = cache::Resource<Material>(value);
			}
			void OnPartMesh(Model &model, const std::string &value)
			{
				model.parts.back().mesh = cache::Resource<Mesh>(value);
			}
			void OnSkeleton(Model &model, const std::string &value)
			{
				model.skeleton = cache::Resource<Skeleton>(value);
			}

			// parser factory
			inline Parser GetParser(Model &model)
			{
				Parser parser;
				parser.Register("part",          Bind(OnPart,         model));
				parser.Register("part.material", Bind(OnPartMaterial, model), Parser::needValue);
				parser.Register("part.mesh",     Bind(OnPartMesh,     model), Parser::needValue);
				parser.Register("skeleton",      Bind(OnSkeleton,     model), Parser::needValue);
				return parser;
			}
		}

		Model *LoadNativeModel(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			if (!CheckSig(*stream, fmt::shebang)) return 0;
			const std::unique_ptr<Model> model(new Model);
			GetParser(*model).Parse(*stream);
			return model.release();
		}

		LoadFunction GetNativeModelLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			return CheckSig(*stream, fmt::shebang) ? LoadNativeModel : LoadFunction();
		}

		REGISTER_LOADER(Model, GetNativeModelLoader,
			"model,pagemodel", "", "", true)
	}
}
