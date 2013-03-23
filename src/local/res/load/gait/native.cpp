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
				gait.run.animation = cache::Resource<Animation>(value);
			}
			void OnRunStride(Gait &gait, const std::string &value)
			{
				gait.run.stride = boost::lexical_cast<float>(value);
			}
			void OnSneakAnimation(Gait &gait, const std::string &value)
			{
				gait.sneak.animation = cache::Resource<Animation>(value);
			}
			void OnSneakStride(Gait &gait, const std::string &value)
			{
				gait.sneak.stride = boost::lexical_cast<float>(value);
			}
			void OnTurnAnimation(Gait &gait, const std::string &value)
			{
				gait.turn.animation = cache::Resource<Animation>(value);
			}
			void OnTurnStride(Gait &gait, const std::string &value)
			{
				gait.turn.stride = boost::lexical_cast<float>(value);
			}
			void OnWalkAnimation(Gait &gait, const std::string &value)
			{
				gait.walk.animation = cache::Resource<Animation>(value);
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
