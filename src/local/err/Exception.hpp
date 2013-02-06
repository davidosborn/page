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

#ifndef    page_local_err_Exception_hpp
#   define page_local_err_Exception_hpp

#	include <exception>
#	include <string>

	// Boost
#	include <boost/exception/all.hpp>
#	include <boost/mpl/back_inserter.hpp>
#	include <boost/mpl/copy.hpp>
#	include <boost/mpl/permutate.hpp>
#	include <boost/mpl/transform.hpp>
#	include <boost/mpl/unpack_to_template.hpp>
#	include <boost/mpl/vector.hpp>

	// local
#	include "Tag.hpp"

namespace page
{
	namespace err
	{
		/**
		 * A base class representing an exception.
		 */
		class BasicException :
			public virtual std::exception,
			public virtual boost::exception
		{
			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			explicit BasicException(const char *description = "unknown error") :
				description(description) {}

			explicit BasicException(const std::string &description) :
				description(description) {}

			/*----------+
			| observers |
			+----------*/

			public:
			/**
			 * @return @copydoc description
			 */
			const char *what() const noexcept override
			{
				return description.c_str();
			}

			/*-----------------+
			| member variables |
			+-----------------*/

			private:
			/**
			 * A string describing the exception.
			 */
			std::string description;
		};

////////////////////////////////////////////////////////////////////////////////

		namespace detail
		{
			/**
			 * A base class of @c Exception, which is meant to be used as a
			 * target for @c catch.  It is instantiated for each permutation of
			 * @c Exception::Tags, allowing the exception to be caught with any
			 * combination of its associated tags.
			 */
			template <typename... Tags>
				class ExceptionPermutation :
					public virtual BasicException
			{
				protected:
				ExceptionPermutation() {}

				explicit ExceptionPermutation(const char *description) :
					BasicException(description) {}

				explicit ExceptionPermutation(const std::string &description) :
					BasicException(description) {}
			};

			/**
			 * @return A normalized instantiation of @c ExceptionPermutation
			 *         that can be used in a @c catch statement to catch an
			 *         @c Exception having at least the given tags.
			 */
			template <typename... _Tags>
				struct GetExceptionPermutation
			{
				typedef
					typename boost::mpl::unpack_to_template<
						ExceptionPermutation
						>::apply<
							typename TagSet<_Tags...>::Tags
							>::type type;
			};

			/**
			 * An intermediate class for deriving new exception classes from
			 * multiple base classes.
			 */
			template <typename... Bases>
				class ExceptionInheritor :
					public virtual Bases...
			{
				protected:
				ExceptionInheritor() {}

				explicit ExceptionInheritor(const char *description) :
					Bases(description)... {}

				explicit ExceptionInheritor(const std::string &description) :
					Bases(description)... {}
			};

			/**
			 * @return A type which inherits from a normalized instantiation of
			 *         @c ExceptionPermutation for each permutation of the
			 *         provided tags.
			 */
			template <typename... _Tags>
				struct GetExceptionPermutations
			{
				typedef
					typename boost::mpl::unpack_to_template<ExceptionInheritor>::apply<
						typename boost::mpl::transform<
							typename boost::mpl::copy<
								typename boost::mpl::permutate<
									typename TagSet<_Tags...>::Tags
									>::type,
								boost::mpl::back_inserter<
									boost::mpl::vector<
										boost::mpl::vector<>>>
								>::type,
							boost::mpl::unpack_to_template<ExceptionPermutation>
							>::type
						>::type type;
			};
		}

////////////////////////////////////////////////////////////////////////////////

		/**
		 * A @c boost::error_info tag which describes the tags associated with
		 * an exception.
		 */
		using errinfo_tags = boost::error_info<struct errinfo_tags_, RuntimeTagSet>;

////////////////////////////////////////////////////////////////////////////////

		/**
		 * A tagged exception class.
		 */
		template <typename... _Tags>
			class Exception final :
				public virtual detail::GetExceptionPermutations<_Tags...>::type
		{
			/*------+
			| types |
			+------*/

			private:
			/**
			 * The tags associated with this exception, represented by a
			 * @c TagSet.
			 */
			using TagSet = err::TagSet<_Tags...>;

			public:
			/**
			 * The tags associated with this exception, represented by a
			 * Boost/MPL sequence.
			 */
			using Tags = typename TagSet::Tags;

			/**
			 * An instantiation of @c detail::ExceptionPermutation which has all
			 * of the associated tags.
			 */
			using Permutation = typename detail::GetExceptionPermutation<_Tags...>::type;

			private:
			/**
			 * A type which inherits from @c ExceptionPermutation for each
			 * permutation of the associated tags.
			 */
			using Permutations = typename detail::GetExceptionPermutations<_Tags...>::type;

			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			public:
			Exception() {}

			explicit Exception(const char *description) :
				BasicException(description),
				Permutations(description) {}

			explicit Exception(const std::string &description) :
				BasicException(description),
				Permutations(description) {}

			/*---------------+
			| initialization |
			+---------------*/

			private:
			/**
			 * Perform the initialization tasks that are common to all
			 * constructors, such adding some @c boost::error_info tags to the
			 * exception.
			 */
			struct Initializer
			{
				explicit Initializer(boost::exception &e)
				{
					e << errinfo_tags(RuntimeTagSet(TagSet()));
				}
			} _initializer = Initializer(*this);
		};
	}
}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Throw an exception in exactly the same way as @c BOOST_THROW_EXCEPTION,
	 * but don't call @c boost::enable_current_exception (which is fine because
	 * C++11 provides @c std::current_exception).
	 *
	 * @note We avoid @c boost::enable_current_exception because Boost's
	 *       implementation wraps a copy of the exception in its own class (via
	 *       inheritance), which becomes the "most derived class".  This
	 *       wrapping class calls the default constructor of its virtual bases,
	 *       causing the original @c std::exception::what string to be lost.
	 */
#	define THROW(E) \
		throw ::boost::enable_error_info(E) << \
			::boost::throw_function(BOOST_CURRENT_FUNCTION) << \
			::boost::throw_file(__FILE__) << \
			::boost::throw_line(__LINE__);

	/**
	 * Catch an @c Exception having at least the specified tags.
	 */
#	define CATCH_TAGS(PREFIX, SUFFIX, ...) \
		catch (PREFIX ::page::err::Exception<__VA_ARGS__>::Permutation SUFFIX)

#	include "tags.hpp"
#endif
