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
#include "phonemodeaddressbook.hpp"
#include "phone.hpp"
#include "nsbinterpreter.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstring>

const int16_t MASK_TEX_X = 425;
const int16_t MASK_TEX_Y = 21;
const int16_t MASK_WIDTH = 220;
const int16_t MASK_HEIGHT = 253;

const char* ContactString[] =
{
    "ダル",
    "電話レンジ (仮)",
    "フェイリス",
    "まゆり",
    "ルカ子"
};

PhoneModeAddressBook::PhoneModeAddressBook(Phone* pPhone) :
PhoneMode(pPhone),
ContactHighlight(0),
CallAllowedMask(0)
{
    pWhite = LoadTextureFromColor("white", MASK_WIDTH, MASK_HEIGHT);
    pHighlight = LoadTextureFromColor("#ffc896", 220, 20);
    Highlight.setTexture(*pHighlight);
    Mask.setTexture(*pPhone->pPhoneTex);
    Mask.setPosition(PHONE_WALLPAPER_X, PHONE_WALLPAPER_Y);

    for (int i = 0; i < 5; ++i)
    {
        Contacts[i].setString(sf::String::fromUtf8(ContactString[i], ContactString[i] + strlen(ContactString[i])));
        Contacts[i].setFont(Text::Font);
        Contacts[i].setPosition(PHONE_WALLPAPER_X, BLUE_HEADER_POS_Y + BLUE_HEADER_HEIGHT + i * 20);
        Contacts[i].setCharacterSize(20);
        Contacts[i].setColor(sf::Color::Black);
    }

    HeaderText.setFont(Text::Font);
    HeaderText.setPosition(BLUE_HEADER_POS_X + 24, BLUE_HEADER_POS_Y);
    HeaderText.setCharacterSize(20);
}

PhoneModeAddressBook::~PhoneModeAddressBook()
{
    delete pHighlight;
    delete pWhite;
}

void PhoneModeAddressBook::OnOpen(uint8_t OldMode)
{
    sf::Mouse::setPosition(sf::Vector2i(BLUE_HEADER_POS_X + BLUE_HEADER_WIDTH / 2, BLUE_HEADER_POS_Y + BLUE_HEADER_HEIGHT + 10), *pPhone->pWindow);
    sf::IntRect BlueHeaderClipArea(BLUE_HEADER_TEX_X, BLUE_HEADER_TEX_Y[BLUE_HEADER_CONTACTS], BLUE_HEADER_WIDTH, BLUE_HEADER_HEIGHT);
    pPhone->BlueHeader.setTextureRect(BlueHeaderClipArea);
    pPhone->Wallpaper.setTexture(*pWhite, true);
    sf::IntRect MaskClipArea(MASK_TEX_X, MASK_TEX_Y, MASK_WIDTH, MASK_HEIGHT);
    Mask.setTextureRect(MaskClipArea);
    HeaderText.setString(sf::String::fromUtf8(HeaderString[0], HeaderString[0] + strlen(HeaderString[0]) - 3));
}

void PhoneModeAddressBook::OnClose()
{
}

void PhoneModeAddressBook::Draw(sf::RenderWindow* pWindow)
{
    pWindow->draw(Mask);
    pWindow->draw(pPhone->BlueHeader);
    pWindow->draw(HeaderText);
    pWindow->draw(Highlight);
    for (int i = 0; i < 5; ++i)
        pWindow->draw(Contacts[i]);
}

void PhoneModeAddressBook::MouseMoved(sf::Vector2i Pos)
{
    if (Pos.x > BLUE_HEADER_POS_X && Pos.x < BLUE_HEADER_POS_X + MASK_WIDTH && Pos.y > BLUE_HEADER_POS_Y + BLUE_HEADER_HEIGHT)
    {
        int i = (Pos.y - (BLUE_HEADER_POS_Y + BLUE_HEADER_HEIGHT)) / 20;
        if (i >= 0 && i < 5)
        {
            Highlight.setPosition(BLUE_HEADER_POS_X, BLUE_HEADER_POS_Y + BLUE_HEADER_HEIGHT + i * 20);
            ContactHighlight = i;
        }
    }
}

uint8_t PhoneModeAddressBook::LeftMouseClicked()
{
    if (CallAllowedMask & (1 << ContactHighlight))
        return MODE_ADDRESS_CONFIRM_DIAL;
    return MODE_INVALID;
}

uint8_t PhoneModeAddressBook::RightMouseClicked()
{
    return MODE_DEFAULT_OPERATABLE;
}
