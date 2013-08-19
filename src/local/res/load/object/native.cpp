#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <boost/lexical_cast.hpp>

#include "../../adapt/text.hpp" // CheckSig
#include "../../adapt/text/Parser.hpp"
#include "../../fmt/native/object.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::~Stream
#include "../../type/Object.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

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
				object.radius = boost::lexical_cast<float>(value);
			}
			void OnScale(Object &object, const std::string &value)
			{
				object.scale = boost::lexical_cast<float>(value);
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
