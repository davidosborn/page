#ifndef    page_local_cfg_source_FileSource_hpp
#   define page_local_cfg_source_FileSource_hpp

#	include <string>

#	include "Source.hpp"

namespace page { namespace cfg
{
	/**
	 * Represents a file containing configuration variables.
	 */
	class FileSource : public Source
	{
		IMPLEMENT_CLONEABLE(FileSource, Source)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit FileSource(const std::string &);

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
		std::string path;
	};
}}

#endif
