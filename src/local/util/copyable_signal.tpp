#include <utility> // forward

namespace page { namespace util
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename Signature> template <typename... Args>
		copyable_signal<Signature>::copyable_signal(Args &&... args) :
			boost::signal<Signature>(std::forward<Args>(args)...) {}

	/*--------------------+
	| copy/move semantics |
	+--------------------*/

	template <typename Signature>
		copyable_signal<Signature>::copyable_signal(const copyable_signal &other) {}

	template <typename Signature>
		copyable_signal<Signature> &
		copyable_signal<Signature>::operator =(const copyable_signal &other) {}
}}
