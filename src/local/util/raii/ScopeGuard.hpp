#ifndef    page_local_util_raii_ScopeGuard_hpp
#   define page_local_util_raii_ScopeGuard_hpp

#	include <functional> // function

#	include "../class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace util
	{
		/**
		 * A generic scope-guard to ensure RAII consistency.
		 *
		 * @ingroup scope-guard
		 */
		struct ScopeGuard : Uncopyable<ScopeGuard>
		{
			typedef std::function<void ()> Callback;

			// constructor/destructor
			ScopeGuard(const Callback & = nullptr);
			~ScopeGuard();

			// modifiers
			void Release();
			void Reset(const Callback & = nullptr);

			private:
			Callback cb;
		};
	}
}

#endif
