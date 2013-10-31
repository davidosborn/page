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
#	include "special_member_functions.hpp" // Polymorphic

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
		 * @todo This could be replaced with @c PublicVirtualInheritor if the
		 *       compiler supported it.
		 */
		template <typename... Bases>
			class CloneableInheritor :
				public virtual Bases... {};

		/**
		 * A metafunction which returns the most-derived specialization of @c
		 * Cloneable that is a base of @c Derived.
		 *
		 * @tparam Derived A class that is derived from @c Cloneable.
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
	 * Makes the derived class cloneable, which means that it has the @c Clone
	 * and @c Copy member functions declared here, and it is polymorphic.
	 *
	 * This specialization is for top-level base classes.
	 *
	 * @tparam Derived The abstract class that is to be cloneable, which
	 *         directly inherits from this class.
	 *
	 * @note You should use @c virtual inheritance when deriving from this
	 *       class.
	 */
	template <typename Derived>
		class Cloneable<Derived> : public Polymorphic<Cloneable<Derived>>
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

		/*------------------+
		| cloning functions |
		+------------------*/

		/**
		 * Creates a new copy of the object.
		 *
		 * @note Calls @c DoClone().
		 */
		std::unique_ptr<Derived> Clone() const
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
	 * @tparam Bases The cloneable direct-base-classes that are implemented by
	 *         the derived class.  Classes that are not cloneable are ignored.
	 *
	 * @note You should use @c virtual inheritance when deriving from this
	 *       class.
	 */
	template <typename Derived, typename... Bases>
		class Cloneable :
			public boost::mpl::unpack<detail::CloneableInheritor>::apply<
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
		 * An implementation of @c Cloneable<Base>::Clone which uses @c
		 * Derived's copy constructor.
		 */
		cloneable_type *DoClone() const override
		{
			return new Derived(dynamic_cast<const Derived &>(*this));
		}
	};
}}

	/**
	 * Implements the Cloneable interface for a derived class using its copy
	 * constructor.
	 */
#	define IMPLEMENT_CLONEABLE(DERIVED, BASE) \
		BASE *DoClone() const override \
		{ \
			return new DERIVED(*this); \
		}


#endif
