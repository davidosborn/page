#include "../../opt.hpp" // cfgVars
#include "CmdlineSource.hpp"

namespace page { namespace cfg
{
////////// CmdlineSource::Reader definition ////////////////////////////////////

	/**
	 * The implementation of @c CmdlineSource's reader.
	 */
	class CmdlineSource::Reader : public Source::Reader
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		public:
		explicit Reader(const CmdlineSource &);

		/*----------------------+
		| copy & move semantics |
		+----------------------*/

		public:
		MAKE_UNCOPYABLE(Reader)

		/*------------------------------+
		| Source::Reader implementation |
		+------------------------------*/

		public:
		boost::optional<std::string> Read(const std::string &) override;

		/*-----------------------------------------+
		| Source::ReaderWriter covariant overrides |
		+-----------------------------------------*/

		public:
		const CmdlineSource &GetSource() const;
	};

////////// CmdlineSource ///////////////////////////////////////////////////////

	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

	CmdlineSource::CmdlineSource(int argc, const char *const *argv) :
		Source("cmdline://", "command line") {}

	/*----------------------+
	| Source implementation |
	+----------------------*/

	std::unique_ptr<Source::Reader> CmdlineSource::OpenReader() const
	{
		return std::unique_ptr<Source::Reader>(new CmdlineSource::Reader(*this));
	}

////////// CmdlineSource::Reader implementation ////////////////////////////////

	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

	CmdlineSource::Reader::Reader(const CmdlineSource &source) :
		Source::Reader(source) {}

	/*------------------------------+
	| Source::Reader implementation |
	+------------------------------*/

	boost::optional<std::string> CmdlineSource::Reader::Read(const std::string &key)
	{
		for (const auto &kv : opt::cfgVars)
			if (kv.first == key)
				return kv.second;
		return boost::none;
	}

	/*-----------------------------------------+
	| Source::ReaderWriter covariant overrides |
	+-----------------------------------------*/

	const CmdlineSource &CmdlineSource::Reader::GetSource() const
	{
		return static_cast<const CmdlineSource &>(Source::Reader::GetSource());
	}
}}
