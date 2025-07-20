/*
* Nirvana mock library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
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
#ifndef NIRVANA_MOCK_EXPORT_H_
#define NIRVANA_MOCK_EXPORT_H_
#pragma once

#ifdef COREMOCK_BUILD

#ifdef _MSC_VER
#define NIRVANA_MOCK_EXP __declspec(dllexport)
#else
#define NIRVANA_MOCK_EXP __attribute__ ((visibility ("default")))
#endif

#else

#ifdef _MSC_VER
#define NIRVANA_MOCK_EXP __declspec(dllimport)
#else
#define NIRVANA_MOCK_EXP
#endif

#endif

#define NIRVANA_MOCK_EXPORT extern "C" NIRVANA_MOCK_EXP

#endif
