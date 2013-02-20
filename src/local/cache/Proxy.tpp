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
 *
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
#include <ostream>

#include "../cache.hpp" // Fetch, Invalidate, Purge, Store
#include "../util/lexical_cast.hpp"
#include "../util/typeinfo.hpp" // GetTypeId
#include "Proxy.hpp"

namespace page
{
	namespace cache
	{
		// construct/destroy
		template <typename T> Proxy<T>::Proxy(const Repair &repair) :
			repair(repair) {}
		template <typename T> Proxy<T>::~Proxy() {}

		// access
		template <typename T> const T &Proxy<T>::operator *() const
		{
			return *get();
		}
		template <typename T> const T *Proxy<T>::operator ->() const
		{
			return get();
		}
		template <typename T> const T *Proxy<T>::get() const
		{
			return lock().get();
		}
		template <typename T> typename Proxy<T>::Instance Proxy<T>::lock() const
		{
			assert(*this);
			Instance instance(reference.lock());
			if (!instance)
			{
				std::string
					sig(GetSignature()),
					name(util::lexical_cast<std::string>(*this));
				if (!(instance = Fetch<T>(sig, name)))
				{
					instance = Make();
					assert(instance);
					Store(sig, name, instance, repair ?
						std::bind(repair, std::ref(*std::const_pointer_cast<T>(instance))) :
						std::function<void ()>());
				}
				// FIXME: we can't use the weak reference in its current state
				// because the access time will not be updated
//				reference = instance;
			}
			return instance;
		}

		// attributes
		template <typename T> std::string Proxy<T>::GetSignature() const
		{
			return util::GetTypeId<T>().name() + GetType() + GetSource();
		}

		// modifiers
		template <typename T> void Proxy<T>::Invalidate() const
		{
			std::string
				sig(GetSignature()),
				name(util::lexical_cast<std::string>(*this));
			cache::Invalidate(sig, name);
		}
		template <typename T> void Proxy<T>::Purge() const
		{
			std::string
				sig(GetSignature()),
				name(util::lexical_cast<std::string>(*this));
			cache::Purge(sig, name);
		}

		// function objects
		template <typename T> std::function<void ()> Proxy<T>::GetInvalidate() const
		{
			std::string
				sig(GetSignature()),
				name(util::lexical_cast<std::string>(*this));
			return std::bind(cache::Invalidate, sig, name);
		}
		template <typename T> std::function<void ()> Proxy<T>::GetPurge() const
		{
			std::string
				sig(GetSignature()),
				name(util::lexical_cast<std::string>(*this));
			return std::bind(
				static_cast<void (*)(const std::string &, const std::string &)>(cache::Purge),
				sig, name);
		}

		// comparison
		template <typename T> bool CompareProxyType<T>::operator ()(const Proxy<T> &a, const Proxy<T> &b) const
		{
			return a.GetType() < b.GetType();
		}
		template <typename T> bool CompareProxySource<T>::operator ()(const Proxy<T> &a, const Proxy<T> &b) const
		{
			return a.GetSource() < b.GetSource();
		}

		// stream insertion
		template <typename T> std::ostream &operator <<(std::ostream &os, const Proxy<T> &proxy)
		{
			return proxy ?
				os << proxy.GetType() << " from " << proxy.GetSource() :
				os << "invalid " << proxy.GetType();
		}
	}
}

namespace std
{
	// hash specialization
	template <typename T> size_t hash< ::page::cache::Proxy<T>>::operator ()(const ::page::cache::Proxy<T> &proxy) const
	{
		return hash<string>()(::page::util::lexical_cast<string>(proxy));
	}
}
