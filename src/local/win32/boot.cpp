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

#include <exception> // exception
#include <iostream> // cerr
#include <string>
#include <windows.h>
#include "../util/pp.hpp" // STRINGIZE

namespace
{
	HANDLE inReadPipe, inWritePipe,
		outReadPipe, outWritePipe,
		errReadPipe, errWritePipe;

	DWORD WINAPI InDispatch(LPVOID)
	{
		static char buffer[256];
		DWORD read;
		while (ReadFile(GetStdHandle(STD_INPUT_HANDLE), buffer, sizeof buffer, &read, 0) && read)
		{
			DWORD write;
			if (!WriteFile(inWritePipe, buffer, read, &write, 0))
				break;
		}
	}
	DWORD WINAPI OutDispatch(LPVOID)
	{
		static char buffer[256];
		DWORD read;
		while (ReadFile(outReadPipe, buffer, sizeof buffer, &read, 0) && read)
		{
			DWORD write;
			if (!WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, read, &write, 0))
				break;
		}
	}
	DWORD WINAPI ErrDispatch(LPVOID)
	{
		static char buffer[256];
		DWORD read;
		while (ReadFile(errReadPipe, buffer, sizeof buffer, &read, 0) && read)
		{
			DWORD write;
			if (!WriteFile(GetStdHandle(STD_ERROR_HANDLE), buffer, read, &write, 0))
				break;
		}
	}
}

int main()
{
	try
	{
		SECURITY_ATTRIBUTES sa = {};
		sa.nLength = sizeof sa;
		sa.bInheritHandle = TRUE;
		if (!CreatePipe(&inReadPipe, &inWritePipe, &sa, 0))
			throw std::exception();
		if (!SetHandleInformation(inWritePipe, HANDLE_FLAG_INHERIT, 0) ||
			!CreatePipe(&outReadPipe, &outWritePipe, &sa, 0))
		{
			CloseHandle(inWritePipe);
			CloseHandle(inReadPipe);
			throw std::exception();
		}
		if (!SetHandleInformation(outReadPipe, HANDLE_FLAG_INHERIT, 0) ||
			!CreatePipe(&errReadPipe, &errWritePipe, &sa, 0))
		{
			CloseHandle(outWritePipe);
			CloseHandle(outReadPipe);
			CloseHandle(inWritePipe);
			CloseHandle(inReadPipe);
			throw std::exception();
		}
		if (!SetHandleInformation(errReadPipe, HANDLE_FLAG_INHERIT, 0))
		{
			CloseHandle(errWritePipe);
			CloseHandle(errReadPipe);
			CloseHandle(outWritePipe);
			CloseHandle(outReadPipe);
			CloseHandle(inWritePipe);
			CloseHandle(inReadPipe);
			throw std::exception();
		}

		STARTUPINFO si = {};
		si.cb = sizeof si;
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdInput = inReadPipe;
		si.hStdOutput = outWritePipe;
		si.hStdError = errWritePipe;
		PROCESS_INFORMATION pi;
		if (!CreateProcess(TEXT(STRINGIZE(PACKAGE) ".exe"),
			GetCommandLine(), 0, 0, TRUE, 0, 0, 0, &si, &pi))
		{
			CloseHandle(errWritePipe);
			CloseHandle(errReadPipe);
			CloseHandle(outWritePipe);
			CloseHandle(outReadPipe);
			CloseHandle(inWritePipe);
			CloseHandle(inReadPipe);
			throw std::exception();
		}
		CloseHandle(pi.hThread);
		CloseHandle(errWritePipe);
		CloseHandle(outWritePipe);
		CloseHandle(inReadPipe);

		DWORD threadId;
		HANDLE inThread;
		if (!(inThread = CreateThread(0, 0, InDispatch, 0, 0, &threadId)))
		{
			CloseHandle(pi.hProcess);
			CloseHandle(errReadPipe);
			CloseHandle(outReadPipe);
			CloseHandle(inWritePipe);
			throw std::exception();
		}
		HANDLE outThread;
		if (!(outThread = CreateThread(0, 0, OutDispatch, 0, 0, &threadId)))
		{
			CloseHandle(inThread);
			CloseHandle(pi.hProcess);
			CloseHandle(errReadPipe);
			CloseHandle(outReadPipe);
			CloseHandle(inWritePipe);
			throw std::exception();
		}
		HANDLE errThread;
		if (!(errThread = CreateThread(0, 0, ErrDispatch, 0, 0, &threadId)))
		{
			CloseHandle(outThread);
			CloseHandle(inThread);
			CloseHandle(pi.hProcess);
			CloseHandle(errReadPipe);
			CloseHandle(outReadPipe);
			CloseHandle(inWritePipe);
			throw std::exception();
		}
		if (WaitForSingleObject(pi.hProcess, INFINITE) == WAIT_FAILED)
		{
			CloseHandle(outThread);
			CloseHandle(inThread);
			CloseHandle(pi.hProcess);
			CloseHandle(errReadPipe);
			CloseHandle(outReadPipe);
			CloseHandle(inWritePipe);
			throw std::exception();
		}
		CloseHandle(outThread);
		CloseHandle(inThread);

		DWORD status = 0;
		GetExitCodeProcess(pi.hProcess, &status);
		CloseHandle(pi.hProcess);
		CloseHandle(errReadPipe);
		CloseHandle(outReadPipe);
		CloseHandle(inWritePipe);
		return status;
	}
	catch (...)
	{
		std::cerr << "error: win32 bootloader failed" << std::endl;
		return 1;
	}
}
