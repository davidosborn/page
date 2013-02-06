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

#ifndef    page_local_cfg_source_win32_RegistrySource_hpp
#   define page_local_cfg_source_win32_RegistrySource_hpp

#	include <windows.h>

#	include "../Source.hpp"

namespace page
{
	namespace cfg
	{
		namespace win32
		{
////////// RegistrySource //////////////////////////////////////////////////////
			
			/**
			 * Represents a folder in the Windows registry.
			 */
			class RegistrySource : public Source
			{
				/*--------------------------+
				| constructors & destructor |
				+--------------------------*/

				public:
				explicit RegistrySource(HKEY key, const std::string &path);
				
				private:
				/**
				 * Generate a URI from a registry key and path.
				 */
				static std::string MakeUri(HKEY key, const std::string &path);

				/*----------------------+
				| copy & move semantics |
				+----------------------*/

				public:
				MAKE_UNCOPYABLE(RegistrySource)

				/*----------------------+
				| Source implementation |
				+----------------------*/

				private:
				class Reader;
				class Writer;
				
				public:
				std::unique_ptr<Source::Reader> OpenReader() const override;
				std::unique_ptr<Source::Writer> OpenWriter() const override;

				/*-------------+
				| data members |
				+-------------*/

				private:
				HKEY key;
				const std::string path;
			};

////////// RegistrySource::Reader //////////////////////////////////////////////
			
			/**
			 * The implementation of @c RegistrySource's reader.
			 */
			class RegistrySource::Reader : public Source::Reader
			{
				/*--------------------------+
				| constructors & destructor |
				+--------------------------*/

				public:
				explicit Reader(const RegistrySource &);
				~Reader();

				/*----------------------+
				| copy & move semantics |
				+----------------------*/

				public:
				MAKE_UNCOPYABLE(Reader)

				/*------------------------------+
				| Source::Reader implementation |
				+------------------------------*/

				public:
				boost::optional<std::string> Read(const std::string &key) override;
				boost::optional<std::vector<std::string>> ReadSequence(const std::string &key) override;

				private:
				/**
				 * A data structure for storing a raw registry value returned by
				 * @c RegQueryValueEx.
				 */
				struct RegistryValue
				{
					std::vector<BYTE> content;
					DWORD type;
				};
				
				/**
				 * Read a registry value, store its type, and return its
				 * content.
				 */
				boost::optional<RegistryValue> ReadValue(const std::string &key) const;

				/**
				 * Parse a registry value and converts it to a string.
				 */
				static std::string ParseValue(const RegistryValue &);

				/*-----------------------------------------+
				| Source::ReaderWriter covariant overrides |
				+-----------------------------------------*/

				public:
				const RegistrySource &GetSource() const;

				/*-------------+
				| data members |
				+-------------*/

				private:
				HKEY key;
			};

////////// RegistrySource::Writer //////////////////////////////////////////////
			
			/**
			 * The implementation of @c RegistrySource's writer.
			 */
			class RegistrySource::Writer : public Source::Writer
			{
				/*--------------------------+
				| constructors & destructor |
				+--------------------------*/

				public:
				explicit Writer(const RegistrySource &);
				~Writer();

				/*----------------------+
				| copy & move semantics |
				+----------------------*/

				public:
				MAKE_UNCOPYABLE(Writer)

				/*------------------------------+
				| Source::Writer implementation |
				+------------------------------*/

				public:
				void Write(const std::string &key, const std::string &value) override;
				void WriteSequence(const std::string &key, const std::vector<std::string> &value) override;
				void Remove(const std::string &key) override;

				/*-----------------------------------------+
				| Source::ReaderWriter covariant overrides |
				+-----------------------------------------*/

				public:
				const RegistrySource &GetSource() const;

				/*-------------+
				| data members |
				+-------------*/

				private:
				HKEY key;
			};
		}
	}
}

#endif
