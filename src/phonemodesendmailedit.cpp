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
#include "phonemodesendmailedit.hpp"
#include "phone.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

sf::Texture* LoadTextureFromColor(string Color, int32_t Width, int32_t Height);

PhoneModeSendMailEdit::PhoneModeSendMailEdit(Phone* pPhone) : PhoneMode(pPhone)
{
    pWhite = LoadTextureFromColor("white", MASK_WIDTH, MASK_HEIGHT);
    Mask.setTexture(*pPhone->pPhoneTex);
    Mask.setPosition(PHONE_WALLPAPER_X, PHONE_WALLPAPER_Y);

    Body.setPosition(729.0f, 144.0f);
    Body.setCharacterSize(17);
    Body.setFont(Text::Font);
    Body.setColor(sf::Color::Black);
}

PhoneModeSendMailEdit::~PhoneModeSendMailEdit()
{
    delete pWhite;
}

void PhoneModeSendMailEdit::OnOpen(uint8_t OldMode)
{
    pPhone->Wallpaper.setTexture(*pWhite, true);
    sf::IntRect MaskClipArea(MASK_TEX_X, MASK_TEX_Y, MASK_WIDTH, MASK_HEIGHT);
    Mask.setTextureRect(MaskClipArea);
}

void PhoneModeSendMailEdit::Draw(sf::RenderWindow* pWindow)
{
    pWindow->draw(Mask);
    pWindow->draw(pPhone->BlueHeader);
    pWindow->draw(Body);
}

void PhoneModeSendMailEdit::MouseMoved(sf::Vector2i Pos)
{
}

uint8_t PhoneModeSendMailEdit::LeftMouseClicked()
{
    return MODE_INVALID;
}

uint8_t PhoneModeSendMailEdit::RightMouseClicked()
{
    return MODE_INVALID;
}

void PhoneModeSendMailEdit::SetText(const string& Subject, const string& Sender, const string& Body)
{
    this->Body.setString(sf::String::fromUtf8(Body.begin(), Body.end()));
}
