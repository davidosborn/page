#ifndef    page_local_util_copyable_signal_hpp
#   define page_local_util_copyable_signal_hpp

#	include <boost/signal.hpp>

#	include "class/special_member_functions.hpp" // DEFINE_MOVE

namespace page { namespace util
{
	/**
	 * A wrapper around @c boost::signal to make it copyable.
	 */
	template <typename Signature>
		class copyable_signal : public boost::signal<Signature>
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		public:
		template <typename... Args>
			explicit copyable_signal(Args &&...);

		/*----------------------+
		| copy & move semantics |
		+----------------------*/

		/**
		 * Does nothing; the connections are not copied.
		 */
		copyable_signal(const copyable_signal &);

		/**
		 * @copydoc copyable_signal(const copyable_signal &)
		 */
		copyable_signal &operator =(const copyable_signal &);

		DEFINE_MOVE(copyable_signal, default)
	};
}}

#	include "copyable_signal.tpp"
#endif
