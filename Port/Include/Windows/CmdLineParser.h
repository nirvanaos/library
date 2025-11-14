/// \file
/*
* Nirvana Core. Windows port library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIRVANA_WINDOWS_CMDLINEPARSER_H_
#define NIRVANA_WINDOWS_CMDLINEPARSER_H_
#pragma once

#include <Nirvana/Allocator.h>

#ifdef _MSC_VER
#define NIRVANA_CRTL_IMPORT __declspec(dllimport)
#else
#define NIRVANA_CRTL_IMPORT
#endif

extern "C" NIRVANA_CRTL_IMPORT wchar_t* __stdcall GetCommandLineW (void);
extern "C" NIRVANA_CRTL_IMPORT wchar_t** __stdcall CommandLineToArgvW (const wchar_t* lpCmdLine, int* pNumArgs);
extern "C" NIRVANA_CRTL_IMPORT void* __stdcall LocalFree (void* hMem);
extern "C" NIRVANA_CRTL_IMPORT wchar_t* __stdcall GetEnvironmentStringsW (void);
extern "C" NIRVANA_CRTL_IMPORT int __stdcall FreeEnvironmentStringsW (wchar_t* penv);

extern "C" NIRVANA_CRTL_IMPORT int __stdcall WideCharToMultiByte (
	unsigned int CodePage,
	unsigned long dwFlags,
	const wchar_t* lpWideCharStr,
	int cchWideChar,
	char* lpMultiByteStr,
	int cbMultiByte,
	const char* lpDefaultChar,
	int* lpUsedDefaultChar
);

#define CP_UTF8                   65001       // UTF-8 translation
#define WC_ERR_INVALID_CHARS      0x00000080  // error for invalid chars
#define WC_NO_BEST_FIT_CHARS      0x00000400  // do not use best fit chars

namespace Nirvana {
namespace Windows {

/// @brief Command line parser for Windows applications.
template <template <class C> class Allocator = Nirvana::Allocator>
class CmdLineParser
{
public:
	CmdLineParser ()
	{
		size_t ccnt;
		wchar_t** argv = get_argv (ccnt);
		int ptr_cnt = argc_;
		try {
			char* buf;
			char** uarg = allocate_and_convert_argv (ptr_cnt, argv, ccnt, buf);
		} catch (...) {
			LocalFree (argv);
			throw;
		}
		LocalFree (argv);
	}

	~CmdLineParser ()
	{
		Allocator <char> ().deallocate ((char*)argv_, cb_);
	}

	char** argv () const
	{
		return argv_;
	}

	int argc () const
	{
		return argc_;
	}

protected:
	static size_t to_utf8 (const wchar_t* ws, char* us = nullptr, size_t cb = 0)
	{
		return WideCharToMultiByte (CP_UTF8, WC_ERR_INVALID_CHARS | WC_NO_BEST_FIT_CHARS, ws, -1, us, (int)cb, nullptr, nullptr);
	}

	wchar_t** get_argv (size_t& ccnt)
	{
		wchar_t** argv = CommandLineToArgvW (GetCommandLineW (), &argc_);
		ccnt = 0;
		for (wchar_t** arg = argv, **end = argv + argc_; arg != end; ++arg) {
			ccnt += to_utf8 (*arg);
		}
		return argv;
	}

	char** allocate_and_convert_argv (int ptr_cnt, wchar_t** argv, size_t& ccnt, char*& buf)
	{
		cb_ = ptr_cnt * sizeof (char*) + ccnt;
		char** uarg = argv_ = (char**)Allocator <char> ().allocate (cb_);
		buf = (char*)(uarg + ptr_cnt);
		for (wchar_t** arg = argv, **end = argv + argc_; arg != end; ++arg, ++uarg) {
			*uarg = buf;
			size_t cb = to_utf8 (*arg, buf, ccnt);
			buf += cb;
			ccnt -= cb;
		}
		return uarg;
	}

protected:
	char** argv_;
	size_t cb_;
	int argc_;
};

/// @brief Command line parser for Windows applications.
template <template <class C> class Allocator = Nirvana::Allocator>
class CmdLineParserEnv : public CmdLineParser <Allocator>
{
	using Base = CmdLineParser <Allocator>;

public:
	CmdLineParserEnv ()
	{
		size_t ccnt;
		wchar_t** argv = Base::get_argv (ccnt);
		int env_cnt = 0;
		wchar_t* env = GetEnvironmentStringsW ();
		for (wchar_t* p = env; *p; p += wcslen (p) + 1) {
			++env_cnt;
			ccnt += Base::to_utf8 (p);
		}
		int ptr_cnt = Base::argc_ + env_cnt + 1;
		try {
			char* buf;
			char** uarg = Base::allocate_and_convert_argv (ptr_cnt, argv, ccnt, buf);
			for (wchar_t* p = env; *p; p += wcslen (p), ++uarg) {
				*uarg = buf;
				size_t cb = Base::to_utf8 (p, buf, ccnt);
				buf += cb;
				ccnt -= cb;
			}
			*uarg = nullptr;
		} catch (...) {
			LocalFree (argv);
			FreeEnvironmentStringsW (env);
			throw;
		}
		LocalFree (argv);
		FreeEnvironmentStringsW (env);
	}

	char** envp () const
	{
		return Base::argv_+ Base::argc_;
	}
};

}
}

#endif
