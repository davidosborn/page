#ifndef    page_local_res_source_FileSource_hpp
#   define page_local_res_source_FileSource_hpp

#	include "Source.hpp"

namespace page { namespace res
{
	/**
	 *
	 */
	class FileSource : public Source
	{
		public:
		explicit FileSource(const std::string &path);

		/**
		 * @copydoc Source::CheckPath
		 */
		static bool CheckPath(const std::string &path);

		void Refresh() override;

		private:
		/**
		 *
		 */
		void Index();

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 *
		 */
		std::string path;

		/**
		 *
		 */
		unsigned mtime;
	};
}}

#endif
