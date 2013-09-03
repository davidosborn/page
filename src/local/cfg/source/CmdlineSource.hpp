#ifndef    page_local_cfg_source_CmdlineSource_hpp
#   define page_local_cfg_source_CmdlineSource_hpp

#	include "Source.hpp"

namespace page { namespace cfg
{
	/**
	 * Provides access to configuration variables specified on the command line.
	 */
	class CmdlineSource :
		public Source,
		public virtual util::Cloneable<CmdlineSource, Source>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		CmdlineSource(int argc, const char *const *argv);

		/*----------------------+
		| Source implementation |
		+----------------------*/

		private:
		class Reader;

		public:
		std::unique_ptr<Source::Reader> OpenReader() const override;
	};
}}

#endif
