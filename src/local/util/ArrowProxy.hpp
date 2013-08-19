#ifndef    page_local_util_ArrowProxy_hpp
#   define page_local_util_ArrowProxy_hpp

namespace page { namespace util
{
	/**
	 * A class which can be used to return a temporary value from <tt>operator
	 * -></tt>.
	 */
	template <typename T>
		class ArrowProxy
	{
		public:
		ArrowProxy(const T &);

		const T *operator ->() const;

		private:
		const T value;
	};
}}

#	include "ArrowProxy.tpp"
#endif
