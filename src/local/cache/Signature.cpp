#include <ostream> // basic_ostream
#include <sstream>
#include <utility> // forward

#include <boost/range/algorithm/copy.hpp>
#include <boost/tokenizer.hpp>

#include "../err/Exception.hpp"
#include "../util/io/separated_ostream_iterator.hpp"
#include "Signature.hpp"

namespace page { namespace cache
{
////////// SigType /////////////////////////////////////////////////////////////

	SigType::SigType(const std::string &name, const std::vector<SigType> &params) :
		name(name), params(params) {}

	SigType::SigType(const char *name, const std::vector<SigType> &params) :
		name(name), params(params) {}

	SigType::SigType(const std::string &s)
	{
		std::istringstream ss(s);
		if ((ss >> *this).fail())
			THROW((err::Exception<err::CacheModuleTag, err::FormatTag>("failed to parse signature type")))
	}

	SigType::SigType(const char *s) :
		SigType(std::string(s)) {}

	SigType::operator std::string() const
	{
		std::ostringstream ss;
		ss << *this;
		return ss.str();
	}

	std::ostream &operator <<(std::ostream &os, const SigType &type)
	{
		os << type.name;
		if (!type.params.empty())
		{
			os << '<';
			boost::copy(type.params,
				util::separated_ostream_iterator<std::string>(os, ','));
			os << '>';
		}
		return os;
	}

	namespace
	{
		/**
		 *
		 */
		template <typename TokenIterator>
			bool ExtractTypeParams(
				SigType             & type,
				TokenIterator       & iter,
				TokenIterator const & end)
		{
			for (;;)
			{
				type.params.emplace_back(*iter++);

				if (iter == end)
					return false;

				switch ((*iter++).front())
				{
					case ',': break;
					case '>': return true;
					case '<':
					if (!ExtractTypeParams(type.params.back(), iter, end))
						return false;
					break;
				}
			}
		}
	}

	std::istream &operator >>(std::istream &is, SigType &type)
	{
		boost::tokenizer<
			boost::char_separator<char>,
			std::istreambuf_iterator<char>>
			tokenizer(
				std::istreambuf_iterator<char>(is),
				std::istreambuf_iterator<char>(),
				boost::char_separator<char>("", ",<>"));

		auto iter(tokenizer.begin());
		if (iter == tokenizer.end())
			return is;

		type.name = *iter++;
		if (iter == tokenizer.end())
			return is;

		if ((*iter++).front() != '<')
			return is.setstate(is.failbit), is;

		if (!ExtractTypeParams(type, iter, tokenizer.end()))
			return is.setstate(is.failbit), is;

		return is;
	}

	bool operator ==(const SigType &a, const SigType &b)
	{
		return
			a.name   == b.name &&
			a.params == b.params;
	}

	bool operator !=(const SigType &a, const SigType &b)
	{
		return
			a.name   != b.name &&
			a.params != b.params;
	}

////////// SigSource ///////////////////////////////////////////////////////////

	SigSource::SigSource(const std::vector<std::string> &parts) :
		parts(parts) {}

	SigSource::SigSource(std::initializer_list<std::string> parts) :
		parts(parts) {}

	SigSource::SigSource(const std::string &s)
	{
		std::istringstream ss(s);
		if ((ss >> *this).fail())
			THROW((err::Exception<err::CacheModuleTag, err::FormatTag>("failed to parse signature source")))
	}

	SigSource::SigSource(const char *s) :
		SigSource(std::string(s)) {}

	SigSource::operator std::string() const
	{
		std::ostringstream ss;
		ss << *this;
		return ss.str();
	}

	std::ostream &operator <<(std::ostream &os, const SigSource &source)
	{
		boost::copy(source.parts,
			util::separated_ostream_iterator<std::string>(os, ','));
		return os;
	}

	std::istream &operator >>(std::istream &is, SigSource &source)
	{
		boost::tokenizer<
			boost::char_separator<char>,
			std::istreambuf_iterator<char>>
			tokenizer(
				std::istreambuf_iterator<char>(is),
				std::istreambuf_iterator<char>(),
				boost::char_separator<char>("", ",()<>"));

		auto typeDepth(0), sourceDepth(1);
		std::ostringstream ss;
		for (auto iter(tokenizer.begin()); iter != tokenizer.end(); ++iter)
		{
			switch (iter->front())
			{
				case '(': ++sourceDepth; break;
				case ')': --sourceDepth;
				if (!sourceDepth)
				{
					source.parts.emplace_back(ss.str());
					is.unget();
					return is;
				}
				break;

				case '<': ++typeDepth; break;
				case '>': --typeDepth; break;

				case ',':
				if (!typeDepth && sourceDepth == 1)
				{
					source.parts.emplace_back(ss.str());
					ss.clear();
					continue;
				}
			}
			ss << *iter;
		}

		// ensure brackets are closed
		if (typeDepth || sourceDepth != 1)
			is.setstate(is.failbit);

		return is;
	}

	bool operator ==(const SigSource &a, const SigSource &b)
	{
		return a.parts == b.parts;
	}

	bool operator !=(const SigSource &a, const SigSource &b)
	{
		return a.parts != b.parts;
	}

////////// Signature ///////////////////////////////////////////////////////////

	Signature::Signature(const SigType &type, const SigSource &source) :
		type(type), source(source) {}

	Signature::Signature(const std::string &s)
	{
		auto i(s.find('('));
		type = s.substr(0, i);
		if (i != s.npos)
		{
			++i;
			source = s.substr(i, s.rfind(')') - i);
		}
	}

	Signature::operator std::string() const
	{
		std::ostringstream ss;
		ss << *this;
		return ss.str();
	}

	std::ostream &operator <<(std::ostream &os, const Signature &signature)
	{
		os << signature.type;
		if (!signature.source.parts.empty())
			os << '(' << signature.source << ')';
		return os;
	}

	std::istream &operator >>(std::istream &is, Signature &signature)
	{
		signature = Signature();

		{
			boost::tokenizer<
				boost::char_separator<char>,
				std::istreambuf_iterator<char>>
				tokenizer(
					std::istreambuf_iterator<char>(is),
					std::istreambuf_iterator<char>(),
					boost::char_separator<char>("", "()", boost::keep_empty_tokens));

			auto iter(tokenizer.begin());
			if (iter == tokenizer.end())
				return is;

			signature.type = *iter++;
			if (iter == tokenizer.end())
				return is;

			if ((*iter++).front() != '(')
				return is.setstate(is.failbit), is;
		}

		is >> signature.source;

		// ensure brackets are closed
		if (is.get() != ')')
			is.setstate(is.failbit);

		return is;
	}

	bool operator ==(const Signature &a, const Signature &b)
	{
		return
			a.type   == b.type &&
			a.source == b.source;
	}

	bool operator !=(const Signature &a, const Signature &b)
	{
		return
			a.type   != b.type &&
			a.source != b.source;
	}
}}
