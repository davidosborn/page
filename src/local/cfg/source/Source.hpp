#ifndef    page_local_cfg_source_Source_hpp
#   define page_local_cfg_source_Source_hpp

#	include <memory> // unique_ptr
#	include <string>
#	include <vector>

#	include <boost/optional.hpp>

#	include "../../util/class/Cloneable.hpp"

namespace page { namespace cfg
{
////////// Source //////////////////////////////////////////////////////////////

	/**
	 * The base class for configuration storage-mediums.
	 */
	class Source : public util::Cloneable<Source>
	{
		/*-------------+
		| constructors |
		+-------------*/

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
		/*-------------+
		| constructors |
		+-------------*/

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
		/*-------------+
		| constructors |
		+-------------*/

		protected:
		explicit Reader(const Source &);

		/*-----------+
		| operations |
		+-----------*/

		public:
		/**
		 * Return the value of a variable stored in the source, identified by
		 * its key.
		 */
		virtual boost::optional<std::string> Read(const std::string &key) = 0;

		/**
		 * Return the value of a sequential variable stored in the source,
		 * identified by its key.
		 *
		 * The default implementation parses the string returned by @c Read to
		 * extract comma-separated elements.
		 */
		virtual boost::optional<std::vector<std::string>> ReadSequence(const std::string &key);
	};

////////// Source::Writer //////////////////////////////////////////////////////

	/**
	 * The base class for source writers.
	 */
	class Source::Writer : public ReaderWriter
	{
		/*-------------+
		| constructors |
		+-------------*/

		protected:
		explicit Writer(const Source &);

		/*-----------+
		| operations |
		+-----------*/

		public:
		/**
		 * Change the value of a variable stored in the source, identified by
		 * its key.
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
}}

#endif
