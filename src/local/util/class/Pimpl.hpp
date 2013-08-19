#ifndef    codepad_util_PimplMixin_hpp
#   define codepad_util_PimplMixin_hpp

#	include <memory> // shared_ptr
#	include <utility> // forward

#	include "Cloneable.hpp"
//#	include "Polymorphic.hpp"

namespace page { namespace util
{
	/**
	 * A mixin for the @e Pimpl idiom, providing the derived class with a
	 * private implementation.
	 *
	 * @note This class should be inherited from virtually.
	 *
	 * @see http://www.drdobbs.com/article/print?articleId=205918714
	 * @see http://herbsutter.com/gotw/_100/
	 */
	template <typename Derived, typename Impl_>
		class Pimpl// : public Polymorphic<Pimpl<Derived, Impl>>
	{
		public:
		using Impl = Impl_;

		/*-------------+
		| constructors |
		+-------------*/

		explicit Pimpl(const std::shared_ptr<Impl> &impl = nullptr) :
			impl(impl) {}

		explicit Pimpl(const Cloneable<Impl> &impl) :
			impl(impl.Clone()) {}

		/*----------+
		| observers |
		+----------*/

		protected:
		/**
		 * @return The implementation.
		 */
		Impl &GetImpl()
		{
			return impl;
		}

		/**
		 * @return The implementation.
		 */
		const Impl &GetImpl() const
		{
			return impl;
		}

		/*-----------------+
		| member variables |
		+-----------------*/

		private:
		std::shared_ptr<Impl> impl;
	};

	template <
		typename Derived, typename DerivedImpl,
		typename Base,    typename BaseImpl>
			class Pimpl2 :
				public virtual Pimpl<Base, BaseImpl>
	{
		protected:
		/**
		 * @return The implementation.
		 */
		DerivedImpl &GetImpl()
		{
			return dynamic_cast<DerivedImpl &>(
				Pimpl<Base, BaseImpl>::GetImpl());
		}

		/**
		 * @return The implementation.
		 */
		const DerivedImpl &GetImpl() const
		{
			return dynamic_cast<const DerivedImpl &>(
				Pimpl<Base, BaseImpl>::GetImpl());
		}
	};
}}

#endif
