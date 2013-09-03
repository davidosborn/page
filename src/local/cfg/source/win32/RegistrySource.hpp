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
		/*-------------+
		| constructors |
		+-------------*/

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
}}}

#endif
