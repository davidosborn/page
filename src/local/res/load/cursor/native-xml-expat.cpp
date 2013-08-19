#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <expat.h>

#include "../../../util/cpp.hpp" // STRINGIZE
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::~Stream
#include "../../type/Cursor.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page { namespace res
{
	std::unique_ptr<Cursor> LoadNativeExpatCursor(const std::shared_ptr<const Pipe> &pipe)
	{
		assert(pipe);
		const std::unique_ptr<Stream> stream(pipe->Open());
		// FIXME: implement
		const std::unique_ptr<Cursor> cursor(new Cursor);
		return cursor;
	}

	bool CheckNativeExpatCursor(const Pipe &pipe)
	{
		const std::unique_ptr<Stream> stream(pipe.Open());
		// FIXME: implement
		return false;
	}

	REGISTER_LOADER(
		Cursor,
		STRINGIZE(NAME) " cursor (XML)",
		LoadNativeExpatCursor,
		CheckNativeExpatCursor,
		{"application/x-page-cursor-xml"},
		{"cursor.xml", "xml"})
}}
