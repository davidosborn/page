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

// delegation function object with bound arguments

#ifndef    page_local_util_Caller_hpp
#   define page_local_util_Caller_hpp

#	include <functional> // function

namespace page
{
	namespace util
	{
		template <typename Signature> struct Caller;
		template <typename Result>
			struct Caller<Result ()>
		{
			typedef Result result_type;
			typedef Result Signature();
			Result operator ()(const std::function<Signature> &func) const
				{ return func(); }
		};
		template <typename Result, typename Arg1>
			struct Caller<Result (Arg1)>
		{
			typedef Result result_type;
			typedef Result Signature(Arg1);
			explicit Caller(Arg1 a1) : a1(a1) {}
			Result operator ()(const std::function<Signature> &func) const
				{ return func(a1); }
			private: Arg1 a1;
		};
		template <typename Result, typename Arg1, typename Arg2>
			struct Caller<Result (Arg1, Arg2)>
		{
			typedef Result result_type;
			typedef Result Signature(Arg1, Arg2);
			Caller(Arg1 a1, Arg2 a2) : a1(a1), a2(a2) {}
			Result operator ()(const std::function<Signature> &func) const
				{ return func(a1, a2); }
			private: Arg1 a1; Arg2 a2;
		};
		template <typename Result, typename Arg1, typename Arg2, typename Arg3>
			struct Caller<Result (Arg1, Arg2, Arg3)>
		{
			typedef Result result_type;
			typedef Result Signature(Arg1, Arg2, Arg3);
			Caller(Arg1 a1, Arg2 a2, Arg3 a3) : a1(a1), a2(a2), a3(a3) {}
			Result operator ()(const std::function<Signature> &func) const
				{ return func(a1, a2, a3); }
			private: Arg1 a1; Arg2 a2; Arg3 a3;
		};
		template <typename Result, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
			struct Caller<Result (Arg1, Arg2, Arg3, Arg4)>
		{
			typedef Result result_type;
			typedef Result Signature(Arg1, Arg2, Arg3, Arg4);
			Caller(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) : a1(a1), a2(a2), a3(a3), a4(a4) {}
			Result operator ()(const std::function<Signature> &func) const
				{ return func(a1, a2, a3, a4); }
			private: Arg1 a1; Arg2 a2; Arg3 a3; Arg4 a4;
		};
		template <typename Result, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
			struct Caller<Result (Arg1, Arg2, Arg3, Arg4, Arg5)>
		{
			typedef Result result_type;
			typedef Result Signature(Arg1, Arg2, Arg3, Arg4, Arg5);
			Caller(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) : a1(a1), a2(a2), a3(a3), a4(a4), a5(a5) {}
			Result operator ()(const std::function<Signature> &func) const
				{ return func(a1, a2, a3, a4, a5); }
			private: Arg1 a1; Arg2 a2; Arg3 a3; Arg4 a4; Arg5 a5;
		};
		template <typename Result, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
			struct Caller<Result (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>
		{
			typedef Result result_type;
			typedef Result Signature(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
			Caller(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6) : a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6) {}
			Result operator ()(const std::function<Signature> &func) const
				{ return func(a1, a2, a3, a4, a5, a6); }
			private: Arg1 a1; Arg2 a2; Arg3 a3; Arg4 a4; Arg5 a5; Arg6 a6;
		};
		template <typename Result, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
			struct Caller<Result (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>
		{
			typedef Result result_type;
			typedef Result Signature(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
			Caller(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7) : a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6), a7(a7) {}
			Result operator ()(const std::function<Signature> &func) const
				{ return func(a1, a2, a3, a4, a5, a6, a7); }
			private: Arg1 a1; Arg2 a2; Arg3 a3; Arg4 a4; Arg5 a5; Arg6 a6; Arg7 a7;
		};
		template <typename Result, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
			struct Caller<Result (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>
		{
			typedef Result result_type;
			typedef Result Signature(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
			Caller(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8) : a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6), a7(a7), a8(a8) {}
			Result operator ()(const std::function<Signature> &func) const
				{ return func(a1, a2, a3, a4, a5, a6, a7, a8); }
			private: Arg1 a1; Arg2 a2; Arg3 a3; Arg4 a4; Arg5 a5; Arg6 a6; Arg7 a7; Arg8 a8;
		};
		template <typename Result, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
			struct Caller<Result (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>
		{
			typedef Result result_type;
			typedef Result Signature(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9);
			Caller(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9) : a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6), a7(a7), a8(a8), a9(a9) {}
			Result operator ()(const std::function<Signature> &func) const
				{ return func(a1, a2, a3, a4, a5, a6, a7, a8, a9); }
			private: Arg1 a1; Arg2 a2; Arg3 a3; Arg4 a4; Arg5 a5; Arg6 a6; Arg7 a7; Arg8 a8; Arg9 a9;
		};
		template <typename Result, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
			struct Caller<Result (Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>
		{
			typedef Result result_type;
			typedef Result Signature(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10);
			Caller(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5, Arg6 a6, Arg7 a7, Arg8 a8, Arg9 a9, Arg10 a10) : a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6), a7(a7), a8(a8), a9(a9), a10(a10) {}
			Result operator ()(const std::function<Signature> &func) const
				{ return func(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10); }
			private: Arg1 a1; Arg2 a2; Arg3 a3; Arg4 a4; Arg5 a5; Arg6 a6; Arg7 a7; Arg8 a8; Arg9 a9; Arg10 a10;
		};
	}
}

#endif
