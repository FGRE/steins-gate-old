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
#include "nsbinterpreter.hpp"
#include "sgexe.hpp"

class Phone;
class SteinsGate;

namespace std
{
    class thread;
};

class SGInterpreter : public NsbInterpreter
{
    friend class SteinsGate;
    friend class Phone;
public:
    SGInterpreter(ExePublisher Version);
    ~SGInterpreter();
    void Initialize(Game* pSteinsGate);
    void Main();

    void MouseMoved(sf::Vector2i Pos);
    void MouseClicked(sf::Event::MouseButtonEvent Event);

    virtual void OnVariableChanged(const string& Identifier);

private:
    void WriteHack(const char* ScriptName, uint32_t LineNumber, uint16_t NewMagic);

    virtual void Literal();

    void SendMailEdit();
    void AllowPhoneCall();
    void UNK101();
    void UNK143();

    void PhoneToggle();
    void PhoneAddressMenuHighlight();
    void SGPhoneOpen();
    void SGPhoneMode();
    void SGPhonePriority();

    Phone* pPhone;
    std::thread* pScriptThread;
};
