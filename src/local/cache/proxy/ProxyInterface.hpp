#ifndef    page_local_cache_proxy_ProxyInterface_hpp
#   define page_local_cache_proxy_ProxyInterface_hpp

#	include <iosfwd> // basic_ostream
#	include <memory> // shared_ptr
#	include <string>

namespace page { namespace cache
{
	/**
	 *
	 */
	template <typename Derived, typename T>
		class ProxyInterface
	{
		/*-------+
		| traits |
		+-------*/

		public:
		/**
		 * The type of the cached object.
		 */
		using element_type = T;

		/**
		 * The type of a shared reference to the cached object.
		 */
		using pointer = std::shared_ptr<element_type>;

		/*----------+
		| observers |
		+----------*/

		/**
		 * @return *lock().
		 */
		element_type &operator *() const;

		/**
		 * @return lock().
		 */
		pointer operator ->() const;

		/**
		 * @return lock().
		 */
		pointer get() const;

		/**
		 * @return Derived::DoLock().
		 */
		pointer lock() const;

		/**
		 * @return !GetSignature().empty().
		 */
		explicit operator bool() const noexcept;

		/**
		 * @return Derived::DoGetSignature().
		 */
		const std::string &GetSignature() const noexcept;
	};

	/*---------------------+
	| signature comparison |
	+---------------------*/

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator ==(
			const ProxyInterface<D, T> &,
			const ProxyInterface<E, U> &) noexcept;

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator !=(
			const ProxyInterface<D, T> &,
			const ProxyInterface<E, U> &) noexcept;

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator <(
			const ProxyInterface<D, T> &,
			const ProxyInterface<E, U> &) noexcept;

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator >(
			const ProxyInterface<D, T> &,
			const ProxyInterface<E, U> &) noexcept;

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator <=(
			const ProxyInterface<D, T> &,
			const ProxyInterface<E, U> &) noexcept;

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator >=(
			const ProxyInterface<D, T> &,
			const ProxyInterface<E, U> &) noexcept;

	/*-----------------+
	| stream insertion |
	+-----------------*/

	/**
	 * Writes the proxy's signature to the output stream.
	 */
	template <typename Char, typename CharTraits, typename D, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(
			std::basic_ostream<Char, CharTraits> &, const ProxyInterface<D, T> &);
}}

#	include "ProxyInterface.tpp"
#endif
