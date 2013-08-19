#include "../../err/Exception.hpp"

namespace page { namespace util
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename T>
		factory_ptr<T>::factory_ptr(std::nullptr_t) noexcept {}

	template <typename T>
		factory_ptr<T>::factory_ptr(const factory_function &f) :
			f(f) {}

	template <typename T> template <typename U>
		factory_ptr<T>::factory_ptr(const factory_ptr<U> &other) :
			f(other.f) {}

	/*----------+
	| observers |
	+----------*/

	template <typename T>
		auto factory_ptr<T>::operator *() const -> element_type &
	{
		assert(*this);
		return *lock();
	}

	template <typename T>
		auto factory_ptr<T>::operator ->() const -> pointer
	{
		return lock();
	}

	template <typename T>
		auto factory_ptr<T>::get() const -> pointer
	{
		return lock();
	}

	template <typename T>
		auto factory_ptr<T>::lock() const -> pointer
	{
		if (*this) return f();
		THROW(std::bad_weak_ptr());
	}

	template <typename T>
		factory_ptr<T>::operator bool() const noexcept
	{
		return f;
	}
}}
