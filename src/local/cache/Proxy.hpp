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

#ifndef    page_local_cache_Proxy_hpp
#   define page_local_cache_Proxy_hpp

#	include <functional> // binary_function, function, hash
#	include <ios> // ostream
#	include <memory> // {shared,weak}_ptr
#	include <string>
#	include "../util/Cloneable.hpp"

namespace page
{
	namespace cache
	{
		template <typename T> struct Proxy : util::Cloneable<Proxy<T>>
		{
			typedef T value_type;
			typedef std::shared_ptr<const T> Instance;
			typedef std::function<void (T &)> Repair;

			// construct/destroy
			explicit Proxy(const Repair & = 0);
			virtual ~Proxy();

			// access
			const T &operator *() const;
			const T *operator ->() const;
			const T *get() const;
			// HACK: virtual for Reference
			virtual Instance lock() const;

			// attributes
			std::string GetSignature() const;
			virtual std::string GetType() const = 0;
			virtual std::string GetSource() const = 0;

			// dependency satisfaction
			virtual operator bool() const = 0;

			// modifiers
			// HACK: virtual for Reference
			virtual void Invalidate() const;
			virtual void Purge() const;

			protected:
			// function objects
			std::function<void ()> GetInvalidate() const;
			std::function<void ()> GetPurge() const;

			private:
			// instantiation
			virtual Instance Make() const = 0;

			mutable std::weak_ptr<const T> reference;
			Repair repair;
		};

		// comparison
		template <typename T> struct CompareProxyType :
			std::binary_function<Proxy<T>, Proxy<T>, bool>
		{
			bool operator ()(const Proxy<T> &, const Proxy<T> &) const;
		};
		template <typename T> struct CompareProxySource :
			std::binary_function<Proxy<T>, Proxy<T>, bool>
		{
			bool operator ()(const Proxy<T> &, const Proxy<T> &) const;
		};

		// stream insertion
		template <typename T> std::ostream &operator <<(std::ostream &, const Proxy<T> &);
	}
}

namespace std
{
	// hash specialization
	template <typename T> struct hash< ::page::cache::Proxy<T>> :
		unary_function< ::page::cache::Proxy<T>, size_t>
	{
		size_t operator ()(const ::page::cache::Proxy<T> &) const;
	};
}

#	include "Proxy.tpp"
#endif
