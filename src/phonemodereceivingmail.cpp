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
#include "phonemodereceivingmail.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstring>

static const char* RecMailStr = "受信中";

PhoneModeReceivingMail::PhoneModeReceivingMail(Phone* pPhone) : PhoneMode(pPhone), AnimStep(0)
{
    for (int i = 0; i < 4; ++i)
    {
        HDots[i].setTexture(*pPhone->pPhoneTex);
        HDots[i].setPosition(780 + (i % 2) * 48 + (i % 2) * 2 + (i / 2) * 10, 150 + (i / 2) * 48 + (i / 2) * 2);
    }
    for (int i = 0; i < 2; ++i)
    {
        VDots[i].setTexture(*pPhone->pPhoneTex);
        VDots[i].setPosition(780 + i * 48 * 2 + i * 4, 150 + (!i) * 10);
    }
    RecMailText.setFont(Text::Font);
    RecMailText.setString(sf::String::fromUtf8(RecMailStr, RecMailStr + strlen(RecMailStr)));
    // TODO: position and size are a guess
    RecMailText.setPosition(780 + 2 * 8, 150 + 2 * 8);
    RecMailText.setCharacterSize(20);
    UpdateAnim();
}

PhoneModeReceivingMail::~PhoneModeReceivingMail()
{
}

void PhoneModeReceivingMail::OnOpen(uint8_t OldMode)
{
}

void PhoneModeReceivingMail::Draw(sf::RenderWindow* pWindow)
{
    // TODO: animation time is a guess
    if (AnimClock.getElapsedTime().asMilliseconds() > 100)
        UpdateAnim();

    pWindow->draw(pPhone->Header);
    for (int i = 0; i < 4; ++i)
        pWindow->draw(HDots[i]);
    for (int i = 0; i < 2; ++i)
        pWindow->draw(VDots[i]);
    pWindow->draw(RecMailText);
}

void PhoneModeReceivingMail::MouseMoved(sf::Vector2i Pos)
{
}

void PhoneModeReceivingMail::UpdateAnim()
{
    AnimClock.restart();

    // TODO: animation flow is a guess
    for (int i = 0; i < 4; ++i)
        HDots[i].setTextureRect(sf::IntRect(1000, 720 + AnimStep * 10, 48, 8));
    for (int i = 0; i < 2; ++i)
        VDots[i].setTextureRect(sf::IntRect(1000 + AnimStep * 10, 720, 8, 48));

    if (++AnimStep == 5)
        AnimStep = 0;
}
