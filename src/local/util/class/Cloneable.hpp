/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_local_util_class_Cloneable_hpp
#   define page_local_util_class_Cloneable_hpp

#	include <memory> // unique_ptr
#	include <type_traits> // is_{base_of,same,void}

#	include <boost/mpl/back_inserter.hpp>
#	include <boost/mpl/copy.hpp>
#	include <boost/mpl/eval_if.hpp>
#	include <boost/mpl/identity.hpp>
#	include <boost/mpl/if.hpp>
#	include <boost/mpl/remove_if.hpp>
#	include <boost/mpl/unique.hpp>
#	include <boost/mpl/unpack.hpp>
#	include <boost/mpl/vector.hpp>

#	include "../raii/copy_ptr.hpp"
#	include "../type_traits/sfinae.hpp" // DEFINE_SFINAE_TYPE_TRAIT
#	include "copy_move.hpp" // DEFINE_{COPY,MOVE}

namespace page { namespace util
{
	template <typename, typename...>
		class Cloneable;

////////////////////////////////////////////////////////////////////////////////

	namespace detail
	{
		/**
		 * An intermediate class for deriving new cloneable classes from
		 * multiple base classes.
		 *
		 * @note This could be replaced with @c PublicVirtualInheritor if the
		 *       compiler supported it.
		 */
		template <typename... Bases>
			class CloneableInheritor :
				public virtual Bases... {};

		/**
		 * A metafunction which returns the specialization of @c Cloneable that
		 * makes the specified class cloneable, which is its direct base class.
		 */
		template <typename Derived>
			struct GetCloneableSpecialization
		{
			typedef
				typename boost::mpl::unpack<Cloneable>::apply<
					typename boost::mpl::copy<
						typename Derived::cloneable_bases,
						boost::mpl::back_inserter<
							boost::mpl::vector<
								typename Derived::cloned_type
								>
							>
						>::type
					>::type
				type;
		};
	}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * A metafunction which determines whether a type is cloneable.
	 */
	DEFINE_SFINAE_TYPE_TRAIT_1(is_cloneable,
		typename std::enable_if<(
			std::is_base_of<
				Cloneable<typename T::cloneable_type>,
				T
				>::value
			)>::type)

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Makes the derived class cloneable, which means it has the @c Clone
	 * and @c Copy member functions declared here.  This specialization is
	 * for abstract classes.
	 *
	 * @tparam Derived The abstract class that is to be cloneable, which
	 *         directly inherits from this class.
	 *
	 * @note You should use @c virtual inheritance when deriving from this
	 *       class.
	 */
	template <typename Derived>
		class Cloneable<Derived>
	{
		/*------+
		| types |
		+------*/

		public:
		/**
		 * The abstract class of which this class is a direct base.
		 */
		typedef Derived cloneable_type;

		/**
		 * The concrete class that is produced by @c Clone.
		 */
		typedef Derived cloned_type;

		/**
		 * The direct base classes of @c cloned_type that are cloneable.
		 */
		typedef boost::mpl::vector<> cloneable_bases;

		/*-------------------------+
		| special member functions |
		+-------------------------*/

		/**
		 * It is necessary to explicitly declare the default constructor
		 * because it will not be provided by default.
		 */
		Cloneable() = default;

		/**
		 * It is necessary to explicitly declare the copy constructor and
		 * assignment operator because they will not be provided by default.
		 */
		DEFINE_COPY(Cloneable, default)

		/**
		 * It is necessary to explicitly declare the move constructor and
		 * assignment operator because they will not be provided by default,
		 * since the class has a user-declared destructor
		 * (ISO/IEC N3242 12.8.10).
		 */
		DEFINE_MOVE(Cloneable, default)

		/**
		 * It is necessary to declare the destructor to be virtual to ensure
		 * that the destructor of the derived class will be called.
		 *
		 * @note Under GCC 4.7.1, the default destructor has a noexcept
		 *       exception specification.
		 *
		 * @note A loose exception specification is used here to permit
		 *       derived classes to also have a loose exception
		 *       specification.
		 */
		virtual ~Cloneable();

		/*------------------+
		| cloning functions |
		+------------------*/

		/**
		 * Creates a new copy of the object.
		 *
		 * @note Calls @c DoClone().
		 */
		std::unique_ptr<Derived> Clone() const final
		{
			return std::unique_ptr<Derived>(DoClone());
		}

		private:
		/**
		 * Creates a new copy of the object.
		 *
		 * @note This function exists to get around GCC bug #56636.
		 */
		virtual Derived *DoClone() const = 0;

		public:
		/**
		 * Creates a new copy of the object, wrapped in a copyable smart-pointer
		 * which uses @c Clone to implement further copying.
		 */
		copy_ptr<Derived> Copy() const
		{
			return copy_ptr<Derived>(Clone(),
				[](const Derived &x) { return x.Clone(); });
		}
	};

////////////////////////////////////////////////////////////////////////////////

	/**
	 * Causes the derived class to override the implementation of its cloneable
	 * base classes.
	 *
	 * @tparam Derived The concrete class that is to override its cloneable base
	 *         classes, which directly inherits from this class.
	 *
	 * @tparam Bases The cloneable base classes that are implemented by the
	 *         derived class.  Classes that are not cloneable are ignored.
	 *
	 * @note You should use @c virtual inheritance when deriving from this
	 *       class.
	 */
	template <typename Derived, typename... Bases>
		class Cloneable :
			boost::mpl::unpack<detail::CloneableInheritor>::apply<
				typename boost::mpl::unique<
					typename boost::mpl::remove_if<
						boost::mpl::vector<
							typename boost::mpl::eval_if<
								is_cloneable<Bases>,
								detail::GetCloneableSpecialization<Bases>,
								boost::mpl::identity<void>
								>::type...
							>,
						std::is_void<boost::mpl::_1>
						>::type,
					std::is_same<boost::mpl::_1, boost::mpl::_2>
					>::type
				>::type
	{
		/*------+
		| types |
		+------*/

		public:
		typedef typename Cloneable::cloneable_type cloneable_type;

		typedef Derived cloned_type;

		typedef
			typename boost::mpl::unique<
				typename boost::mpl::remove_if<
					boost::mpl::vector<
						typename boost::mpl::if_<
							is_cloneable<Bases>,
							Bases,
							void
							>::type...
						>,
					std::is_void<boost::mpl::_1>
					>::type,
				std::is_same<boost::mpl::_1, boost::mpl::_2>
				>::type
			cloneable_bases;

		/*------------------+
		| cloning functions |
		+------------------*/

		private:
		/**
		 * An implementation of @c Cloneable<Base>::Clone which uses
		 * @c Derived's copy constructor.
		 */
		cloneable_type *DoClone() const override
		{
			return new Derived(dynamic_cast<const Derived &>(*this));
		}
	};
}}

#endif
