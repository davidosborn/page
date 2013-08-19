#include "ScopeGuard.hpp"

namespace page
{
	namespace util
	{
		// constructor/destructor
		ScopeGuard::ScopeGuard(const Callback &cb) : cb(cb) {}
		ScopeGuard::~ScopeGuard()
		{
			if (cb) cb();
		}

		// modifiers
		void ScopeGuard::Release()
		{
			cb = nullptr;
		}
		void ScopeGuard::Reset(const Callback &cb)
		{
			if (this->cb) this->cb();
			this->cb = cb;
		}
	}
}
