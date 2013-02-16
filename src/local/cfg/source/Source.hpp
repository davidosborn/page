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

#ifndef    page_local_cfg_source_Source_hpp
#   define page_local_cfg_source_Source_hpp

	// C++
#	include <memory> // unique_ptr
#	include <string>
#	include <vector>

	// Boost
#	include <boost/optional.hpp>

	// local
#	include "../../util/class/copy_move.hpp" // MAKE_UNCOPYABLE

namespace page
{
	namespace cfg
	{
////////// Source //////////////////////////////////////////////////////////////

		/**
		 * The base class for configuration storage-mediums.
		 */
		class Source
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			protected:
			/**
			 * Constructor.
			 *
			 * @param[in] uri          @copydoc uri
			 * @param[in] friendlyName @copydoc friendlyName
			 */
			Source(
				const std::string &uri,
				const std::string &friendlyName);

			public:
			virtual ~Source() = default;

			/*----------------------+
			| copy & move semantics |
			+----------------------*/

			MAKE_UNCOPYABLE(Source)

			/*----------+
			| observers |
			+----------*/

			public:
			/**
			 * @return @copydoc uri
			 */
			const std::string &GetUri() const;

			/**
			 * @return @copydoc friendlyName
			 */
			const std::string &GetFriendlyName() const;

			/*----+
			| I/O |
			+----*/

			protected:
			/**
			 *
			 */
			class ReaderWriter;

			/**
			 *
			 */
			class Reader;

			/**
			 *
			 */
			class Writer;

			public:
			/**
			 * Open the source for reading
			 */
			virtual std::unique_ptr<Reader> OpenReader() const = 0;

			/**
			 * Open the source for writing.
			 */
			virtual std::unique_ptr<Writer> OpenWriter() const;

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * A URI that uniquely identifies the source.
			 */
			std::string uri;

			/**
			 * A user-friendly string that identifies the source.
			 */
			std::string friendlyName;
		};

////////// Source::ReaderWriter ////////////////////////////////////////////////

		/**
		 * The base class for source readers and writers.
		 */
		class Source::ReaderWriter
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			protected:
			explicit ReaderWriter(const Source &);

			/*----------+
			| observers |
			+----------*/

			public:
			/**
			 * @return @copydoc source
			 */
			const Source &GetSource() const;

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * The source on which this reader/writer operates.
			 */
			const Source &source;
		};

////////// Source::Reader //////////////////////////////////////////////////////

		/**
		 * The base class for source readers.
		 */
		class Source::Reader : public ReaderWriter
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			protected:
			explicit Reader(const Source &);

			/*-----------+
			| operations |
			+-----------*/

			public:
			/**
			 * Return the value of a variable stored in the source, identified
			 * by its key.
			 */
			virtual boost::optional<std::string> Read(const std::string &key) = 0;

			/**
			 * Return the value of a sequential variable stored in the source,
			 * identified by its key.
			 *
			 * The default implementation parses the string returned by @c Read
			 * to extract comma-separated elements.
			 */
			virtual boost::optional<std::vector<std::string>> ReadSequence(const std::string &key);
		};

////////// Source::Writer //////////////////////////////////////////////////////

		/**
		 * The base class for source writers.
		 */
		class Source::Writer : public ReaderWriter
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			protected:
			explicit Writer(const Source &);

			/*-----------+
			| operations |
			+-----------*/

			public:
			/**
			 * Change the value of a variable stored in the source, identified
			 * by its key.
			 */
			virtual void Write(const std::string &key, const std::string &value) = 0;

			/**
			 * Change the value of a sequential variable stored in the source,
			 * identified by its key.
			 */
			virtual void WriteSequence(const std::string &key, const std::string &value);

			/**
			 * Remove the variable having the specified key from the source.
			 */
			virtual void Remove(const std::string &key) = 0;
		};
	}
}

#endif
