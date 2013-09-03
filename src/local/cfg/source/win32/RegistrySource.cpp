#include <algorithm> // find
#include <cstdint> // uintptr_t
#include <sstream> // ostringstream
#include <string> // to_string
#include <vector>

#include "../../../err/Exception.hpp"
#include "../../../util/class/special_member_functions.hpp" // Uncopyable
#include "RegistrySource.hpp"

#ifndef QWORD
#	include <cstdint>
#	define QWORD std::uint64_t
#endif

namespace page { namespace cfg { namespace win32
{
////////// RegistrySource::Reader //////////////////////////////////////////////

	/**
	 * The implementation of @c RegistrySource's reader.
	 */
	class RegistrySource::Reader :
		public Source::Reader,
		public util::Uncopyable<RegistrySource::Reader>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Reader(const RegistrySource &);
		~Reader();

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
		 * Read a registry value, store its type, and return its content.
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
	class RegistrySource::Writer :
		public Source::Writer,
		public util::Uncopyable<RegistrySource::Writer>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Writer(const RegistrySource &);
		~Writer();

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

////////// RegistrySource //////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	RegistrySource::RegistrySource(HKEY key, const std::string &path) :
		Source(MakeUri(key, path), "registry"),
		key(key), path(path) {}

	std::string RegistrySource::MakeUri(HKEY key, const std::string &path)
	{
		std::ostringstream ss;
		ss << "registry://";
		switch (reinterpret_cast<std::uintptr_t>(key))
		{
			case reinterpret_cast<std::uintptr_t>(HKEY_CURRENT_USER):
			ss << "HKEY_CURRENT_USER";
			break;

			case reinterpret_cast<std::uintptr_t>(HKEY_LOCAL_MACHINE):
			ss << "HKEY_LOCAL_MACHINE";
			break;

			default:
			THROW((err::Exception<err::CfgModuleTag, err::ReadOnlyTag>("source is read-only")))
		}
		ss << path;
		return ss.str();
	}

////////// RegistrySource::Writer //////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	RegistrySource::Reader::Reader(const RegistrySource &source) :
		Source::Reader(source)
	{
		if (RegOpenKeyExA(source.key, source.path.c_str(), 0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS)
			THROW((err::Exception<err::CfgModuleTag, err::Win32PlatformTag>("failed to open registry key") <<
				boost::errinfo_api_function("RegOpenKeyEx")))
	}

	RegistrySource::Reader::~Reader()
	{
		RegCloseKey(key);
	}

	/*------------------------------+
	| Source::Reader implementation |
	+------------------------------*/

	boost::optional<std::string> RegistrySource::Reader::Read(const std::string &key)
	{
		auto value(ReadValue(key));
		return value ?
			boost::optional<std::string>(ParseValue(*value)) :
			boost::optional<std::string>(boost::none);
	}

	boost::optional<std::vector<std::string>> RegistrySource::Reader::ReadSequence(const std::string &key)
	{
		auto value(ReadValue(key));
		if (!value)
			return boost::none;
		if (value->type != REG_MULTI_SZ)
			return std::vector<std::string>({ParseValue(*value)});

		std::vector<std::string> r;
		auto
			iter = value->content.begin(),
			end  = value->content.end();

		/* NOTE: If the first character is null, but not the second, skip the
		 * first because, according to the spec, two nulls are required to
		 * terminate a multi-string.
		 */
		if (iter != end && (*iter || ++iter != end && *iter))
			for (;;)
		{
			auto next = std::find(iter, end, '\0');
			r.push_back(std::string(iter, next));
			iter = next;
			if (iter == end || ++iter == end || !*iter)
				break;
		}
		return r;
	}

	boost::optional<RegistrySource::Reader::RegistryValue>
		RegistrySource::Reader::ReadValue(const std::string &key) const
	{
		// get size of registry value
		DWORD size;
		LONG result;
		switch (RegQueryValueExA(this->key, key.c_str(), 0, 0, 0, &size))
		{
			case ERROR_SUCCESS:
			break;

			case ERROR_FILE_NOT_FOUND:
			THROW((err::Exception<err::CfgModuleTag, err::KeyNotFoundTag, err::Win32PlatformTag>() <<
				boost::errinfo_api_function("RegQueryValueEx")))

			default:
			THROW((err::Exception<err::CfgModuleTag, err::Win32PlatformTag>("failed to get size of registry value") <<
				boost::errinfo_api_function("RegQueryValueEx")))
		}

		// read registry value into buffer
		RegistryValue r;
		r.content.resize(size);
		for (;;)
		{
			switch (RegQueryValueExA(this->key, key.c_str(), 0, &r.type, r.content.data(), &size))
			{
				case ERROR_SUCCESS:
				r.content.resize(size);
				break;

				case ERROR_MORE_DATA:
				r.content.resize(size);
				continue;

				case ERROR_FILE_NOT_FOUND:
				THROW((err::Exception<err::CfgModuleTag, err::KeyNotFoundTag, err::Win32PlatformTag>() <<
					boost::errinfo_api_function("RegQueryValueEx")))

				default:
				THROW((err::Exception<err::CfgModuleTag, err::Win32PlatformTag>("failed to query registry value") <<
					boost::errinfo_api_function("RegQueryValueEx")))
			}
			break;
		}
		return r;
	}

	std::string RegistrySource::Reader::ParseValue(const RegistryValue &value)
	{
		switch (value.type)
		{
			case REG_DWORD:
			return std::to_string(*reinterpret_cast<const DWORD *>(value.content.data()));

			case REG_QWORD:
			return std::to_string(*reinterpret_cast<const QWORD *>(value.content.data()));

			case REG_MULTI_SZ:
			{
				std::string r;
				r.reserve(value.content.size());
				auto
					iter = value.content.begin(),
					end  = value.content.end();

				/* NOTE: If the first character is null, but not the second,
				 * skip the first because, according to the spec, two nulls are
				 * required to terminate a multi-string.
				 */
				if (iter != end && (*iter || ++iter != end && *iter))
					for (;;)
				{
					auto next = std::find(iter, end, '\0');
					r.append(iter, next);
					iter = next;
					if (iter == end || ++iter == end || !*iter)
						break;
					r.push_back(',');
				}
				return r;
			}

			case REG_EXPAND_SZ:
			{
				// ensure content is null terminated
				std::vector<char> buffer(value.content.begin(), value.content.end());
				if (buffer.empty() || buffer.back())
					buffer.push_back('\0');

				// expand environment variables
				std::vector<char> buffer2(buffer.size() + 1);
				DWORD size;
				while ((size = ExpandEnvironmentStringsA(reinterpret_cast<const char *>(buffer.data()), buffer2.data(), buffer2.size())) > buffer2.size())
					buffer2.resize(size);
				if (!size)
					THROW((err::Exception<err::CfgModuleTag, err::Win32PlatformTag>("failed to expand environment variables") <<
						boost::errinfo_api_function("ExpandEnvironmentStrings")))

				// convert buffer to string
				return std::string(buffer2.begin(),
					std::find(buffer2.begin(), buffer2.end(), '\0'));
			}

			case REG_SZ:
			return std::string(value.content.begin(),
				std::find(value.content.begin(), value.content.end(), '\0'));

			case REG_BINARY:
			return "";

			case REG_NONE:
			return "";

			default:
			THROW((err::Exception<err::CfgModuleTag, err::Win32PlatformTag>("unknown registry value type")))
		}
	}

	/*-----------------------------------------+
	| Source::ReaderWriter covariant overrides |
	+-----------------------------------------*/

	const RegistrySource &RegistrySource::Reader::GetSource() const
	{
		return static_cast<const RegistrySource &>(Source::Reader::GetSource());
	}

////////// RegistrySource::Writer //////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	RegistrySource::Writer::Writer(const RegistrySource &source) :
		Source::Writer(source)
	{
		if (RegCreateKeyExA(source.key, source.path.c_str(), 0, 0, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, 0, &key, 0) != ERROR_SUCCESS)
			THROW((err::Exception<err::CfgModuleTag, err::Win32PlatformTag>("failed to create registry key") <<
				boost::errinfo_api_function("RegCreateKeyEx")))
	}

	RegistrySource::Writer::~Writer()
	{
		RegCloseKey(key);
	}

	/*------------------------------+
	| Source::Writer implementation |
	+------------------------------*/

	void RegistrySource::Writer::Write(const std::string &key, const std::string &value)
	{
		if (RegSetValueExA(this->key, key.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE *>(value.c_str()), value.size() + 1) != ERROR_SUCCESS)
			THROW((err::Exception<err::CfgModuleTag, err::Win32PlatformTag>("failed to set registry value") <<
				boost::errinfo_api_function("RegSetValueEx")))
	}

	void RegistrySource::Writer::WriteSequence(const std::string &key, const std::vector<std::string> &values)
	{
		/* Concatenate the values in a buffer, following the REG_MULTI_SZ
		 * format, where each value is separated by a null, and the whole thing
		 * is null-terminated.
		 */
		std::vector<BYTE> buffer;
		for (const auto &value : values)
		{
			buffer.insert(buffer.end(), value.begin(), value.end());
			buffer.push_back('\0');
		}
		buffer.push_back('\0');

		if (RegSetValueExA(this->key, key.c_str(), 0, REG_MULTI_SZ, buffer.data(), buffer.size()) != ERROR_SUCCESS)
			THROW((err::Exception<err::CfgModuleTag, err::Win32PlatformTag>("failed to set registry value") <<
				boost::errinfo_api_function("RegSetValueEx")))
	}

	void RegistrySource::Writer::Remove(const std::string &key)
	{
		if (RegDeleteValueA(this->key, key.c_str()) != ERROR_SUCCESS)
			THROW((err::Exception<err::CfgModuleTag, err::Win32PlatformTag>("failed to delete registry value") <<
				boost::errinfo_api_function("RegDeleteValue")))
	}

	/*-----------------------------------------+
	| Source::ReaderWriter covariant overrides |
	+-----------------------------------------*/

	const RegistrySource &RegistrySource::Writer::GetSource() const
	{
		return static_cast<const RegistrySource &>(Source::Writer::GetSource());
	}
}}}
