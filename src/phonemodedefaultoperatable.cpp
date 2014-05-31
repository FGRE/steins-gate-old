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
#include "phonemodedefaultoperatable.hpp"
#include "phone.hpp"
#include "sgexe.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <unistd.h>

enum PhoneButton
{
    BUTTON_MAIL = 0,
    BUTTON_CONTACTS,
    BUTTON_WEB,
    BUTTON_SETTINGS,
    BUTTON_MAX
};

const int16_t PHONE_BUTTON_TEX_X = 27;
const int16_t PHONE_BUTTON_TEX_Y[BUTTON_MAX * 2] =
{
    151, 222,
    20, 82,
    291, 363,
    436, 505
};
const int16_t PHONE_BUTTON_POS_X[] =
{
    766,
    848
};
const int16_t PHONE_BUTTON_POS_Y[] =
{
    154,
    223
};
const int16_t PHONE_BUTTON_WIDTH = 50;
const int16_t PHONE_BUTTON_HEIGHT = 50;

// Menu wallpaper
const int16_t PHONE_MENU_TEX_X = 532;
const int16_t PHONE_MENU_TEX_Y = 760;
const int16_t PHONE_MENU_WIDTH = 220;
const int16_t PHONE_MENU_HEIGHT = 254;

const int16_t PHONE_MENU_MAIN_TEX_X = 670;
const int16_t PHONE_MENU_MAIN_TEX_Y = 213;
const int16_t PHONE_MENU_MAIN_WIDTH = 220;
const int16_t PHONE_MENU_MAIN_HEIGHT = 38;

PhoneModeDefaultOperatable::PhoneModeDefaultOperatable(Phone* pPhone) :
PhoneMode(pPhone),
ButtonHighlightX(-1),
ButtonHighlightY(-1)
{
    MenuOverlay.setTexture(*pPhone->pPhoneTex);
    MenuOverlay.setPosition(PHONE_WALLPAPER_X, PHONE_WALLPAPER_Y);

    for (int y = 0; y < 2; ++y)
    {
        for (int x = 0; x < 2; ++x)
        {
            sf::IntRect ClipArea(PHONE_BUTTON_TEX_X, PHONE_BUTTON_TEX_Y[(y * 2 + x) * 2 + 1], PHONE_BUTTON_WIDTH, PHONE_BUTTON_HEIGHT);
            Button[y][x].setTexture(*pPhone->pPhoneTex);
            Button[y][x].setTextureRect(ClipArea);
            Button[y][x].setPosition(PHONE_BUTTON_POS_X[x], PHONE_BUTTON_POS_Y[y]);
        }
    }
}

void PhoneModeDefaultOperatable::OnOpen(uint8_t OldMode)
{
    sf::IntRect WallpaperClipArea(PHONE_MENU_TEX_X, PHONE_MENU_TEX_Y, PHONE_MENU_WIDTH, PHONE_MENU_HEIGHT);
    pPhone->Wallpaper.setTexture(*pPhone->pPhoneTex);
    pPhone->Wallpaper.setTextureRect(WallpaperClipArea);
    sf::IntRect OverlayClipArea(PHONE_MENU_MAIN_TEX_X, PHONE_MENU_MAIN_TEX_Y, PHONE_MENU_MAIN_WIDTH, PHONE_MENU_MAIN_HEIGHT);
    MenuOverlay.setTextureRect(OverlayClipArea);

    switch (OldMode)
    {
        case MODE_ADDRESS_BOOK:
            sf::Mouse::setPosition(sf::Vector2i(PHONE_BUTTON_POS_X[1] + PHONE_BUTTON_WIDTH / 2,
                                                PHONE_BUTTON_POS_Y[0] + PHONE_BUTTON_HEIGHT / 2),
                                   *pPhone->pWindow);
            break;
        case MODE_MAIL_MENU:
            sf::Mouse::setPosition(sf::Vector2i(PHONE_BUTTON_POS_X[0] + PHONE_BUTTON_WIDTH / 2,
                                                PHONE_BUTTON_POS_Y[0] + PHONE_BUTTON_HEIGHT / 2),
                                   *pPhone->pWindow);
            break;
    }
}

void PhoneModeDefaultOperatable::Draw(sf::RenderWindow* pWindow)
{
    pWindow->draw(MenuOverlay);
    for (int y = 0; y < 2; ++y)
        for (int x = 0; x < 2; ++x)
            pWindow->draw(Button[y][x]);
}

void PhoneModeDefaultOperatable::MouseMoved(sf::Vector2i Pos)
{
    for (int8_t y = 0; y < 2; ++y)
        for (int8_t x = 0; x < 2; ++x)
            if (Pos.x > PHONE_BUTTON_POS_X[x] && Pos.x < PHONE_BUTTON_POS_X[x] + PHONE_BUTTON_WIDTH)
                if (Pos.y > PHONE_BUTTON_POS_Y[y] && Pos.y < PHONE_BUTTON_POS_Y[y] + PHONE_BUTTON_HEIGHT)
                    HighlightButton(x, y);
}

uint8_t PhoneModeDefaultOperatable::LeftMouseClicked()
{
    if (ButtonHighlightX != -1)
    {
        switch (ButtonHighlightY * 2 + ButtonHighlightX)
        {
            case BUTTON_CONTACTS:
                return MODE_ADDRESS_BOOK;
            case BUTTON_MAIL:
                return MODE_MAIL_MENU;
            case BUTTON_WEB:
                const char* Website;
                switch (sExe->GetVersion())
                {
                    case EXE_NITROPLUS:
                        Website = "http://futuregadget-lab.com/";
                        break;
                    // Fuwanovel executable opens japanese (.com) website
                    // But it makes more sense to open english version instead
                    case EXE_FUWANOVEL:
                    case EXE_JAST:
                        Website = "http://futuregadget-lab.us/";
                        break;
                }
                if (fork() == 0)
                    execlp("/usr/bin/xdg-open", "/usr/bin/xdg-open", Website, NULL);
                break;
            case BUTTON_SETTINGS:
                break;
        }
    }
    return MODE_INVALID;
}

uint8_t PhoneModeDefaultOperatable::RightMouseClicked()
{
    return MODE_DEFAULT;
}

void PhoneModeDefaultOperatable::HighlightButton(int8_t x, int8_t y)
{
    if (x == ButtonHighlightX && y == ButtonHighlightY)
        return;

    // Remove old highlight
    sf::IntRect OldClipArea(PHONE_BUTTON_TEX_X, PHONE_BUTTON_TEX_Y[(ButtonHighlightY * 2 + ButtonHighlightX) * 2 + 1], PHONE_BUTTON_WIDTH, PHONE_BUTTON_HEIGHT);
    Button[ButtonHighlightY][ButtonHighlightX].setTextureRect(OldClipArea);

    // Add new highlight
    ButtonHighlightX = x;
    ButtonHighlightY = y;
    sf::IntRect ClipArea(PHONE_BUTTON_TEX_X, PHONE_BUTTON_TEX_Y[(y * 2 + x) * 2], PHONE_BUTTON_WIDTH, PHONE_BUTTON_HEIGHT);
    Button[y][x].setTextureRect(ClipArea);

    // Move mouse to center of button
    sf::Mouse::setPosition(sf::Vector2i(PHONE_BUTTON_POS_X[x] + PHONE_BUTTON_WIDTH / 2,
                                        PHONE_BUTTON_POS_Y[y] + PHONE_BUTTON_HEIGHT / 2),
                           *pPhone->pWindow);
}
