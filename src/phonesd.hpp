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
#ifndef PHONE_SD_HPP
#define PHONE_SD_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <string>
using std::string;

namespace sf { class RenderWindow; class Texture; }
class Phone;

class PhoneSD
{
    friend class Phone;
private:
    PhoneSD();
    ~PhoneSD();

    void Draw(sf::RenderWindow* pWindow);
    void SetDate(string Date);

    int DateToWeekDay(string Date);

    sf::Sprite SD;
    sf::Sprite SDDate[6];
    sf::Sprite SDIcon[4];
    sf::Texture* pSDTex;
};

#endif
