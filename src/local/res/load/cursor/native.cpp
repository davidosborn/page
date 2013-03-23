/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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
