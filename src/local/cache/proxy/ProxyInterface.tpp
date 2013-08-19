#include <ostream> // basic_ostream

namespace page { namespace cache
{
	template <typename D, typename T>
		auto ProxyInterface<D, T>::operator *() const -> element_type &
	{
		return *lock();
	}

	template <typename D, typename T>
		auto ProxyInterface<D, T>::operator ->() const -> pointer
	{
		return lock();
	}

	template <typename D, typename T>
		auto ProxyInterface<D, T>::get() const -> pointer
	{
		return lock();
	}

	template <typename D, typename T>
		auto ProxyInterface<D, T>::lock() const -> pointer
	{
		auto r(static_cast<const D &>(*this).DoLock());
		if (r == nullptr)
			THROW(std::bad_weak_ptr());
		return r;
	}

	template <typename D, typename T>
		ProxyInterface<D, T>::operator bool() const noexcept
	{
		return !GetSignature().empty();
	}

	template <typename D, typename T>
		const Signature &ProxyInterface<D, T>::GetSignature() const noexcept
	{
		return static_cast<const D &>(*this).DoGetSignature();
	}

	/*-------------------+
	| pointer comparison |
	+-------------------*/

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator ==(
			const ProxyInterface<D, T> &a,
			const ProxyInterface<E, U> &b) noexcept
	{
		return a.GetSignature() == b.GetSignature();
	}

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator !=(
			const ProxyInterface<D, T> &a,
			const ProxyInterface<E, U> &b) noexcept
	{
		return a.GetSignature() != b.GetSignature();
	}

	/*-----------------+
	| stream insertion |
	+-----------------*/

	template <typename Char, typename CharTraits, typename D, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(
			std::basic_ostream<Char, CharTraits> &os, const ProxyInterface<D, T> &proxy)
	{
		return os << proxy.GetSignature();
	}
}}
