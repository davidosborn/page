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

#include <algorithm> // find
#include <iterator> // back_inserter
#include <vector>
#include "../../../err/exception/throw.hpp" // THROW
#include "../../../util/lexical_cast.hpp"
#include "../../../util/serialize/deserialize_string.hpp" // Deserialize
#include "../../../util/win32/string.hpp" // Narrow, Native, String
#include "RegSource.hpp"

#ifndef QWORD
#	include <cstdint>
#	define QWORD std::uint64_t
#endif

namespace page
{
	namespace cfg
	{
		namespace win32
		{
			const unsigned initialBufferSize = 32;

			namespace
			{
				struct RegReader : Reader
				{
					RegReader(const Source &, HKEY, const std::string &path);
					~RegReader();

					bool Read(const std::string &name, std::string &);
					bool ReadSeq(const std::string &name, std::vector<std::string> &);

					private:
					HKEY key;
				};
				struct RegWriter : Writer
				{
					RegWriter(const Source &, HKEY, const std::string &path);
					~RegWriter();

					void Write(const std::string &name, const std::string &);
					void WriteSeq(const std::string &name, const std::vector<std::string> &);

					void Remove(const std::string &name);

					private:
					HKEY key;
				};

				// reader
				RegReader::RegReader(const Source &src, HKEY key, const std::string &path) :
					Reader(src)
				{
					if (RegOpenKeyEx(key, util::win32::Native(path).c_str(), 0, KEY_QUERY_VALUE, &this->key) != ERROR_SUCCESS)
						THROW err::PlatformException<err::Win32PlatformTag>("failed to open registry key");
				}
				RegReader::~RegReader()
				{
					RegCloseKey(key);
				}
				bool RegReader::Read(const std::string &name, std::string &str)
				{
					typedef std::vector<BYTE> Buffer;
					// HACK: MinGW GCC 4 returns a corrupt value in this context
					// (bug #30047) but making the vector static and deferring
					// initialization patches it
#if defined __MINGW32__ && GCC_VERSION >= 40000
					static Buffer buffer;
					buffer.resize(initialBufferSize);
#else
					Buffer buffer(initialBufferSize);
#endif
					DWORD type, size = buffer.size();
					LONG result;
					while ((result = RegQueryValueEx(key, util::win32::Native(name).c_str(), 0, &type, &*buffer.begin(), &size)) == ERROR_MORE_DATA)
						buffer.resize(size);
					if (result != ERROR_SUCCESS) return false;
					buffer.resize(size);
					switch (type)
					{
						case REG_DWORD:
						str.assign(util::lexical_cast<std::string>(*reinterpret_cast<const DWORD *>(&*buffer.begin())));
						return true;
						case REG_QWORD:
						str.assign(util::lexical_cast<std::string>(*reinterpret_cast<const QWORD *>(&*buffer.begin())));
						return true;
						case REG_MULTI_SZ:
						{
							str.clear();
							str.reserve(
								reinterpret_cast<LPTSTR>(&*buffer.end()) -
								reinterpret_cast<LPTSTR>(&*buffer.begin()));
							LPCTSTR
								iter = reinterpret_cast<LPTSTR>(&*buffer.begin()),
								end  = reinterpret_cast<LPTSTR>(&*buffer.end());
							// NOTE: if the first character is null, but not
							// the second, skip the first because, according to
							// the spec, two nulls are required to terminate a
							// multi-string
							if (iter != end && (*iter || ++iter != end && *iter)) for (;;)
							{
								LPCTSTR next = std::find(iter, end, TEXT('\0'));
								str.append(util::win32::Narrow(util::win32::String(iter, next)));
								iter = next;
								if (iter == end || ++iter == end || !*iter) break;
								str.push_back(',');
							}
							return true;
						}
						case REG_EXPAND_SZ:
						{
							if (buffer.back()) buffer.push_back('\0');
							std::vector<TCHAR> expBuffer(
								reinterpret_cast<LPTSTR>(&*buffer.end()) -
								reinterpret_cast<LPTSTR>(&*buffer.begin()) + 1);
							DWORD size;
							while ((size = ExpandEnvironmentStrings(
								reinterpret_cast<LPTSTR>(&*buffer.begin()),
								&*expBuffer.begin(), expBuffer.size())) > expBuffer.size())
								expBuffer.resize(size);
							if (!size) THROW err::PlatformException<err::Win32PlatformTag>("failed to expand environment variables");
							str.assign(util::win32::Narrow(util::win32::String(
								reinterpret_cast<LPTSTR>(&*expBuffer.begin()),
								reinterpret_cast<LPTSTR>(&*expBuffer.end()) - 1)));
							return true;
						}
						case REG_SZ:
						str.assign(util::win32::Narrow(util::win32::String(
							reinterpret_cast<LPTSTR>(&*buffer.begin()), std::find(
							reinterpret_cast<LPTSTR>(&*buffer.begin()),
							reinterpret_cast<LPTSTR>(&*buffer.end()), TEXT('\0')))));
						return true;
						case REG_BINARY:
						case REG_NONE:
						str.clear();
						return true;
					}
					THROW err::PlatformException<err::Win32PlatformTag>("unknown registry value type");
				}
				bool RegReader::ReadSeq(const std::string &name, std::vector<std::string> &seq)
				{
					typedef std::vector<BYTE> Buffer;
					// HACK: MinGW GCC 4 returns a corrupt value in this context
					// (bug #30047) but making the vector static and deferring
					// initialization patches it
#if defined __MINGW32__ && GCC_VERSION >= 40000
					static Buffer buffer;
					buffer.resize(initialBufferSize);
#else
					Buffer buffer(initialBufferSize);
#endif
					DWORD type, size = buffer.size();
					LONG result;
					while ((result = RegQueryValueEx(key, util::win32::Native(name).c_str(), 0, &type, &*buffer.begin(), &size)) == ERROR_MORE_DATA)
						buffer.resize(size);
					if (result != ERROR_SUCCESS) return false;
					buffer.resize(size);
					switch (type)
					{
						case REG_DWORD:
						seq.assign(1, util::lexical_cast<std::string>(*reinterpret_cast<const DWORD *>(&*buffer.begin())));
						return true;
						case REG_QWORD:
						seq.assign(1, util::lexical_cast<std::string>(*reinterpret_cast<const QWORD *>(&*buffer.begin())));
						return true;
						case REG_MULTI_SZ:
						{
							seq.clear();
							LPCTSTR
								iter = reinterpret_cast<LPTSTR>(&*buffer.begin()),
								end  = reinterpret_cast<LPTSTR>(&*buffer.end());
							// NOTE: if the first character is null, but not
							// the second, skip the first because, according to
							// the spec, two nulls are required to terminate a
							// multi-string
							if (iter != end && (*iter || ++iter != end && *iter)) for (;;)
							{
								LPCTSTR next = std::find(iter, end, TEXT('\0'));
								seq.push_back(util::win32::Narrow(util::win32::String(iter, next)));
								iter = next;
								if (iter == end || ++iter == end || !*iter) break;
							}
							return true;
						}
						case REG_EXPAND_SZ:
						{
							if (buffer.back()) buffer.push_back('\0');
							std::vector<TCHAR> expBuffer(
								reinterpret_cast<LPTSTR>(&*buffer.end()) -
								reinterpret_cast<LPTSTR>(&*buffer.begin()) + 1);
							DWORD size;
							while ((size = ExpandEnvironmentStrings(
								reinterpret_cast<LPTSTR>(&*buffer.begin()),
								&*expBuffer.begin(), expBuffer.size())) > expBuffer.size())
								expBuffer.resize(size);
							if (!size) THROW err::PlatformException<err::Win32PlatformTag>("failed to expand environment variables");
							seq.clear();
							util::Deserialize(
								util::win32::Narrow(util::win32::String(
									reinterpret_cast<LPTSTR>(&*expBuffer.begin()),
									reinterpret_cast<LPTSTR>(&*expBuffer.end()) - 1)),
								std::back_inserter(seq),
								util::SequenceDeserializationFlags::none, ',');
							return true;
						}
						case REG_SZ:
						seq.clear();
						util::Deserialize(
							util::win32::Narrow(util::win32::String(
								reinterpret_cast<LPTSTR>(&*buffer.begin()), std::find(
								reinterpret_cast<LPTSTR>(&*buffer.begin()),
								reinterpret_cast<LPTSTR>(&*buffer.end()), TEXT('\0')))),
							std::back_inserter(seq),
							util::SequenceDeserializationFlags::none, ',');
						return true;
						case REG_BINARY:
						case REG_NONE:
						seq.clear();
						return true;
					}
					THROW err::PlatformException<err::Win32PlatformTag>("unknown registry value type");
				}

				// writer
				RegWriter::RegWriter(const Source &src, HKEY key, const std::string &path) :
					Writer(src)
				{
					if (RegCreateKeyEx(key, util::win32::Native(path).c_str(), 0, 0, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, 0, &this->key, 0) != ERROR_SUCCESS)
						THROW err::PlatformException<err::Win32PlatformTag>("failed to create registry key");
				}
				RegWriter::~RegWriter()
				{
					RegCloseKey(key);
				}
				void RegWriter::Write(const std::string &name, const std::string &str)
				{
					util::win32::String ts(util::win32::Native(str));
					if (RegSetValueEx(key, util::win32::Native(name).c_str(), 0, REG_SZ, reinterpret_cast<const BYTE *>(ts.c_str()), ts.size() + 1) != ERROR_SUCCESS)
						THROW err::PlatformException<err::Win32PlatformTag>("failed to set registry value");
				}
				void RegWriter::WriteSeq(const std::string &name, const std::vector<std::string> &seq)
				{
					std::vector<TCHAR> buffer;
					for (std::vector<std::string>::const_iterator iter(seq.begin()); iter != seq.end(); ++iter)
					{
						buffer.reserve(buffer.size() + iter->size() + 1);
						util::win32::String ts(util::win32::Native(*iter));
						buffer.insert(buffer.end(), ts.begin(), ts.end());
						buffer.push_back(TEXT('\0'));
					}
					buffer.push_back(TEXT('\0'));
					if (RegSetValueEx(key, util::win32::Native(name).c_str(), 0, REG_SZ,
						reinterpret_cast<LPBYTE>(&*buffer.begin()),
						reinterpret_cast<LPBYTE>(&*buffer.end()) -
						reinterpret_cast<LPBYTE>(&*buffer.begin())) != ERROR_SUCCESS)
							THROW err::PlatformException<err::Win32PlatformTag>("failed to set registry value");
				}
				void RegWriter::Remove(const std::string &name)
				{
					if (RegDeleteValue(key, util::win32::Native(name).c_str()) != ERROR_SUCCESS)
						THROW err::PlatformException<err::Win32PlatformTag>("failed to delete registry value");
				}
			}

			// source
			RegSource::RegSource(HKEY key, const std::string &path) :
				key(key), path(path) {}
			Reader *RegSource::OpenReader() const
			{
				return new RegReader(*this, key, path);
			}
			Writer *RegSource::OpenWriter() const
			{
				return new RegWriter(*this, key, path);
			}
			std::string RegSource::GetName() const
			{
				return std::string(
					key == HKEY_CURRENT_USER ? "HKEY_CURRENT_USER\\" :
					key == HKEY_LOCAL_MACHINE ? "HKEY_LOCAL_MACHINE\\" :
					"") + path;
			}
		}
	}
}
