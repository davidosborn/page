#ifndef    page_local_util_raii_copy_ptr_hpp
#   define page_local_util_raii_copy_ptr_hpp

#	include <cstddef> // nullptr_t
#	include <functional> // function
#	include <iosfwd> // basic_ostream
#	include <memory> // unique_ptr

namespace page { namespace util
{
	/**
	 * A smart pointer with copy semantics and a customizable cloner.
	 */
	template <typename T>
		class copy_ptr
	{
		template <typename>
			friend class copy_ptr;

		/*------+
		| types |
		+------*/

		public:
		/**
		 * The type of the pointed-to value.
		 */
		using element_type = T;

		/**
		 * The deleter type.
		 */
		using deleter_type = typename std::unique_ptr<T>::deleter_type;

		/**
		 * The cloner type.
		 */
		using cloner_type = std::function<std::unique_ptr<T> (const T &)>;

		/*-------------+
		| constructors |
		+-------------*/

		constexpr copy_ptr();
		constexpr copy_ptr(std::nullptr_t);

		explicit copy_ptr(T *);
		copy_ptr(T *, const cloner_type &);

		/*-----------+
		| unique_ptr |
		+-----------*/

		explicit copy_ptr(std::unique_ptr<T> &&);
		copy_ptr(std::unique_ptr<T> &&, const cloner_type &);

		template <typename U>
			explicit copy_ptr(std::unique_ptr<U> &&);

		template <typename U>
			copy_ptr(std::unique_ptr<U> &&, const cloner_type &);

		/**
		 * Moves the pointee into a @c std::unique_pointer.
		 *
		 * @fixme rvalue references for @c *this are not supported in GCC yet.
		 */
		//operator std::unique_ptr<T>() && noexcept;

		/*---------------+
		| copy semantics |
		+---------------*/

		copy_ptr(const copy_ptr &);
		copy_ptr &operator =(const copy_ptr &);

		template <typename U>
			copy_ptr(const copy_ptr<U> &);

		template <typename U>
			copy_ptr &operator =(const copy_ptr<U> &);

		/*---------------+
		| move semantics |
		+---------------*/

		copy_ptr(copy_ptr &&) = default;
		copy_ptr &operator =(copy_ptr &&) = default;

		template <typename U>
			copy_ptr(copy_ptr<U> &&);

		template <typename U>
			copy_ptr &operator =(copy_ptr<U> &&);

		/*----------+
		| observers |
		+----------*/

		T &operator *() const noexcept;
		T *operator ->() const noexcept;
		T *get() const noexcept;
		deleter_type &get_deleter() noexcept;
		const deleter_type &get_deleter() const noexcept;
		cloner_type &get_cloner() noexcept;
		const cloner_type &get_cloner() const noexcept;
		explicit operator bool() const noexcept;

		/*----------+
		| modifiers |
		+----------*/

		T *release() noexcept;
		void reset(T *);
		void reset(T *, const cloner_type &);

		/*-------------+
		| data members |
		+-------------*/

		private:
		std::unique_ptr<T> p;
		cloner_type cloner;
	};

	/*-------------------+
	| pointer comparison |
	+-------------------*/

	template <typename T, typename U>
		bool operator ==(const copy_ptr<T> &, const copy_ptr<U> &) noexcept;

	template <typename T, typename U>
		bool operator !=(const copy_ptr<T> &, const copy_ptr<U> &) noexcept;

	template <typename T, typename U>
		bool operator <=(const copy_ptr<T> &, const copy_ptr<U> &) noexcept;

	template <typename T, typename U>
		bool operator >=(const copy_ptr<T> &, const copy_ptr<U> &) noexcept;

	template <typename T, typename U>
		bool operator <(const copy_ptr<T> &, const copy_ptr<U> &) noexcept;

	template <typename T, typename U>
		bool operator >(const copy_ptr<T> &, const copy_ptr<U> &) noexcept;

	/*-------------------+
	| nullptr comparison |
	+-------------------*/

	template <typename T>
		bool operator ==(const copy_ptr<T> &, std::nullptr_t) noexcept;

	template <typename T>
		bool operator ==(std::nullptr_t, const copy_ptr<T> &) noexcept;

	template <typename T>
		bool operator !=(const copy_ptr<T> &, std::nullptr_t) noexcept;

	template <typename T>
		bool operator !=(std::nullptr_t, const copy_ptr<T> &) noexcept;

	/*-----------------+
	| stream insertion |
	+-----------------*/

	template <typename Char, typename CharTraits, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(
			std::basic_ostream<Char, CharTraits> &, const copy_ptr<T> &);
}}

#	include "copy_ptr.tpp"
#endif
