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
#include "sgexe.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstring>

sf::Texture* LoadTextureFromColor(string Color, int32_t Width, int32_t Height);

PhoneModeAddressBook::PhoneModeAddressBook(Phone* pPhone) :
PhoneMode(pPhone),
ContactHighlight(0),
CallAllowedMask(0),
AddressMask(0)
{
    pWhite = LoadTextureFromColor("white", MASK_WIDTH, MASK_HEIGHT);
    pHighlight = LoadTextureFromColor("#ffc896", 220, 20);
    Highlight.setTexture(*pHighlight);
    Mask.setTexture(*pPhone->pPhoneTex);
    Mask.setPosition(PHONE_WALLPAPER_X, PHONE_WALLPAPER_Y);
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

void PhoneModeAddressBook::Draw(sf::RenderWindow* pWindow)
{
    pWindow->draw(Mask);
    pWindow->draw(pPhone->BlueHeader);
    pWindow->draw(HeaderText);
    pWindow->draw(Highlight);
    for (auto i = Contacts.begin(); i != Contacts.end(); ++i)
        pWindow->draw(i->Text);
}

void PhoneModeAddressBook::MouseMoved(sf::Vector2i Pos)
{
    if (Pos.x > BLUE_HEADER_POS_X && Pos.x < BLUE_HEADER_POS_X + MASK_WIDTH &&
        Pos.y > BLUE_HEADER_POS_Y + BLUE_HEADER_HEIGHT)
        SetHighlight((Pos.y - (BLUE_HEADER_POS_Y + BLUE_HEADER_HEIGHT)) / 20);
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

void PhoneModeAddressBook::SetHighlight(int16_t Index)
{
    if (Index < 0 || Index >= Contacts.size())
        return;

    Highlight.setPosition(BLUE_HEADER_POS_X, BLUE_HEADER_POS_Y + BLUE_HEADER_HEIGHT + Index * 20);
    ContactHighlight = Index;
}

void PhoneModeAddressBook::SetAddressMask(uint16_t AddressMask)
{
    if (this->AddressMask & AddressMask)
        return;

    string Name = sExe->ReadStringIndirect(VA_PHONE_ADDRMENU, __builtin_ctz(AddressMask), 0xC, 0x4);
    sf::Text ContactText(sf::String::fromUtf8(Name.begin(), Name.end()), Text::Font, 20);
    ContactText.setPosition(PHONE_WALLPAPER_X, BLUE_HEADER_POS_Y + BLUE_HEADER_HEIGHT + Contacts.size() * 20);
    ContactText.setColor(sf::Color::Black);
    Contacts.push_back({ ContactText, (uint8_t)__builtin_ctz(AddressMask) });
    this->AddressMask |= AddressMask;
}

void PhoneModeAddressBook::ResetAddressMask(uint16_t AddressMask)
{
    if (!(this->AddressMask & AddressMask))
        return;

    uint8_t Index = (uint8_t)__builtin_ctz(AddressMask);
    for (auto i = Contacts.begin(); i != Contacts.end(); ++i)
    {
        if (i->Index == Index)
        {
            Contacts.erase(i);
            break;
        }
    }
    this->AddressMask &= ~AddressMask;
}
