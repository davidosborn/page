#ifndef    page_util_iterator_deferred_iterator_hpp
#   define page_util_iterator_deferred_iterator_hpp

#	include <boost/iterator/iterator_adaptor.hpp>

namespace page { namespace util
{
	/**
	 * An iterator that returns the base iterator when it is dereferenced; this
	 * is the opposite of @c boost::indirect_iterator.
	 */
	template <typename Iterator>
		class deferred_iterator :
			public boost::iterator_adaptor<
				deferred_iterator<Iterator>, Iterator, const Iterator>
	{
		friend class boost::iterator_core_access;

		// inherit constructors
		using deferred_iterator::iterator_adaptor::iterator_adaptor;

		/// iterator_adapter implementation
		typename deferred_iterator::reference dereference() const
		{
			return this->base();
		}
	};

	/// factory functions
	/**
	 * Factory function (for convenience).
	 */
	template <typename Iterator>
		deferred_iterator<Iterator> make_deferred_iterator(const Iterator &iter)
			{ return deferred_iterator<Iterator>(iter); }
}}

#endif
