/* 
 * steins-gate: Open source implementation of Steins;Gate Visual Novel
 * Copyright (C) 2014 Mislav Blažević <krofnica996@gmail.com>
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
#include "steinsgate.hpp"
#include "sginterpreter.hpp"

int PrintArguments(char** argv)
{
    std::cout << "usage: " << argv[0] << " <Nitroplus | JAST | Fuwanovel>" << std::endl;
    return 1;
}

ExePublisher ParseExeVersion(char** argv)
{
    string VersionString = argv[1];
    if (VersionString == "Nitroplus")
        return EXE_NITROPLUS;
    else if (VersionString == "JAST")
        return EXE_JAST;
    else if (VersionString == "Fuwanovel")
        return EXE_FUWANOVEL;
    else
        return EXE_INVALID;
}

int main(int argc, char** argv)
{
    if (argc != 2)
        return PrintArguments(argv);

    ExePublisher Version = ParseExeVersion(argv);
    if (Version == EXE_INVALID)
        return PrintArguments(argv);

    SGInterpreter* pInterpreter = new SGInterpreter(Version);
    SteinsGate* pGame = new SteinsGate(pInterpreter);
    pGame->GLCallback(std::bind(&SGInterpreter::Initialize, pInterpreter, pGame));
    pGame->Run();

    delete pGame;
    delete pInterpreter;
    delete sResourceMgr;
}
