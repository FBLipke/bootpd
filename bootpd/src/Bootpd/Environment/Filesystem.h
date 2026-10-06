/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "Environment.h"

namespace bootp
{
	class Filesystem
	{
	public:
		static _STRING __pathSeperatorChar();
		static _STRING __replaceSlash(const _STRING &p);
		static _STRING CurrentDirectory();
		static bool IsDirExist(const _STRING &path);
		static bool MakePath(const _STRING &path);
		static _STRING Combine(const _STRING &p1, const _STRING &p2);
		static _SIZET FileLength(const _STRING &file);
		static bool FileExist(const _STRING &filename);
		static _SIZET FileRead(char *dst, _SIZET length, FILE *handle);
		static _SIZET FileWrite(const _STRING &filename, const char *src, const _SIZET &length);
		static bool WriteLeaseEntry(const _STRING &filename, const _STRING &ipaddress, const _STRING &mac);
		static bool __has_endingslash(const _STRING &p);
		static bool __has_startslash(const _STRING &p);

		Filesystem();
		explicit Filesystem(const _STRING &rootDir);
		~Filesystem();

	private:
		_STRING rootDir;
	};
}
