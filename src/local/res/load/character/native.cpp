#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <boost/lexical_cast.hpp>

#include "../../../util/cpp.hpp" // STRINGIZE
#include "../../adapt/text.hpp" // CheckSig
#include "../../adapt/text/Parser.hpp"
#include "../../format/native/character.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::~Stream
#include "../../type/Character.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page { namespace res
{
	namespace
	{
		// handlers
		void OnAnimationAmbient(Character &character, const std::string &value)
		{
			character.animation.ambient = cache::Resource<Animation>(value);
		}
		void OnAnimationCheer(Character &character, const std::string &value)
		{
			character.animation.cheer = cache::Resource<Animation>(value);
		}
		void OnAnimationClap(Character &character, const std::string &value)
		{
			character.animation.clap = cache::Resource<Animation>(value);
		}
		void OnAnimationDance(Character &character, const std::string &value)
		{
			character.animation.dance = cache::Resource<Animation>(value);
		}
		void OnAnimationJump(Character &character, const std::string &value)
		{
			character.animation.jump = cache::Resource<Animation>(value);
		}
		void OnAnimationSleep(Character &character, const std::string &value)
		{
			character.animation.sleep = cache::Resource<Animation>(value);
		}
		void OnGait(Character &character, const std::string &value)
		{
			character.gait = cache::Resource<Gait>(value);
		}
		void OnModel(Character &character, const std::string &value)
		{
			character.model = cache::Resource<Model>(value);
		}
		void OnName(Character &character, const std::string &value)
		{
			character.name = value;
		}
		void OnRadius(Character &character, const std::string &value)
		{
			character.radius = boost::lexical_cast<float>(value);
		}
		void OnScale(Character &character, const std::string &value)
		{
			character.scale = boost::lexical_cast<float>(value);
		}

		// parser factory
		inline Parser GetParser(Character &character)
		{
			Parser parser;
			parser.Register("animation.ambient", Bind(OnAnimationAmbient, character), Parser::needValue);
			parser.Register("animation.cheer",   Bind(OnAnimationCheer,   character), Parser::needValue);
			parser.Register("animation.clap",    Bind(OnAnimationClap,    character), Parser::needValue);
			parser.Register("animation.dance",   Bind(OnAnimationDance,   character), Parser::needValue);
			parser.Register("animation.jump",    Bind(OnAnimationJump,    character), Parser::needValue);
			parser.Register("animation.sleep",   Bind(OnAnimationSleep,   character), Parser::needValue);
			parser.Register("gait",              Bind(OnGait,             character), Parser::needValue);
			parser.Register("model",             Bind(OnModel,            character), Parser::needValue);
			parser.Register("name",              Bind(OnName,             character), Parser::needValue);
			parser.Register("radius",            Bind(OnRadius,           character), Parser::needValue);
			parser.Register("scale",             Bind(OnScale,            character), Parser::needValue);
			return parser;
		}
	}

	std::unique_ptr<Character> LoadNativeCharacter(const std::shared_ptr<const Pipe> &pipe)
	{
		assert(pipe);
		const std::unique_ptr<Stream> stream(pipe->Open());
		if (!CheckSig(*stream, fmt::shebang)) return 0;
		const std::unique_ptr<Character> character(new Character);
		GetParser(*character).Parse(*stream);
		return character;
	}

	bool CheckNativeCharacter(const Pipe &pipe)
	{
		const std::unique_ptr<Stream> stream(pipe.Open());
		return CheckSig(*stream, fmt::shebang);
	}

	REGISTER_LOADER(
		Character,
		STRINGIZE(NAME) " character",
		LoadNativeCharacter,
		CheckNativeCharacter,
		{"application/x-page-character"},
		{"char", "character", "pagechar", "pagecharacter"})
}}
