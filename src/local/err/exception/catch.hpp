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

#ifndef    page_local_err_exception_catch_hpp
#   define page_local_err_exception_catch_hpp

#	include <exception> // exception
#	include <iostream> // clog
#	include <string>
#	include "../../log/manip.hpp" // Warn
#	include "../msg.hpp" // Message
#	include "BasicException.hpp"
#	include "CatchableException.hpp"

#	define CAUGHT_EXCEPTION e
#	define CAUGHT_EXCEPTION_WHAT CAUGHT_EXCEPTION.what()

#	define PRINT_ERROR ::page::err::Message(CAUGHT_EXCEPTION_WHAT);
#	define PRINT_WARNING ::std::clog << ::page::log::Warn << CAUGHT_EXCEPTION_WHAT << ::std::endl;

#	define CATCH_ALL_AND_PRINT_ERROR           CATCH_ALL_AND_PRINT_ERROR_AND(;)
#	define CATCH_ALL_AND_PRINT_ERROR_AND(DO)   CATCH_ALL_AND(PRINT_ERROR DO)
#	define CATCH_ALL_AND_PRINT_WARNING         CATCH_ALL_AND_PRINT_WARNING_AND(;)
#	define CATCH_ALL_AND_PRINT_WARNING_AND(DO) CATCH_ALL_AND(PRINT_WARNING DO)
#	define CATCH_ALL_AND(DO) \
		catch (const ::page::err::BasicException &CAUGHT_EXCEPTION) \
		{ \
			DO \
		} \
		catch (const ::std::exception &CAUGHT_EXCEPTION) \
		{ \
			DO \
		} \
		catch (...) \
		{ \
			const ::page::err::BasicException CAUGHT_EXCEPTION; \
			DO \
		}

#	define CATCH_TAGS(...) \
		catch (const ::page::err::CatchableException<__VA_ARGS__> &CAUGHT_EXCEPTION)

#	include "tags.hpp"
#endif
