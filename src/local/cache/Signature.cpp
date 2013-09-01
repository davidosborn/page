#include <functional> // hash
#include <istream>
#include <ostream>

#include "Signature.hpp"

namespace page { namespace cache
{
	// string access
	std::string &Signature::str()
	{
		return s;
	}

	const std::string &Signature::str() const
	{
		return s;
	}

	// stream insertion/extraction
	std::ostream &operator <<(std::ostream &os, const Signature &signature)
	{
		return os << signature.str();
	}

	std::istream &operator >>(std::istream &is, Signature &signature)
	{
		return is >> signature.str();
	}

	// comparison
	bool operator ==(const Signature &a, const Signature &b)
	{
		return a.str() == b.str();
	}

	bool operator !=(const Signature &a, const Signature &b)
	{
		return a.str() != b.str();
	}
}}

////////// std::hash<Signature> ////////////////////////////////////////////////

namespace std
{
	auto hash<::page::cache::Signature>::operator ()(
		const argument_type &x) const noexcept -> result_type
	{
		return hash<string>()(x.str());
	}
}
