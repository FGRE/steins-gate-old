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
#include "resourcemgr.hpp"
#include "text.hpp"
#include <thread>

extern "C"
{
    void gst_init(int* argc, char** argv[]);
}

int main(int argc, char** argv)
{
    gst_init(&argc, &argv);
    Text::Initialize("/usr/share/fonts/cjkuni-uming/uming.ttc");
    sResourceMgr = new ResourceMgr({"cg.npa", "nss.npa", "voice.npa", "sound.npa"});
    SGInterpreter* pInterpreter = new SGInterpreter;
    SteinsGate* pGame = new SteinsGate(pInterpreter);
    pGame->GLCallback(std::bind(&SGInterpreter::Initialize, pInterpreter, pGame));
    std::thread ScriptThread(std::bind(&SGInterpreter::Main, pInterpreter));
    pGame->Run();
    ScriptThread.join();
    delete pGame;
    delete sResourceMgr;
    delete pInterpreter;
    return 0;
}
