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
#include "phonemodemailmenu.hpp"
#include "phone.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstring>

const int16_t OVERLAY_RED_TEX_X = 302;
const int16_t OVERLAY_RED_TEX_Y = 722;
const int16_t OVERLAY_RED_WIDTH = 220;
const int16_t OVERLAY_RED_HEIGHT = 119;
const int16_t OVERLAY_RED_POS_X = 728;
const int16_t OVERLAY_RED_POS_Y = 159;

const int16_t MAIL_MENU_TEXT_POS_X = 750;

PhoneModeMailMenu::PhoneModeMailMenu(Phone* pPhone) :
PhoneMode(pPhone),
MailMenuHighlight(0)
{
    OverlayRed.setTexture(*pPhone->pPhoneTex);
    OverlayRed.setPosition(OVERLAY_RED_POS_X, OVERLAY_RED_POS_Y);
    OverlayRed.setTextureRect(sf::IntRect(OVERLAY_RED_TEX_X, OVERLAY_RED_TEX_Y, OVERLAY_RED_WIDTH, OVERLAY_RED_HEIGHT));

    for (int i = 0; i < 2; ++i)
    {
        MailMenuText[i].setString(sf::String::fromUtf8(HeaderString[i + 1], HeaderString[i + 1] + strlen(HeaderString[i + 1])));
        MailMenuText[i].setFont(Text::Font);
        MailMenuText[i].setPosition(MAIL_MENU_TEXT_POS_X, 170 + i * 20);
        MailMenuText[i].setCharacterSize(20);
        MailMenuText[i].setColor(sf::Color::Black);
    }
    MailMenuText[MailMenuHighlight].setColor(sf::Color::Red);
}

void PhoneModeMailMenu::OnOpen(uint8_t OldMode)
{
    if (MailMenuHighlight != 0)
    {
        MailMenuText[MailMenuHighlight].setColor(sf::Color::Black);
        MailMenuText[0].setColor(sf::Color::Red);
        MailMenuHighlight = 0;
    }
    MouseMoved(sf::Mouse::getPosition(*pPhone->pWindow));
}

void PhoneModeMailMenu::Draw(sf::RenderWindow* pWindow)
{
    pWindow->draw(OverlayRed);
    for (int i = 0; i < 2; ++i)
        pWindow->draw(MailMenuText[i]);
}

void PhoneModeMailMenu::MouseMoved(sf::Vector2i Pos)
{
    if (Pos.x > MAIL_MENU_TEXT_POS_X && Pos.x < MAIL_MENU_TEXT_POS_X + MailMenuText[0].getLocalBounds().width)
    {
        int i = (Pos.y - 170) / 20;
        if (i >= 0 && i < 2 && i != MailMenuHighlight)
        {
            MailMenuText[MailMenuHighlight].setColor(sf::Color::Black);
            MailMenuText[i].setColor(sf::Color::Red);
            MailMenuHighlight = i;
        }
    }
}

uint8_t PhoneModeMailMenu::LeftMouseClicked()
{
    return MODE_INVALID;
}

uint8_t PhoneModeMailMenu::RightMouseClicked()
{
    return MODE_DEFAULT_OPERATABLE;
}
