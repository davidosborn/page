/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_util_StateSaver_hpp
#   define page_local_util_StateSaver_hpp

#	include "class/special_member_functions.hpp" // Polymorphic, Uncopyable

namespace page { namespace util
{
	/**
	 * A state-saving scope-guard to ensure RAII consistency.
	 *
	 * @par Requirements for derived classes
	 * Derived classes must implement the following private member
	 * functions.
	 *
	 * @par
	 * <dl>
	 *     <dt>
	 *         <tt>void Save()</tt>
	 *     </dt><dd>
	 *         Saves the state of the guarded resource.
	 *     </dd><dt>
	 *         <tt>void Load()</tt>
	 *     </dt><dd>
	 *         Restores the state of the guarded resource.
	 *     </dd>
	 * </dl>
	 *
	 * @par
	 * Derived classes should also provide the following public member
	 * functions.
	 *
	 * @par
	 * <dl>
	 *     <dt>
	 *         <tt>Derived()</tt>
	 *     </dt><dd>
	 *         Should do nothing but call Derived::Reset().
	 *     </dd><dt>
	 *         <tt>Derived(resource)</tt>
	 *     </dt><dd>
	 *         Should do nothing but call Derived::Reset(resource).
	 *     </dd><dt>
	 *         <tt>void Reset()</tt>
	 *     </dt><dd>
	 *         Must call Base::Reset().
	 *     </dd><dt>
	 *         <tt>void Reset(resource)</tt>
	 *     </dt><dd>
	 *         Should store some kind of reference to the resource, and must
	 *         call @c Base::Reset(true).
	 *     </dd>
	 * </dl>
	 *
	 * @par
	 * Derived classes must also declare this class to be a friend.
	 *
	 * @ingroup scope-guard
	 *
	 * @sa See @c res::PipeLocker for an example of how to inherit from this
	 *     class.
	 * @sa See http://drdobbs.com/184403758 for more information about the
	 *     scope-guard concept.
	 */
	template <typename Derived>
		class StateSaver :
			public Polymorphic<StateSaver>,
			public Uncopyable<StateSaver>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		/**
		 * Calls Restore().
		 */
		~StateSaver();

		/*----------+
		| modifiers |
		+----------*/

		/**
		 * Release the guarded resource from state-saver control without
		 * restoring its state.
		 */
		void Release();

		/**
		 * Restore the guarded resource to its saved state.
		 *
		 * @note The resource remains under state-saver control after this
		 *       function is called, which means that this function can be
		 *       called repeatedly to continuously restore the resource to
		 *       its saved state.
		 */
		void Restore();

		/**
		 * Reset the guarding state of the state saver.
		 */
		void Reset(bool enabled = false);

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * @c true if the state has been saved.
		 */
		bool enabled = false;
	};
}}

#	include "StateSaver.tpp"
#endif
