/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <cassert>
#include "../util/sstream.hpp" // make_ostringstream
#include "Var.hpp"

namespace page
{
	namespace cfg
	{
		BasicVar
			*BasicVar::first,
			*BasicVar::last;

		// constructor/destructor
		BasicVar::BasicVar(const std::string &name) : name(name),
			userSet(false), siteSet(false),
			userMod(false), siteMod(false),
			userEverMod(false)
		{
			// insert in order by name
			for (BasicVar *var = first;; var = var->next)
			{
				if (!var)
				{
					last = ((prev = last) ? last->next : first) = this;
					next = 0;
					break;
				}
				if (var->name > name)
				{
					prev = (next = var)->prev;
					var->prev = (var->prev ? var->prev->next : first) = this;
					break;
				}
			}
		}
		BasicVar::~BasicVar()
		{
			prev ? prev->next : first = next;
			next ? next->prev : last  = prev;
		}

		// observers
		const std::string &BasicVar::GetName() const
		{
			return name;
		}
		std::string BasicVar::GetRepr(Domain domain) const
		{
			MAKE_OSTRINGSTREAM_1(char)//auto ss(util::make_ostringstream<char>());
			Serialize(ss, domain);
			return ss.str();
		}
		bool BasicVar::WasUserModified() const
		{
			return userEverMod;
		}

		// modifiers
		void BasicVar::Unset(Domain domain)
		{
			switch (domain == autoDomain ? GetDomain() : domain)
			{
				case autoDomain:
				if (userSet)
				{
					UserDomain:
					userSet = false;
					userMod = true;
					userEverMod = true;
				}
				else if (siteSet)
				{
					SiteDomain:
					siteSet = false;
					siteMod = true;
				}
				break;
				case userDomain: if (userSet) goto UserDomain; break;
				case siteDomain: if (siteSet) goto SiteDomain; break;
				case defaultDomain: assert(!"default domain is read-only");
				default: assert(!"invalid domain");
			}
		}
		
		// iterators
		util::range<typename BasicVar::Iterator> BasicVar::GetGlobalRange()
		{
			return util::make_range(
				Iterator(first),
				Iterator(last));
		}
	}
}
