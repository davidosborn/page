#include <cassert>
#include <memory> // {shared,unique}_ptr

#include "../../adapt/text.hpp" // CheckSig
#include "../../adapt/text/Parser.hpp"
#include "../../fmt/native/model.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::~Stream
#include "../../type/Model.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

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
