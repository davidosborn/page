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

#ifndef    page_local_util_StateSaver_hpp
#   define page_local_util_StateSaver_hpp

#	include "NonCopyable.hpp"

namespace page
{
	namespace util
	{
		/**
		 * A state-saving scope-guard to ensure RAII consistency.
		 *
		 * Derived classes must implement the following private member
		 * functions.
		 * @code
		 * void Save(); // saves the state of the guarded resource
		 * void Load(); // restores the state of the guarded resource
		 * @endcode
		 *
		 * Derived classes should also provide the following public member
		 * functions.
		 * @code
		 * Derived();            // should do nothing but call Derived::Reset()
		 * Derived(resource);    // should do nothing but call Derived::Reset(xxx)
		 * void Reset();         // must call Base::Reset()
		 * void Reset(resource); // should store some kind of reference to the
		 *                       // resource, and must call Base::Reset(true)
		 * @endcode
		 *
		 * Derived classes must also declare this class to be a friend.
		 *
		 * @ingroup scope-guard
		 *
		 * @sa See ::page::res::PipeLocker for an example of how to inherit from
		 *     this class.
		 * @sa See http://drdobbs.com/184403758 for more information about the
		 *     scope-guard concept.
		 */
		template <typename Derived> struct StateSaver : NonCopyable
		{
			// constructor/destructor
			explicit StateSaver();
			virtual ~StateSaver();

			// modifiers
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

			private:
			bool enabled;
		};
	}
}

#	include "StateSaver.tpp"
#endif
