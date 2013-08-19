#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <expat.h>

#include "../../../util/cpp.hpp" // STRINGIZE
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::~Stream
#include "../../type/Character.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page { namespace res
{
	std::unique_ptr<Character> LoadNativeExpatCharacter(const std::shared_ptr<const Pipe> &pipe)
	{
		assert(pipe);
		const std::unique_ptr<Stream> stream(pipe->Open());
		// FIXME: implement
		const std::unique_ptr<Character> character(new Character);
		return character;
	}

	bool CheckNativeExpatCharacter(const Pipe &pipe)
	{
		const std::unique_ptr<Stream> stream(pipe.Open());
		// FIXME: implement
		return false;
	}

	REGISTER_LOADER(
		Character,
		STRINGIZE(NAME) " character (XML)",
		LoadNativeExpatCharacter,
		CheckNativeExpatCharacter,
		{"application/x-page-character-xml"},
		{"char.xml", "character.xml", "xml"})
}}
