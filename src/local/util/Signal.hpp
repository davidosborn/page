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
 *
 * @file
 *
 * Inspired by the boost/signals library.
 */

#ifndef    page_local_util_Signal_hpp
#   define page_local_util_Signal_hpp

#	include <functional> // function
#	include <memory> // {shared,weak}_ptr
#	include <vector>
#	include "NonCopyable.hpp"

namespace page
{
	namespace util
	{
		// combiners
		template <typename T> struct FirstResult
		{
			T operator ()(const T &, const T &) const;
		};
		template <typename T> struct LastResult
		{
			T operator ()(const T &, const T &) const;
		};

		// connections
		struct Connection
		{
			typedef std::function<void ()> DisconnectCallback;
			typedef std::weak_ptr<DisconnectCallback> Handle;

			// construct
			explicit Connection(const Handle & = Handle());

			// modifiers
			void Disconnect();
			void Swap(Connection &);

			// validity
			operator bool() const;

			private:
			Handle handle;
		};
		struct ScopedConnection : NonCopyable
		{
			// construct/destroy
			explicit ScopedConnection(const Connection & = Connection());
			~ScopedConnection();

			// modifiers
			Connection Release();
			void Reset();
			void Reset(const Connection &);
			void Swap(ScopedConnection &);

			private:
			Connection con;
		};

		// specialized algorithms
		void swap(Connection &, Connection &);
		void swap(ScopedConnection &, ScopedConnection &);

		namespace detail
		{
			// dispatcher
			template <typename Result> struct Dispatcher
			{
				typedef std::function<Result (Result, Result)> Combiner;

				// construct
				explicit Dispatcher(const Combiner & = LastResult<Result>());

				// dispatch
				template <typename Iterator>
					Result operator ()(Iterator first, Iterator last) const;

				private:
				Combiner combiner;
			};
			template <> struct Dispatcher<void>
			{
				// dispatch
				template <typename Iterator>
					void operator ()(Iterator first, Iterator last) const;
			};

			// signal
			template <typename Result, typename... Args> struct Signal
			{
				typedef Result Signature(Args...);
				typedef std::function<Signature> Function;

				// construct
				explicit Signal(const Dispatcher<Result> & = Dispatcher<Result>());

				// dispatch
				Result operator ()(Args...) const;

				// modifiers
				Connection Connect(const Function &);
				void Disconnect();
				void Disconnect(Connection &);

				private:
				typedef std::function<void ()> DisconnectCallback;
				typedef std::shared_ptr<DisconnectCallback> Handle;
				typedef std::weak_ptr<DisconnectCallback> WeakHandle;

				// disconnection
				// NOTE: need weak_ptr to break function binding cycle
				void CutTheHardLine(const WeakHandle &); // its a trap!

				Dispatcher<Result> dispatcher;
				struct Slot
				{
					Function function;
					Handle handle;
				};
				typedef std::vector<Slot> Slots;
				Slots slots;
			};
		}

		// signal
		template <typename> class Signal;
		template <typename R, typename... ArgTypes>
			struct Signal<R (ArgTypes...)> : detail::Signal<R, ArgTypes...>
		{
			// construct
			template <typename Combiner = LastResult<R>>
				explicit Signal(Combiner = Combiner());
		};
		template <typename... ArgTypes>
			struct Signal<void (ArgTypes...)> : detail::Signal<void, ArgTypes...> {};
	}
}

#	include "Signal.tpp"
#endif
