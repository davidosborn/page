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
