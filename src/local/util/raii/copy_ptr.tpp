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
 *
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

#include "../type_traits/const_volatile.hpp" // copy_cv

namespace page
{
	namespace util
	{
		namespace detail
		{
			// explicit cloner cast adapters
			template <typename T, typename U> struct ClonerConstCastAdapter
			{
				typedef typename copy_ptr<U>::cloner_type cloner_type;

				explicit ClonerConstCastAdapter(cloner_type cloner) :
					cloner(cloner) {}

				T *operator ()(T *p) const
				{
					/**
					 * @note The inner @c const_cast is only needed for
					 *       extraneous @c const_pointer_cast calls, such as
					 *       when casting from non-const to const.
					 */
					return const_cast<T *>(cloner(const_cast<U *>(p)));
				}

				private:
				cloner_type cloner;
			};
			template <typename T, typename U> struct ClonerDynamicCastAdapter
			{
				typedef typename copy_ptr<U>::cloner_type cloner_type;

				explicit ClonerDynamicCastAdapter(cloner_type cloner) :
					cloner(cloner) {}

				T *operator ()(T *p) const
				{
					/**
					 * @note The inner @c static_cast is only needed for
					 *       extraneous @c dynamic_pointer_cast calls, such as
					 *       when casting from a derived class to its base.
					 */
					return dynamic_cast<T *>(cloner(static_cast<U *>(p)));
				}

				private:
				cloner_type cloner;
			};
			template <typename T, typename U> struct ClonerReinterpretCastAdapter
			{
				typedef typename copy_ptr<U>::cloner_type cloner_type;

				explicit ClonerReinterpretCastAdapter(cloner_type cloner) :
					cloner(cloner) {}

				T *operator ()(T *p) const
				{
					return reinterpret_cast<T *>(cloner(reinterpret_cast<U *>(p)));
				}

				private:
				cloner_type cloner;
			};
			template <typename T, typename U> struct ClonerStaticCastAdapter
			{
				typedef typename copy_ptr<U>::cloner_type cloner_type;

				explicit ClonerStaticCastAdapter(cloner_type cloner) :
					cloner(cloner) {}

				T *operator ()(T *p) const
				{
					return static_cast<T *>(cloner(static_cast<U *>(p)));
				}

				private:
				cloner_type cloner;
			};

			// explicit deleter cast adapters
			template <typename T, typename U> struct DeleterConstCastAdapter
			{
				typedef typename copy_ptr<U>::deleter_type deleter_type;

				explicit DeleterConstCastAdapter(deleter_type deleter) :
					deleter(deleter) {}

				void operator ()(T *p) const
				{
					/**
					 * @note The inner @c const_cast is only needed for
					 *       extraneous @c const_pointer_cast calls, such as
					 *       when casting from non-const to const.
					 */
					deleter(const_cast<U *>(p));
				}

				private:
				deleter_type deleter;
			};
			template <typename T, typename U> struct DeleterDynamicCastAdapter
			{
				typedef typename copy_ptr<U>::deleter_type deleter_type;

				explicit DeleterDynamicCastAdapter(deleter_type deleter) :
					deleter(deleter) {}

				void operator ()(T *p) const
				{
					/**
					 * @note The inner @c static_cast is only needed for
					 *       extraneous @c dynamic_pointer_cast calls, such as
					 *       when casting from a derived class to its base.
					 */
					deleter(static_cast<U *>(p));
				}

				private:
				deleter_type deleter;
			};
			template <typename T, typename U> struct DeleterReinterpretCastAdapter
			{
				typedef typename copy_ptr<U>::deleter_type deleter_type;

				explicit DeleterReinterpretCastAdapter(deleter_type deleter) :
					deleter(deleter) {}

				void operator ()(T *p) const
				{
					deleter(reinterpret_cast<U *>(p));
				}

				private:
				deleter_type deleter;
			};
			template <typename T, typename U> struct DeleterStaticCastAdapter
			{
				typedef typename copy_ptr<U>::deleter_type deleter_type;

				explicit DeleterStaticCastAdapter(deleter_type deleter) :
					deleter(deleter) {}

				void operator ()(T *p) const
				{
					deleter(static_cast<U *>(p));
				}

				private:
				deleter_type deleter;
			};
		}

		// default cloner/deleter
		template <typename T> struct copy_ptr<T>::DefaultCloner
		{
			T *operator ()(T *p) const
			{
				return p ? new T(*p) : static_cast<T *>(nullptr);
			}
		};
		template <typename T> struct copy_ptr<T>::DefaultDeleter
		{
			void operator ()(T *p) const
			{
				delete p;
			}
		};

		// const cloner/deleter adapters
		template <typename T> struct copy_ptr<T>::ConstClonerAdapter
		{
			explicit ConstClonerAdapter(cloner_type cloner) :
				cloner(cloner) {}

			T *operator ()(const T *p) const
			{
				return cloner(const_cast<T *>(p));
			}

			private:
			cloner_type cloner;
		};
		template <typename T> struct copy_ptr<T>::ConstDeleterAdapter
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

		// implicit cloner/deleter conversion adapters
		template <typename T> template <typename U> struct copy_ptr<T>::ClonerConversionAdapter
		{
			typedef typename copy_ptr<U>::cloner_type cloner_type;

			explicit ClonerConversionAdapter(cloner_type cloner) :
				cloner(cloner) {}

			T *operator ()(T *p) const
			{
				/**
				 * @note The @c static_cast and @c const_cast are needed to
				 *       convert @c T back into @c U (after @c U was implicitly
				 *       converted to @c T) so that the original cloner can be
				 *       called.
				 */
				return cloner(static_cast<U *>(
					const_cast<typename util::copy_cv<T, U>::type *>(p)));
			}

			private:
			cloner_type cloner;
		};
		template <typename T> template <typename U> struct copy_ptr<T>::DeleterConversionAdapter
		{
			typedef typename copy_ptr<U>::deleter_type deleter_type;

			explicit DeleterConversionAdapter(deleter_type deleter) :
				deleter(deleter) {}

			void operator ()(T *p) const
			{
				/**
				 * @note The @c static_cast and @c const_cast are needed to
				 *       convert @c T back into @c U (after @c U was implicitly
				 *       converted to @c T) so that the original deleter can be
				 *       called.
				 */
				deleter(static_cast<U *>(
					const_cast<typename util::copy_cv<T, U>::type *>(p)));
			}

			private:
			deleter_type deleter;
		};

		// constructors
		template <typename T> copy_ptr<T>::copy_ptr() :
			copy_ptr(nullptr) {}
		template <typename T> copy_ptr<T>::copy_ptr(pointer p) :
			copy_ptr(p, DefaultCloner()) {}
		template <typename T> copy_ptr<T>::copy_ptr(pointer p, cloner_type cloner) :
			copy_ptr(p, cloner, DefaultDeleter()) {}
		template <typename T> copy_ptr<T>::copy_ptr(pointer p, cloner_type cloner, deleter_type deleter) :
			p(p), cloner(cloner), deleter(deleter) {}
		/**
		 * @note The cloner defaults to @c nullptr here to allow @c copy_ptr to
		 *       be initialized as a null pointer to an abstract class, with
		 *       which the default cloner would be incompatible, failing to
		 *       compile.
		 */
		template <typename T> copy_ptr<T>::copy_ptr(std::nullptr_t) :
			copy_ptr(pointer(), nullptr) {}
		template <typename T> copy_ptr<T>::copy_ptr(std::nullptr_t, cloner_type cloner) :
			copy_ptr(pointer(), cloner) {}
		template <typename T> copy_ptr<T>::copy_ptr(std::nullptr_t, cloner_type cloner, deleter_type deleter) :
			copy_ptr(pointer(), cloner, deleter) {}
		template <typename T> copy_ptr<T>::copy_ptr(const copy_ptr &other) :
			p(other.cloner ? other.cloner(other.p) : nullptr),
			cloner (other.cloner), deleter(other.deleter) {}
		template <typename T> template <typename U> copy_ptr<T>::copy_ptr(const copy_ptr<U> &other) :
			p(other.cloner ? other.cloner(other.p) : nullptr),
			cloner(ClonerConversionAdapter<U>(other.cloner)),
			deleter(DeleterConversionAdapter<U>(other.deleter)) {}
		template <typename T> template <typename U> copy_ptr<T>::copy_ptr(std::auto_ptr<U> &&other) :
			copy_ptr(other.release()) {}
		template <typename T> template <typename U> copy_ptr<T>::copy_ptr(std::auto_ptr<U> &&other, cloner_type cloner) :
			copy_ptr(other.release(), cloner) {}
		template <typename T> template <typename U, typename D> copy_ptr<T>::copy_ptr(std::unique_ptr<U, D> &&other) :
			copy_ptr(other, DefaultCloner()) {}
		template <typename T> template <typename U, typename D> copy_ptr<T>::copy_ptr(std::unique_ptr<U, D> &&other, cloner_type cloner) :
			copy_ptr(other.release(), cloner, other.get_deleter()) {}

		// destructors
		template <typename T> copy_ptr<T>::~copy_ptr()
		{
			if (deleter) deleter(p);
		}

		// assignment
		template <typename T> copy_ptr<T> &copy_ptr<T>::operator =(copy_ptr other)
		{
			copy_ptr(other).swap(*this);
			return *this;
		}
		template <typename T> template <typename U> copy_ptr<T> &copy_ptr<T>::operator =(copy_ptr<U> other)
		{
			copy_ptr(other).swap(*this);
			return *this;
		}
		template <typename T> template <typename U> copy_ptr<T> &copy_ptr<T>::operator =(std::auto_ptr<U> &&other)
		{
			copy_ptr(other).swap(*this);
			return *this;
		}
		template <typename T> template <typename U, typename D> copy_ptr<T> &copy_ptr<T>::operator =(std::unique_ptr<U, D> &&other)
		{
			copy_ptr(other).swap(*this);
			return *this;
		}

		// observers
		template <typename T> T &copy_ptr<T>::operator *() const noexcept
		{
			assert(p);
			return *p;
		}
		template <typename T> typename copy_ptr<T>::pointer copy_ptr<T>::operator ->() const noexcept
		{
			assert(p);
			return p;
		}
		template <typename T> typename copy_ptr<T>::pointer copy_ptr<T>::get() const noexcept
		{
			return p;
		}
		template <typename T> const typename copy_ptr<T>::cloner_type &copy_ptr<T>::get_cloner() const noexcept
		{
			return cloner;
		}
		template <typename T> typename copy_ptr<T>::const_cloner_type copy_ptr<T>::get_const_cloner() const
		{
			return ConstClonerAdapter(cloner);
		}
		template <typename T> const typename copy_ptr<T>::deleter_type &copy_ptr<T>::get_deleter() const noexcept
		{
			return deleter;
		}
		template <typename T> typename copy_ptr<T>::const_deleter_type copy_ptr<T>::get_const_deleter() const
		{
			return ConstDeleterAdapter(deleter);
		}
		template <typename T> copy_ptr<T>::operator bool() const noexcept
		{
			return p != nullptr;
		}

		// modifiers
		template <typename T> typename copy_ptr<T>::pointer copy_ptr<T>::release() noexcept
		{
			pointer p = this->p;
			this->p = nullptr;
			return p;
		}
		template <typename T> void copy_ptr<T>::reset()
		{
			copy_ptr().swap(*this);
		}
		template <typename T> template <typename U> void copy_ptr<T>::reset(U *p)
		{
			copy_ptr(p).swap(*this);
		}
		template <typename T> template <typename U, typename C> void copy_ptr<T>::reset(U *p, C cloner)
		{
			copy_ptr(p, cloner).swap(*this);
		}
		template <typename T> template <typename U, typename C, typename D> void copy_ptr<T>::reset(U *p, C cloner, D deleter)
		{
			copy_ptr(p, cloner, deleter).swap(*this);
		}
		template <typename T> void copy_ptr<T>::reset(std::nullptr_t)
		{
			copy_ptr(nullptr).swap(*this);
		}
		template <typename T> template <typename C> void copy_ptr<T>::reset(std::nullptr_t, C cloner)
		{
			copy_ptr(nullptr, cloner).swap(*this);
		}
		template <typename T> template <typename C, typename D> void copy_ptr<T>::reset(std::nullptr_t, C cloner, D deleter)
		{
			copy_ptr(nullptr, cloner, deleter).swap(*this);
		}
		template <typename T> void copy_ptr<T>::swap(copy_ptr &other) noexcept
		{
			using std::swap;
			swap(p,       other.p);
			swap(cloner,  other.cloner);
			swap(deleter, other.deleter);
		}

		// relational operators
		template <typename T, typename U> bool operator < (const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept { return a.get() <  b.get(); }
		template <typename T, typename U> bool operator > (const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept { return a.get() >  b.get(); }
		template <typename T, typename U> bool operator <=(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept { return a.get() <= b.get(); }
		template <typename T, typename U> bool operator >=(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept { return a.get() >= b.get(); }
		template <typename T, typename U> bool operator ==(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept { return a.get() == b.get(); }
		template <typename T, typename U> bool operator !=(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept { return a.get() != b.get(); }

		// nullptr relational operators
		template <typename T> bool operator ==(const copy_ptr<T> &a, std::nullptr_t) noexcept { return  !a; }
		template <typename T> bool operator ==(std::nullptr_t, const copy_ptr<T> &a) noexcept { return  !a; }
		template <typename T> bool operator !=(const copy_ptr<T> &a, std::nullptr_t) noexcept { return !!a; }
		template <typename T> bool operator !=(std::nullptr_t, const copy_ptr<T> &a) noexcept { return !!a; }

		// casts
		template <typename T, typename U> copy_ptr<T> const_pointer_cast(const copy_ptr<U> &a) noexcept
		{
			return copy_ptr<T>(
				const_cast<T *>(a.get_cloner()(a.get())),
				detail::ClonerConstCastAdapter<T, U>(a.get_cloner()),
				detail::DeleterConstCastAdapter<T, U>(a.get_deleter()));
		}
		template <typename T, typename U> copy_ptr<T> dynamic_pointer_cast(const copy_ptr<U> &a) noexcept
		{
			T *p = dynamic_cast<T *>(a.get());
			detail::ClonerDynamicCastAdapter<T, U> cloner(a.get_cloner());
			return copy_ptr<T>(p ? cloner(p) : nullptr, cloner,
				detail::DeleterDynamicCastAdapter<T, U>(a.get_deleter()));
		}
		template <typename T, typename U> copy_ptr<T> reinterpret_pointer_cast(const copy_ptr<U> &a) noexcept
		{
			return copy_ptr<T>(
				reinterpret_cast<T *>(a.get_cloner()(a.get())),
				detail::ClonerReinterpretCastAdapter<T, U>(a.get_cloner()),
				detail::DeleterReinterpretCastAdapter<T, U>(a.get_deleter()));
		}
		template <typename T, typename U> copy_ptr<T> static_pointer_cast(const copy_ptr<U> &a) noexcept
		{
			return copy_ptr<T>(
				static_cast<T *>(a.get_cloner()(a.get())),
				detail::ClonerStaticCastAdapter<T, U>(a.get_cloner()),
				detail::DeleterStaticCastAdapter<T, U>(a.get_deleter()));
		}

		// stream insertion
		template <typename T, typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const copy_ptr<T> &p) noexcept
		{
			return os << p.get();
		}

		// specialized algorithms
		template <typename T> void swap(copy_ptr<T> &a, copy_ptr<T> &b) noexcept
		{
			a.swap(b);
		}
	}
}
