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
#ifndef PHONE_MODE_SEND_MAIL_EDIT_HPP
#define PHONE_MODE_SEND_MAIL_EDIT_HPP

#include "phonemode.hpp"

class Phone;

class PhoneModeSendMailEdit : PhoneMode
{
    friend class Phone;
protected:
    PhoneModeSendMailEdit(Phone* pPhone);

    virtual void OnOpen(uint8_t OldMode);
    virtual void Draw(sf::RenderWindow* pWindow);
    virtual void MouseMoved(sf::Vector2i Pos);
    virtual uint8_t LeftMouseClicked();
    virtual uint8_t RightMouseClicked();

private:
};

#endif
