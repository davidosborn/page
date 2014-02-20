/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
			character.animation.ambient = cache::ResourceProxy<Animation>(value);
		}
		void OnAnimationCheer(Character &character, const std::string &value)
		{
			character.animation.checache::ResourceProxy<Animation>(value);
		}
		void OnAnimationClap(Character &character, const std::string &value)
		{
			character.animation.clap = cache::ResourceProxy<Animation>(value);
		}
		void OnAnimationDance(Character &character, const std::string &value)
		{
			character.animation.dance = cache::ResourceProxy<Animation>(value);
		}
		void OnAnimationJump(Character &character, const std::string &value)
		{
			character.animation.jump = cache::ResourceProxy<Animation>(value);
		}
		void OnAnimationSleep(Character &character, const std::string &value)
		{
			character.animation.sleep = cache::ResourceProxy<Animation>(value);
		}
		void OnGait(Character &character, const std::string &value)
		{
			character.gait = cache::ResourceProxy<Gait>(value);
		}
		void OnModel(Character &character, const std::string &value)
		{
			character.model = cache::ResourceProxy<Model>(value);
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
