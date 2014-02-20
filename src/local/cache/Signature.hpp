/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_cache_Signature_hpp
#   define page_local_cache_Signature_hpp

#	include <cstddef> // nullptr_t
#	include <iosfwd> // [io]stream
#	include <string>

namespace page { namespace cache
{
	/**
	 * A string wrapper that uniquely identifies a resource based on its type
	 * and source.
	 *
	 * The signature is formatted like a function call:
	 * <tt>type(dependency1, dependency2...)</tt>.
	 *
	 * A dependency represents a secondary resource that is needed to acquire
	 * the primary resource.  Together, the dependencies define the source of
	 * the primary resource.
	 */
	class Signature
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		Signature() = default;
		Signature(std::nullptr_t);

		template <typename... Args>
			explicit Signature(const std::string &type, Args &&...);

		/*----------+
		| observers |
		+----------*/

		std::string &str();
		const std::string &str() const;

		/**
		 * Returns @c true if the signature is not empty.
		 */
		explicit operator bool() const;

		/**
		 * Returns the type part of the signature.
		 */
		std::string GetType() const;

		/**
		 * Returns the source part of the signature.
		 */
		std::string GetSource() const;

		/*-------------+
		| data members |
		+-------------*/

		private:
		std::string s;
	};

	/*----------------------------+
	| stream insertion/extraction |
	+----------------------------*/

	std::ostream &operator <<(std::ostream &, const Signature &);
	std::istream &operator >>(std::istream &, Signature &);

	/*-----------+
	| comparison |
	+-----------*/

	bool operator ==(const Signature &, const Signature &);
	bool operator !=(const Signature &, const Signature &);
	bool operator < (const Signature &, const Signature &);
	bool operator > (const Signature &, const Signature &);
	bool operator <=(const Signature &, const Signature &);
	bool operator >=(const Signature &, const Signature &);
}}

////////// std::hash<Signature> ////////////////////////////////////////////////

namespace std
{
	template <typename>
		struct hash;

	/**
	 * A specialization of hash for ::page::cache::Signature.
	 */
	template <>
		struct hash<::page::cache::Signature>
	{
		using result_type   = typename hash<string>::result_type;
		using argument_type = ::page::cache::Signature;

		result_type operator ()(const argument_type &x) const noexcept;
	};
}

#	include "Signature.tpp"
#endif
