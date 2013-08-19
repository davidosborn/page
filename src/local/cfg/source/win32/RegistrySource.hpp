#ifndef    page_local_cfg_source_win32_RegistrySource_hpp
#   define page_local_cfg_source_win32_RegistrySource_hpp

#	include <windows.h>

#	include "../Source.hpp"

namespace page { namespace cfg { namespace win32
{
////////// RegistrySource //////////////////////////////////////////////////////

	/**
	 * Represents a folder in the Windows registry.
	 */
	class RegistrySource :
		public Source,
		public virtual util::Cloneable<RegistrySource, Source>
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
}}}

#endif
