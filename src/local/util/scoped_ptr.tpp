/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <algorithm> // swap
#include <cassert>
#include <ostream>
#include <utility> // move

namespace page
{
	namespace util
	{
		// default deleter
		template <typename T> struct scoped_ptr<T>::DefaultDeleter
		{
			void operator ()(T *p) const
			{
				delete p;
			}
		};
		template <typename T> struct scoped_ptr<T>::ConstDeleterAdapter
		{
			explicit ConstDeleterAdapter(deleter_type deleter) :
				deleter(deleter) {}

			void operator ()(const T *p) const
			{
				deleter(const_cast<T *>(p));
			}

			private:
			deleter_type deleter;
		};

		// constructors
		template <typename T> scoped_ptr<T>::scoped_ptr() :
			scoped_ptr(nullptr) {}
		template <typename T> scoped_ptr<T>::scoped_ptr(pointer p) :
			scoped_ptr(p, DefaultDeleter()) {}
		template <typename T> scoped_ptr<T>::scoped_ptr(pointer p, deleter_type deleter) :
			p(p), deleter(deleter) {}
		template <typename T> scoped_ptr<T>::scoped_ptr(std::nullptr_t) :
			scoped_ptr(pointer()) {}
		template <typename T> scoped_ptr<T>::scoped_ptr(std::nullptr_t, deleter_type deleter) :
			scoped_ptr(pointer(), deleter) {}
		template <typename T> template <typename U> scoped_ptr<T>::scoped_ptr(std::auto_ptr<U> &&other) :
			scoped_ptr(other.release()) {}
		template <typename T> template <typename U, typename D> scoped_ptr<T>::scoped_ptr(std::unique_ptr<U, D> &&other) :
			scoped_ptr(other.release(), other.get_deleter()) {}
		
		// destructors
		template <typename T> scoped_ptr<T>::~scoped_ptr()
		{
			if (deleter) deleter(p);
		}

		// assignment
		template <typename T> template <typename U> scoped_ptr<T> &scoped_ptr<T>::operator =(std::auto_ptr<U> &&other)
		{
			scoped_ptr(other).swap(*this);
			return *this;
		}
		template <typename T> template <typename U, typename D> scoped_ptr<T> &scoped_ptr<T>::operator =(std::unique_ptr<U, D> &&other)
		{
			scoped_ptr(other).swap(*this);
			return *this;
		}

		// observers
		template <typename T> T &scoped_ptr<T>::operator *() const noexcept
		{
			assert(p);
			return *p;
		}
		template <typename T> typename scoped_ptr<T>::pointer scoped_ptr<T>::operator ->() const noexcept
		{
			assert(p);
			return p;
		}
		template <typename T> typename scoped_ptr<T>::pointer scoped_ptr<T>::get() const noexcept
		{
			return p;
		}
		template <typename T> const typename scoped_ptr<T>::deleter_type &scoped_ptr<T>::get_deleter() const noexcept
		{
			return deleter;
		}
		template <typename T> typename scoped_ptr<T>::const_deleter_type scoped_ptr<T>::get_const_deleter() const
		{
			return ConstDeleterAdapter(deleter);
		}
		template <typename T> scoped_ptr<T>::operator bool() const noexcept
		{
			return p != nullptr;
		}

		// modifiers
		template <typename T> typename scoped_ptr<T>::pointer scoped_ptr<T>::release() noexcept
		{
			pointer p = this->p;
			this->p = nullptr;
			return p;
		}
		template <typename T> void scoped_ptr<T>::reset()
		{
			scoped_ptr().swap(*this);
		}
		template <typename T> template <typename U> void scoped_ptr<T>::reset(U *p)
		{
			scoped_ptr(p).swap(*this);
		}
		template <typename T> template <typename U, typename D> void scoped_ptr<T>::reset(U *p, D deleter)
		{
			scoped_ptr(p, deleter).swap(*this);
		}
		template <typename T> void scoped_ptr<T>::reset(std::nullptr_t)
		{
			scoped_ptr(nullptr).swap(*this);
		}
		template <typename T> template <typename D> void scoped_ptr<T>::reset(std::nullptr_t, D deleter)
		{
			scoped_ptr(nullptr, deleter).swap(*this);
		}
		template <typename T> void scoped_ptr<T>::swap(scoped_ptr &other) noexcept
		{
			using std::swap;
			swap(p,       other.p);
			swap(deleter, other.deleter);
		}

		// relational operators
		template <typename T, typename U> bool operator < (const scoped_ptr<T> &a, const scoped_ptr<U> &b) noexcept { return a.get() <  b.get(); }
		template <typename T, typename U> bool operator > (const scoped_ptr<T> &a, const scoped_ptr<U> &b) noexcept { return a.get() >  b.get(); }
		template <typename T, typename U> bool operator <=(const scoped_ptr<T> &a, const scoped_ptr<U> &b) noexcept { return a.get() <= b.get(); }
		template <typename T, typename U> bool operator >=(const scoped_ptr<T> &a, const scoped_ptr<U> &b) noexcept { return a.get() >= b.get(); }
		template <typename T, typename U> bool operator ==(const scoped_ptr<T> &a, const scoped_ptr<U> &b) noexcept { return a.get() == b.get(); }
		template <typename T, typename U> bool operator !=(const scoped_ptr<T> &a, const scoped_ptr<U> &b) noexcept { return a.get() != b.get(); }

		// nullptr relational operators
		template <typename T> bool operator ==(const scoped_ptr<T> &a, std::nullptr_t) noexcept { return  !a; }
		template <typename T> bool operator ==(std::nullptr_t, const scoped_ptr<T> &a) noexcept { return  !a; }
		template <typename T> bool operator !=(const scoped_ptr<T> &a, std::nullptr_t) noexcept { return !!a; }
		template <typename T> bool operator !=(std::nullptr_t, const scoped_ptr<T> &a) noexcept { return !!a; }

		// stream insertion
		template <typename T, typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const scoped_ptr<T> &p) noexcept
		{
			return os << p.get();
		}

		// specialized algorithms
		template <typename T> void swap(scoped_ptr<T> &a, scoped_ptr<T> &b) noexcept
		{
			a.swap(b);
		}
	}
}
