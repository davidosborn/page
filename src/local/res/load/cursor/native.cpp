#include <cassert>
#include <memory> // {shared,unique}_ptr

#include "../../../util/cpp.hpp" // STRINGIZE
#include "../../adapt/text.hpp" // CheckSig
#include "../../adapt/text/extract.hpp" // Extract
#include "../../adapt/text/Parser.hpp"
#include "../../format/native/cursor.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::~Stream
#include "../../type/Cursor.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page { namespace res
{
	namespace
	{
		// handlers
		void OnCenter(Cursor &cursor, const std::string &value)
		{
			Extract(value, cursor.center);
		}
		void OnImage(Cursor &cursor, const std::string &value)
		{
			cursor.image = cache::Resource<Image>(value);
		}
		void OnSize(Cursor &cursor, const std::string &value)
		{
			Extract(value, cursor.size);
		}

		// parser factory
		inline Parser GetParser(Cursor &cursor)
		{
			Parser parser;
			parser.Register("center", Bind(OnCenter, cursor), Parser::needValue);
			parser.Register("image",  Bind(OnImage,  cursor), Parser::needValue);
			parser.Register("size",   Bind(OnSize,   cursor), Parser::needValue);
			return parser;
		}
	}

	std::unique_ptr<Cursor> LoadNativeCursor(const std::shared_ptr<const Pipe> &pipe)
	{
		assert(pipe);
		const std::unique_ptr<Stream> stream(pipe->Open());
		if (!CheckSig(*stream, fmt::shebang)) return 0;
		const std::unique_ptr<Cursor> cursor(new Cursor);
		GetParser(*cursor).Parse(*stream);
		return cursor;
	}

	bool CheckNativeCursor(const Pipe &pipe)
	{
		const std::unique_ptr<Stream> stream(pipe.Open());
		return CheckSig(*stream, fmt::shebang);
	}

	REGISTER_LOADER(
		Cursor,
		STRINGIZE(NAME) " cursor",
		LoadNativeCursor,
		CheckNativeCursor,
		{"application/x-page-cursor"},
		{"cursor", "pagecursor"})

		"cursor,pagecursor", "", "", true)
}}
