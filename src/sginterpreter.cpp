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
#include "nsbmagic.hpp"
#include "nsbcontext.hpp"
#include <thread>

static const string PhoneModeString[] =
{
    "PhoneMode_AddressBook",
    "PhoneMode_AddressConfirmDial",
    "PhoneMode_AddressConfirmMail",
    "PhoneMode_Calling",
    "PhoneMode_CompleteReceiveMail",
    "PhoneMode_CompleteSendMail",
    "PhoneMode_Default",
    "PhoneMode_DefaultOperatable",
    "PhoneMode_DialogSendMailEdit",
    "PhoneMode_EngageNormal",
    "PhoneMode_EngageVisual",
    "PhoneMode_MailMenu",
    "PhoneMode_MailSubMenu",
    "PhoneMode_PowerOff",
    "PhoneMode_ReceiveBox",
    "PhoneMode_ReceivedMail",
    "PhoneMode_ReceivingMail",
    "PhoneMode_SendBox",
    "PhoneMode_Sending",
    "PhoneMode_SendMailEdit"
};

static const string PhoneContactString[] =
{
    "PhID_DAR",
    "PhID_Oven",
    "PhID_FEI",
    "PhID_MAY",
    "PhID_RUK"
/*
macrosys2.nsb:    UNK143("PhID_John");
macrosys2.nsb:    UNK143("PhID_CRS");
macrosys2.nsb:    UNK143("PhID_SUZ");
macrosys2.nsb:    UNK143("PhID_SUZ_0");
*/
};

extern std::map<std::string, int32_t> NsbConstants;

SGInterpreter::SGInterpreter()
{
    Builtins[MAGIC_ALLOW_PHONE_CALL] = (void(NsbInterpreter::*)())&SGInterpreter::AllowPhoneCall;

    SetVariable("#N2Start", new Variable("false"));
    SetVariable("#SYSTEM_cosplay_patch", new Variable("false"));
    SetVariable("$GameClose", new Variable("false"));
    SetVariable("$GameStart", new Variable(0));
    SetVariable("$silhouette", new Variable("false"));
    SetVariable("$OnDialog", new Variable("false"));
}

SGInterpreter::~SGInterpreter()
{
    pScriptThread->join();
    delete pPhone;
    delete pScriptThread;
}

void SGInterpreter::Initialize(Game* pSteinsGate)
{
    NsbInterpreter::Initialize(pSteinsGate);
    pPhone = new Phone(new sf::Sprite(), pSteinsGate);
    pScriptThread = new std::thread(std::bind(&SGInterpreter::Main, this));
}

void SGInterpreter::WriteHack(const char* ScriptName, uint32_t LineNumber, uint16_t NewMagic)
{
    sResourceMgr->GetScriptFile(ScriptName)->GetLine(LineNumber)->Magic = NewMagic;
}

void SGInterpreter::Main()
{
    // Fixes standing characters
    WriteHack("nss/function_stand.nsb", 1083, MAGIC_RETURN);
    // Fixes getting stuck on calling microwave
    WriteHack("nss/sg01_03.nsb", 890, MAGIC_CLEAR_PARAMS);
    WriteHack("nss/sg01_03.nsb", 1057, MAGIC_CLEAR_PARAMS);

    Start();
    ExecuteScript("nss/0_boot.nsb");
}

void SGInterpreter::OnVariableChanged(const string& Identifier)
{
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
        pGame->GLCallback(std::bind(&Phone::SetDate, pPhone, GetVariable<int32_t>("$LR_DATE")));
    else if (Identifier == "$SW_PHONE_PRI")
        pGame->GLCallback(std::bind(&SGInterpreter::SGPhonePriority, this));
    else if (Identifier == "$SW_PHONE_ADRMENUCUR")
        PhoneAddressMenuHighlight();
    else
    {
        auto iter = NsbConstants.find(GetVariable<string>(Identifier));
        if (iter != NsbConstants.end())
            SetVariable(Identifier, new Variable(iter->second));
    }
}

void SGInterpreter::PhoneAddressMenuHighlight()
{
    int32_t Index = GetVariable<int32_t>("$SW_PHONE_ADRMENUCUR");
    pGame->GLCallback(std::bind(&Phone::AddressMenuHighlight, pPhone, Index));
    SetVariable("$SW_PHONE_ADRCURCNO", new Variable(PhoneContactString[Index]));
}

void SGInterpreter::PhoneToggle()
{
    if (GetVariable<int32_t>("$SF_Phone_Open") == 1)
        SetVariable("$SF_Phone_Open", new Variable(0));
    else
        SetVariable("$SF_Phone_Open", new Variable(1));
    SGPhoneOpen();
}

void SGInterpreter::MouseClicked(sf::Event::MouseButtonEvent Event)
{
    NsbInterpreter::MouseClicked(Event);
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

    pPhone->UpdateMode(Mode);
}

void SGInterpreter::AllowPhoneCall()
{
    uint8_t Mask = 0;
    for (uint8_t i = 0; i < 4; ++i)
    {
        string PhID = Pop<string>();
        for (uint8_t j = 0; j < 5; ++j)
        {
            if (PhID == PhoneContactString[j])
            {
                Mask |= (1 << j);
                break;
            }
        }
    }
    pPhone->SetPhoneCallAllowMask(Mask);
}
