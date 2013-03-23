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
// FIXME: we should not use atexit here; the parsing state should not be static
// but rather a new state object should be initialized every load time
#include <cstdlib> // atexit
#include <memory> // {shared,unique}_ptr

#include <expat.h>

#include "../../../pipe/Pipe.hpp" // Pipe::Open
#include "../../../pipe/Stream.hpp"
#include "../../../type/Model.hpp"
#include "../../LoaderRegistry.hpp" // REGISTER_LOADER

// TEST: for file printing
#include <iostream>

namespace page
{
	namespace res
	{
		const unsigned bufferSize = 256;

		namespace
		{
			void OnStart(void *, const XML_Char *name, const XML_Char **attribs)
			{
				// TEST: print file
				std::cout << name << std::endl;
			}
			void OnEnd(void *, const XML_Char *name)
			{
				// TEST: print file
				std::cout << name << std::endl;
			}

			XML_Parser &GetParser();
			void DeleteParser()
			{
				XML_ParserFree(GetParser());
			}
			XML_Parser MakeParser()
			{
				XML_Parser parser(XML_ParserCreate(0));
				XML_SetElementHandler(parser, OnStart, OnEnd);
				std::atexit(DeleteParser);
				return parser;
			}
			inline XML_Parser &GetParser()
			{
				static XML_Parser parser(MakeParser());
				return parser;
			}
		}

		Model *LoadNativeExpatModel(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			// FIXME: check format, perhaps the DTD
			while (!stream->Eof())
				XML_ParseBuffer(GetParser(), stream->ReadSome(XML_GetBuffer(GetParser(), bufferSize), bufferSize), false);
			XML_ParseBuffer(GetParser(), 0, true);
			const std::unique_ptr<Model> model(new Model);
			return model.release();
		}

		LoadFunction GetNativeExpatModelLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			// FIXME: check format, perhaps the DTD
			return LoadNativeExpatModel;
		}

		REGISTER_LOADER(Model, GetNativeExpatModelLoader,
			"model.xml,xml", "", "", true)
	}
}
