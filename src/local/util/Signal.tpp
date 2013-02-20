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
 *
 * @file
 *
 * Inspired by the boost/signals library.
 */

#include <algorithm> // find
#include <cassert>

#include "Caller.hpp"
#include "iterator/call_iterator.hpp"
#include "iterator/member_iterator.hpp"

namespace page
{
	namespace util
	{
		// combiners
		template <typename T> T FirstResult<T>::operator ()(const T &first, const T &) const
		{
			return first;
		}
		template <typename T> T LastResult<T>::operator ()(const T &, const T &second) const
		{
			return second;
		}

		namespace detail
		{
			// dispatcher
			// construct
			template <typename Result>
				Dispatcher<Result>::Dispatcher(const Combiner &combiner) :
				combiner(combiner) {}

			// dispatch
			template <typename Result> template <typename Iterator>
				Result Dispatcher<Result>::operator ()(Iterator first, Iterator last) const
			{
				Result result;
				if (first != last)
				{
					result = *first++;
					while (first != last)
						result = combiner(result, *first++);
				}
				return result;
			}
			template <typename Iterator>
				void Dispatcher<void>::operator ()(Iterator first, Iterator last) const
			{
				while (first != last) *first++;
			}

			// signal
			// construct
			template <typename Result, typename... Args>
				Signal<Result, Args...>::Signal(const Dispatcher<Result> &dispatcher) :
				dispatcher(dispatcher) {}

			// dispatch
			template <typename Result, typename... Args>
				Result Signal<Result, Args...>::operator ()(Args... args) const
			{
				Caller<Signature> caller(args...);
				return dispatcher(
					make_call_iterator(make_member_iterator(slots.begin(), &Slot::function), caller),
					make_call_iterator(make_member_iterator(slots.end(),   &Slot::function), caller));
			}

			// modifiers
			template <typename Result, typename... Args>
				Connection Signal<Result, Args...>::Connect(const Function &function)
			{
				Slot slot =
				{
					function,
					Handle(new DisconnectCallback)
				};
				*slot.handle = std::bind(
					&Signal<Result, Args...>::CutTheHardLine, this,
					WeakHandle(slot.handle));
				slots.push_back(slot);
				return Connection(slot.handle);
			}
			template <typename Result, typename... Args>
				void Signal<Result, Args...>::Disconnect()
			{
				slots.clear();
			}
			template <typename Result, typename... Args>
				void Signal<Result, Args...>::Disconnect(Connection &con)
			{
				con.Disconnect();
			}

			// disconnection
			template <typename Result, typename... Args>
				void Signal<Result, Args...>::CutTheHardLine(const WeakHandle &handle)
			{
				assert(!handle.expired());
				typename Slots::iterator iter(std::find(
					make_member_iterator(slots.begin(), &Slot::handle),
					make_member_iterator(slots.end(),   &Slot::handle),
					handle.lock()));
				assert(iter != slots.end());
				slots.erase(iter);
			}
		}

		// signal
		// construct
		template <typename Result, typename... Args>
			template <typename Combiner>
			Signal<Result (Args...)>::Signal(Combiner combiner) :
			detail::Signal<Result, Args...>(detail::Dispatcher<Result>(combiner)) {}
	}
}
