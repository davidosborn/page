#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <boost/lexical_cast.hpp>

#include "../../adapt/text.hpp" // CheckSig
#include "../../adapt/text/Parser.hpp"
#include "../../fmt/native/gait.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::~Stream
#include "../../type/Gait.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		namespace
		{
			// handlers
			void OnRunAnimation(Gait &gait, const std::string &value)
			{
				gait.run.animation = cache::ResourceProxy<Animation>(value);
			}
			void OnRunStride(Gait &gait, const std::string &value)
			{
				gait.run.stride = boost::lexical_cast<float>(value);
			}
			void OnSneakAnimation(Gait &gait, const std::string &value)
			{
				gait.sneak.animation = cache::ResourceProxy<Animation>(value);
			}
			void OnSneakStride(Gait &gait, const std::string &value)
			{
				gait.sneak.stride = boost::lexical_cast<float>(value);
			}
			void OnTurnAnimation(Gait &gait, const std::string &value)
			{
				gait.turn.animation = cache::ResourceProxy<Animation>(value);
			}
			void OnTurnStride(Gait &gait, const std::string &value)
			{
				gait.turn.stride = boost::lexical_cast<float>(value);
			}
			void OnWalkAnimation(Gait &gait, const std::string &value)
			{
				gait.walk.animation = cache::ResourceProxy<Animation>(value);
			}
			void OnWalkStride(Gait &gait, const std::string &value)
			{
				gait.walk.stride = boost::lexical_cast<float>(value);
			}

			// parser factory
			inline Parser GetParser(Gait &gait)
			{
				Parser parser;
				parser.Register("run");
				parser.Register("run.animation",   Bind(OnRunAnimation,   gait), Parser::needValue);
				parser.Register("run.stride",      Bind(OnRunStride,      gait), Parser::needValue);
				parser.Register("sneak");
				parser.Register("sneak.animation", Bind(OnSneakAnimation, gait), Parser::needValue);
				parser.Register("sneak.stride",    Bind(OnSneakStride,    gait), Parser::needValue);
				parser.Register("turn");
				parser.Register("turn.animation",  Bind(OnTurnAnimation,  gait), Parser::needValue);
				parser.Register("turn.stride",     Bind(OnTurnStride,     gait), Parser::needValue);
				parser.Register("walk");
				parser.Register("walk.animation",  Bind(OnWalkAnimation,  gait), Parser::needValue);
				parser.Register("walk.stride",     Bind(OnWalkStride,     gait), Parser::needValue);
				return parser;
			}
		}

		Gait *LoadNativeGait(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			if (!CheckSig(*stream, fmt::shebang)) return 0;
			const std::unique_ptr<Gait> gait(new Gait);
			GetParser(*gait).Parse(*stream);
			return gait.release();
		}

		LoadFunction GetNativeGaitLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			return CheckSig(*stream, fmt::shebang) ? LoadNativeGait : LoadFunction();
		}

		REGISTER_LOADER(Gait, GetNativeGaitLoader,
			"gait,pagegait", "", "", true)
	}
}
