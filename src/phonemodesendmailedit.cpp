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

PhoneModeSendMailEdit::PhoneModeSendMailEdit(Phone* pPhone) : PhoneMode(pPhone)
{
}

void PhoneModeSendMailEdit::OnOpen(uint8_t OldMode)
{
}

void PhoneModeSendMailEdit::Draw(sf::RenderWindow* pWindow)
{
    pWindow->draw(pPhone->BlueHeader);
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
