#include "../../err/Exception.hpp"
#include "../../util/io/deserialize.hpp"
#include "Source.hpp"

namespace page { namespace cfg
{
////////// Source //////////////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	Source::Source(
		const std::string &uri,
		const std::string &friendlyName) :
			uri(uri),
			friendlyName(friendlyName) {}

	/*----------+
	| observers |
	+----------*/

	const std::string &Source::GetUri() const
	{
		return uri;
	}

	const std::string &Source::GetFriendlyName() const
	{
		return friendlyName;
	}

	/*----+
	| I/O |
	+----*/

	std::unique_ptr<Source::Writer> Source::OpenWriter() const
	{
		THROW((err::Exception<err::CfgModuleTag, err::ReadOnlyTag>("source is read-only")))
	}

////////// Source::ReaderWriter ////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	Source::ReaderWriter::ReaderWriter(const Source &source) :
		source(source) {}

	/*----------+
	| observers |
	+----------*/

	const Source &Source::ReaderWriter::GetSource() const
	{
		return source;
	}

////////// Source::Reader //////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	Source::Reader::Reader(const Source &source) :
		ReaderWriter(source) {}

	/*-----------+
	| operations |
	+-----------*/

	boost::optional<std::vector<std::string>> Source::Reader::ReadSequence(const std::string &key)
	{
		auto value(Read(key));
		if (!value)
			return boost::none;

		std::vector<std::string> sequence;
		util::Deserialize(*value, sequence, util::SequenceDeserializationFlags::none, ',');
		return sequence;
	}

////////// Source::Writer //////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	Source::Writer::Writer(const Source &source) :
		ReaderWriter(source) {}

	/*-----------+
	| operations |
	+-----------*/

	void Source::Writer::WriteSequence(const std::string &key, const std::string &value)
	{
		// FIXME: implement
		assert(false);
	}
}}
