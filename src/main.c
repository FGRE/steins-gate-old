/* 
 * steins-gate: Alternative Steins;Gate Visual Novel executable
 * Copyright (C) 2013 Mislav Blažević <krofnica996@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * */
#ifdef _WIN32
#include <windows.h>
#include <assert.h>
int __stdcall WinMain(HINSTANCE a, HINSTANCE b, char* c, int d)
{
    HINSTANCE pLib = LoadLibrary("libnpengine.dll");
    assert(pLib);
    FARPROC pMain = GetProcAddress(pLib, "NitroplusMain");
    assert(pMain);
    return pMain();
}
#else
int NitroplusMain();
int main(int argc, char** argv)
{
    return NitroplusMain();
}
#endif
