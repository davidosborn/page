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

#ifndef    page_local_cache_proxy_Proxy_hpp
#   define page_local_cache_proxy_Proxy_hpp

#	include <functional> // binary_function, function, hash
#	include <ios> // ostream
#	include <memory> // {shared,weak}_ptr
#	include <string>

#	include "../../util/class/Cloneable.hpp"

namespace page
{
	namespace cache
	{
		/**
		 * A proxy class representing an object which may be stored in the cache
		 * and can be recreated on demand.
		 */
		template <typename T>
			class Proxy :
				public util::Cloneable<Proxy<T>>
		{
			/*------+
			| types |
			+------*/

			public:
			/**
			 * The type of the object.
			 */
			typedef T value_type;

			/**
			 *
			 */
			typedef std::shared_ptr<const T> Instance;

			/**
			 * A function that will recreate an invalidated object.
			 */
			typedef std::function<void (T &)> RepairFunction;

			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			explicit Proxy(const RepairFunction & = 0);

			virtual ~Proxy();

			/*-------+
			| access |
			+-------*/

			public:
			const T &operator *() const;
			const T *operator ->() const;
			const T *get() const;

			/**
			 *
			 *
			 * @note This function is virtual for @c Reference.
			 */
			virtual Instance Lock() const;

			/*----------+
			| observers |
			+----------*/

			public:
			/**
			 * The signature of the cached object.
			 */
			std::string GetSignature() const;

			/**
			 * The type of the cached object.
			 */
			virtual std::string GetType() const = 0;

			/**
			 * @return A user-friendly string identifying the source.
			 */
			virtual std::string GetSource() const = 0;

			/**
			 * @return @c true if all of the dependencies also return @c true.
			 */
			virtual operator bool() const = 0;

			protected:
			/**
			 * @return A function object that will invalidate the cached object.
			 */
			std::function<void ()> GetInvalidate() const;

			/**
			 * @return A function object that will purge the cached object.
			 */
			std::function<void ()> GetPurge() const;

			/*----------+
			| modifiers |
			+----------*/

			public:
			/**
			 *
			 *
			 * @note This function is virtual for @c Reference.
			 */
			virtual void Invalidate() const;

			/**
			 *
			 *
			 * @note This function is virtual for @c Reference.
			 */
			virtual void Purge() const;

			/*--------------+
			| instantiation |
			+--------------*/

			private:
			/**
			 * Creates a new instance of the object.
			 */
			virtual Instance Make() const = 0;

			/*-----------+
			| comparison |
			+-----------*/

			public:
			/**
			 * A function object that compares the types of two proxies.
			 */
			struct CompareType
			{
				typedef Proxy<T> first_argument_type;
				typedef Proxy<T> second_argument_type;
				typedef bool result_type;

				bool operator ()(const Proxy<T> &, const Proxy<T> &) const;
			};

			/**
			 * A function object that compares the sources of two proxies.
			 */
			struct CompareSource
			{
				typedef Proxy<T> first_argument_type;
				typedef Proxy<T> second_argument_type;
				typedef bool result_type;

				bool operator ()(const Proxy<T> &, const Proxy<T> &) const;
			};

			/*-------------+
			| data members |
			+-------------*/

			private:
			mutable std::weak_ptr<const T> reference;
			RepairFunction repair;
		};

		/*------------------------------+
		| stream insertion & extraction |
		+------------------------------*/

		/**
		 *
		 */
		template <typename T>
			std::ostream &operator <<(std::ostream &, const Proxy<T> &);
	}
}

#	include "Proxy.tpp"
#endif
