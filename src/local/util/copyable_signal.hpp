#ifndef    page_local_util_copyable_signal_hpp
#   define page_local_util_copyable_signal_hpp

#	include <boost/signal.hpp>

#	include "class/special_member_functions.hpp" // DEFINE_MOVE

namespace page { namespace util
{
	/**
	 * A wrapper around @c boost::signal to make it copyable.
	 *
	 * The connections are not copied when the signal is copied.
	 */
	template <typename Signature>
		class copyable_signal : public boost::signal<Signature>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		template <typename... Args>
			explicit copyable_signal(Args &&...);

		/*--------------------+
		| copy/move semantics |
		+--------------------*/

		/**
		 * Copies the signal.  The connections are not copied.
		 */
		copyable_signal(const copyable_signal &);

		/**
		 * Copies the signal.  The connections are not copied.
		 */
		copyable_signal &operator =(const copyable_signal &);

		/**
		 * Moves the signal.  The connections are retained.
		 */
		copyable_signal(copyable_signal &&);

		/**
		 * Moves the signal.  The connections are retained.
		 */
		copyable_signal &operator =(copyable_signal &&);
	};
}}

#	include "copyable_signal.tpp"
#endif
