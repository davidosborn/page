#include <sstream> // [io]stringstream
#include <algorithm> // upper_bound

#include "../state/BasicState.hpp" // BasicState::{Register,Unregister}
#include "BasicVar.hpp"

namespace page { namespace cfg
{
	/*-------------+
	| constructors |
	+-------------*/

	BasicVar::BasicVar(BasicState &state, const std::string &key) :
		state(state), key(key)
	{
		state.Register(this);
	}

	BasicVar::~BasicVar()
	{
		state.Unregister(this);
	}

	/*----------+
	| observers |
	+----------*/

	const std::string &BasicVar::GetKey() const
	{
		return key;
	}

	std::string BasicVar::GetString() const
	{
		std::ostringstream ss;
		Serialize(ss);
		return ss.str();
	}

	/*---------+
	| ordering |
	+---------*/

	bool operator <(const BasicVar &a, const BasicVar &b)
	{
		return a.GetKey() < b.GetKey();
	}
}}
