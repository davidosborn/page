#include <cassert>
#include <memory> // {shared,unique}_ptr

#include "../../../util/cpp.hpp" // STRINGIZE
#include "../../adapt/freetype.hpp" // GetLib, OpenArgs
#include "../../format/freetype/sub.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::{~Stream,ReadSome}
#include "../LoaderRegistry.hpp" // REGISTER_LOADER
#include "freetype.hpp" // LoadFreetypeFont

namespace page { namespace res
{
	class Font;

	std::unique_ptr<Font> LoadFreetypeSubFont(const std::shared_ptr<const Pipe> &pipe)
	{
		assert(pipe);
		std::unique_ptr<Stream> stream(pipe->Open());
		fmt::Header header;
		if (stream->ReadSome(&header, sizeof header) < sizeof header) return 0;
		OpenArgs args(stream);
		return LoadFreetypeFont(args, header.index);
	}

	bool CheckFreetypeSubFont(const Pipe &pipe)
	{
		std::unique_ptr<Stream> stream(pipe.Open());
		fmt::Header header;
		if (stream->ReadSome(&header, sizeof header) < sizeof header) return 0;
		OpenArgs args(stream);
		return !FT_Open_Face(GetLib(), args.Get(), -1, 0);
	}

	REGISTER_LOADER(
		Font,
		STRINGIZE(NAME) " font",
		LoadFreetypeSubFont,
		CheckFreetypeSubFont,
		{fmt::mimeTypeForSub})
}}
