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
#include "sginterpreter.hpp"
#include "phone.hpp"
#include "steinsgate.hpp"
#include <thread>

SGInterpreter::SGInterpreter()
{
}

SGInterpreter::~SGInterpreter()
{
    delete pPhone;
    pScriptThread->join();
    delete pScriptThread;
}

void SGInterpreter::Initialize(Game* pSteinsGate)
{
    NsbInterpreter::Initialize(pSteinsGate);
    pPhone = new Phone(new sf::Sprite(), pSteinsGate);
    pScriptThread = new std::thread(std::bind(&SGInterpreter::Main, this));
}

void SGInterpreter::Main()
{
    std::string ScriptName = "nss/sg00_01.nsb";
    do
    {
        Start();
        ExecuteScript(ScriptName);
        ScriptName = "nss/" + GetVariable<std::string>("$GameName");
        ScriptName.back() = 'b';
        std::cout << "Switching to: " << ScriptName << std::endl;
    } while (!StopInterpreter);
}

void SGInterpreter::Set()
{
    NsbInterpreter::Set();

    const string& Identifier = pContext->pLine->Params[0];

    // Handle hardcoded phone operations
    if (Identifier == "$SF_Phone_Open")
        pGame->GLCallback(std::bind(&SGInterpreter::SGPhoneOpen, this));
    else if (Identifier == "$SW_PHONE_MODE")
        pGame->GLCallback(std::bind(&SGInterpreter::SGPhoneMode, this));
    else if (Identifier == "$SF_PhoneMailReciveNew")
        pGame->GLCallback(std::bind(&Phone::MailReceive, pPhone, GetVariable<int32_t>("$SF_PhoneMailReciveNew")));
    else if (Identifier == "$SF_PhoneSD_Disp")
        pGame->GLCallback(std::bind(&Phone::SDDisplay, pPhone, GetVariable<int32_t>("$SF_PhoneSD_Disp")));
    else if (Identifier == "$LR_DATE")
        pGame->GLCallback(std::bind(&Phone::SetDate, pPhone, GetVariable<string>("$LR_DATE")));
    else if (Identifier == "$SW_PHONE_PRI")
        pGame->GLCallback(std::bind(&SGInterpreter::SGPhonePriority, this));
}

void SGInterpreter::PhoneToggle()
{
    if (GetVariable<int32_t>("$SF_Phone_Open") == 1)
        SetVariable("$SF_Phone_Open", Variable{"INT", "0"});
    else
        SetVariable("$SF_Phone_Open", Variable{"INT", "1"});
    SGPhoneOpen();
}

void SGInterpreter::MouseClicked(sf::Event::MouseButtonEvent Event)
{
    if (Event.button == sf::Mouse::Left)
        pPhone->LeftMouseClicked(sf::Vector2i(Event.x, Event.y));
    else if (Event.button == sf::Mouse::Right)
        pPhone->RightMouseClicked(this);
}

void SGInterpreter::MouseMoved(sf::Vector2i Pos)
{
    pPhone->MouseMoved(Pos);
}

void SGInterpreter::SGPhoneOpen()
{
    pPhone->UpdateOpenMode(GetVariable<int32_t>("$SF_Phone_Open"));
    pGame->RemoveDrawable(pPhone);
    pGame->AddDrawable(pPhone);
    pPhone->UpdateMode(MODE_DEFAULT_OPERATABLE);
}

void SGInterpreter::SGPhonePriority()
{
    pPhone->SetPriority(GetVariable<int32_t>("$SW_PHONE_PRI"));
    pGame->RemoveDrawable(pPhone);
    pGame->AddDrawable(pPhone);
}

void SGInterpreter::SGPhoneMode()
{
    string StringMode = GetVariable<string>("$SW_PHONE_MODE");
    uint8_t Mode = MODE_INVALID;

    for (uint8_t i = 0; i <= MODE_SEND_MAIL_EDIT; ++i)
    {
        if (StringMode == PhoneModeString[i])
        {
            Mode = i;
            break;
        }
    }

    if (Mode == MODE_INVALID)
        return;

    pPhone->UpdateMode(Mode);
}
