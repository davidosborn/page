#include <algorithm> // swap
#include <cassert>
#include <ostream> // basic_ostream
#include <utility> // forward, move

#include "../algorithm/downcast.hpp"

namespace page { namespace util
{
	namespace detail
	{
		/**
		 * The default implementation of a cloner.
		 */
		template <typename T>
			struct DefaultCloner
		{
			typedef std::unique_ptr<T> result_type;
			typedef const T &argument_type;

			std::unique_ptr<T> operator ()(const T &x) const
			{
				return std::unique_ptr<T>(new T(x));
			}
		};

		/**
		 * A null cloner.
		 */
		template <typename T>
			struct NullCloner
		{
			typedef std::unique_ptr<T> result_type;
			typedef const T &argument_type;

			std::unique_ptr<T> operator ()(const T &) const
			{
				return nullptr;
			}
		};

		/**
		 * An adaptor for cloners to support implicit pointer conversion, such as
		 * derived-to-base-pointer conversion.
		 */
		template <typename T, typename U>
			struct ClonerAdaptor
		{
			typedef std::unique_ptr<T> result_type;
			typedef const T &argument_type;

			private:
			typedef std::function<std::unique_ptr<U> (const U &)> cloner_type;

			public:
			explicit ClonerAdaptor(const cloner_type &cloner) :
				cloner(cloner) {}

			std::unique_ptr<T> operator ()(const T &x) const
			{
				return cloner(downcast<const U &>(x));
			}

			private:
			cloner_type cloner;
		};
	}

////////// copy_ptr ////////////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	template <typename T>
		constexpr copy_ptr<T>::copy_ptr() :
			copy_ptr(nullptr) {}

	template <typename T>
		constexpr copy_ptr<T>::copy_ptr(std::nullptr_t) :
			copy_ptr(static_cast<T *>(nullptr), detail::NullCloner<T>()) {}

	template <typename T>
		copy_ptr<T>::copy_ptr(T *p) :
			copy_ptr(p, detail::DefaultCloner<T>()) {}

	template <typename T>
		copy_ptr<T>::copy_ptr(T *p, const cloner_type &cloner) :
			p(p), cloner(cloner)
	{
		assert(cloner != nullptr);
	}

	/*-----------+
	| unique_ptr |
	+-----------*/

	template <typename T>
		copy_ptr<T>::copy_ptr(std::unique_ptr<T> &&other) :
			copy_ptr(std::forward<std::unique_ptr<T>>(other), detail::DefaultCloner<T>()) {}

	template <typename T>
		copy_ptr<T>::copy_ptr(std::unique_ptr<T> &&other, const cloner_type &cloner) :
			p(std::move(other)), cloner(cloner)
	{
		assert(cloner != nullptr);
	}

	template <typename T> template <typename U>
		copy_ptr<T>::copy_ptr(std::unique_ptr<U> &&other) :
			copy_ptr(std::forward<std::unique_ptr<U>>(other), detail::DefaultCloner<T>()) {}

	template <typename T> template <typename U>
		copy_ptr<T>::copy_ptr(std::unique_ptr<U> &&other, const cloner_type &cloner) :
			p(std::move(other)), cloner(cloner)
	{
		assert(cloner != nullptr);
	}

	/*template <typename T>
		copy_ptr<T>::operator std::unique_ptr<T>() && noexcept
	{
		return std::move(p);
	}*/

	/*---------------+
	| copy semantics |
	+---------------*/

	template <typename T>
		copy_ptr<T>::copy_ptr(const copy_ptr &other) :
			p(other.cloner(*other)), cloner(other.cloner) {}

	template <typename T>
		copy_ptr<T> &copy_ptr<T>::operator =(const copy_ptr &other)
	{
		p = other.cloner(*other);
		cloner = other.cloner;
		return *this;
	}

	template <typename T> template <typename U>
		copy_ptr<T>::copy_ptr(const copy_ptr<U> &other) :
			p(other.cloner(*other)),
			cloner(detail::ClonerAdaptor<T, U>(other.cloner)) {}

	template <typename T> template <typename U>
		copy_ptr<T> &copy_ptr<T>::operator =(const copy_ptr<U> &other)
	{
		p = other.cloner(*other);
		cloner = detail::ClonerAdaptor<T, U>(other.cloner);
		return *this;
	}

	/*---------------+
	| move semantics |
	+---------------*/

	template <typename T> template <typename U>
		copy_ptr<T>::copy_ptr(copy_ptr<U> &&other) :
			p(std::move(other.p)),
			cloner(detail::ClonerAdaptor<T, U>(other.cloner)) {}

	template <typename T> template <typename U>
		copy_ptr<T> &copy_ptr<T>::operator =(copy_ptr<U> &&other)
	{
		p = std::move(other.p);
		cloner = detail::ClonerAdaptor<T, U>(other.cloner);
		return *this;
	}

	/*----------+
	| observers |
	+----------*/

	template <typename T>
		T &copy_ptr<T>::operator *() const noexcept
	{
		return *p;
	}

	template <typename T>
		T *copy_ptr<T>::operator ->() const noexcept
	{
		return p.get();
	}

	template <typename T>
		T *copy_ptr<T>::get() const noexcept
	{
		return p.get();
	}

	template <typename T>
		typename copy_ptr<T>::deleter_type &copy_ptr<T>::get_deleter() noexcept
	{
		return p.get_deleter();
	}

	template <typename T>
		const typename copy_ptr<T>::deleter_type &copy_ptr<T>::get_deleter() const noexcept
	{
		return p.get_deleter();
	}

	template <typename T>
		typename copy_ptr<T>::cloner_type &copy_ptr<T>::get_cloner() noexcept
	{
		return cloner;
	}

	template <typename T>
		const typename copy_ptr<T>::cloner_type &copy_ptr<T>::get_cloner() const noexcept
	{
		return cloner;
	}

	template <typename T>
		copy_ptr<T>::operator bool() const noexcept
	{
		return p != nullptr;
	}

	/*----------+
	| modifiers |
	+----------*/

	template <typename T>
		T *copy_ptr<T>::release() noexcept
	{
		cloner = detail::NullCloner<T>();
		return p.release();
	}

	template <typename T>
		void copy_ptr<T>::reset(T *p)
	{
		std::swap(copy_ptr(p));
	}

	template <typename T>
		void copy_ptr<T>::reset(T *p, const cloner_type &cloner)
	{
		std::swap(copy_ptr(p, cloner));
	}

	/*-------------------+
	| pointer comparison |
	+-------------------*/

	template <typename T, typename U>
		bool operator ==(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept
	{
		return a.get() == b.get();
	}

	template <typename T, typename U>
		bool operator !=(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept
	{
		return a.get() != b.get();
	}

	template <typename T, typename U>
		bool operator <=(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept
	{
		return a.get() <= b.get();
	}

	template <typename T, typename U>
		bool operator >=(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept
	{
		return a.get() >= b.get();
	}

	template <typename T, typename U>
		bool operator <(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept
	{
		return a.get() < b.get();
	}

	template <typename T, typename U>
		bool operator >(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept
	{
		return a.get() > b.get();
	}

	/*-------------------+
	| nullptr comparison |
	+-------------------*/

	template <typename T>
		bool operator ==(const copy_ptr<T> &p, std::nullptr_t) noexcept
	{
		return p.get() == nullptr;
	}

	template <typename T>
		bool operator ==(std::nullptr_t, const copy_ptr<T> &p) noexcept
	{
		return p.get() == nullptr;
	}

	template <typename T>
		bool operator !=(const copy_ptr<T> &p, std::nullptr_t) noexcept
	{
		return p.get() != nullptr;
	}

	template <typename T>
		bool operator !=(std::nullptr_t, const copy_ptr<T> &p) noexcept
	{
		return p.get() != nullptr;
	}

	/*-----------------+
	| stream insertion |
	+-----------------*/

	template <typename Char, typename CharTraits, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(
			std::basic_ostream<Char, CharTraits> &os, const copy_ptr<T> &p)
	{
		return os << *p;
	}
}}
