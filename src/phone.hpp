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
#include "text.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <string>
using std::string;

enum
{
    MODE_ADDRESS_BOOK = 0,
    MODE_ADDRESS_CONFIRM_DIAL,
    MODE_ADDRESS_CONFIRM_MAIL,
    MODE_CALLING,
    MODE_COMPLETE_RECEIVE_MAIL,
    MODE_COMPLETE_SEND_MAIL,
    MODE_DEFAULT,
    MODE_DEFAULT_OPERATABLE,
    MODE_DIALOG_SEND_MAIL_EDIT,
    MODE_ENGAGE_NORMAL,
    MODE_ENGAGE_VISUAL,
    MODE_MAIL_MENU,
    MODE_MAIL_SUB_MENU,
    MODE_POWER_OFF,
    MODE_RECEIVE_BOX,
    MODE_RECEIVED_MAIL,
    MODE_RECEIVING_MAIL,
    MODE_SEND_BOX,
    MODE_SENDING,
    MODE_SEND_MAIL_EDIT,

    MODE_INVALID = 0xFF // Custom
};

const int16_t PHONE_ANIM_SPEED = 40; // TODO: guess
const int8_t PHONE_ANIM_ROW_MAX = 1;
const int8_t PHONE_ANIM_COLUMN_MAX = 4;
const int8_t PHONE_ANIM_UNDERFLOW = -1;
const int16_t PHONE_WIDTH = 313;
const int16_t PHONE_HEIGHT = 576;
const int16_t PHONE_POS_X = 678;
const int16_t PHONE_POS_Y = 8;

// cg/sys/phone/phone_01.png
const int16_t PHONE_TEX_X = 95; // TODO: guess
const int16_t PHONE_TEX_Y = 0; // TODO: guess
const int16_t PHONE_HEADER_TEX_X = 670;
const int16_t PHONE_HEADER_TEX_Y = 384;
const int16_t PHONE_HEADER_WIDTH = 220;
const int16_t PHONE_HEADER_HEIGHT = 24;
const int16_t PHONE_NEW_MAIL_TEX_X = 302;
const int16_t PHONE_NEW_MAIL_TEX_Y = 576;
const int16_t PHONE_NEW_MAIL_WIDTH = 220;
const int16_t PHONE_NEW_MAIL_HEIGHT = 130;

const int16_t PHONE_HEADER_POS_X = PHONE_POS_X + 49; // 727
const int16_t PHONE_HEADER_POS_Y = PHONE_POS_Y + 89; // 97
const int16_t PHONE_WALLPAPER_X = PHONE_HEADER_POS_X;
const int16_t PHONE_WALLPAPER_Y = PHONE_HEADER_POS_Y + PHONE_HEADER_HEIGHT; // TODO: guess
const int16_t PHONE_OVERLAY_POS_X = PHONE_WALLPAPER_X;
const int16_t PHONE_OVERLAY_POS_Y = 180;

extern const string PhoneModeString[];

class PhoneModeDefaultOperatable;
class SGInterpreter;
class PhoneMode;
class PhoneSD;

class Phone : public DrawableBase
{
    friend class PhoneModeDefaultOperatable;
public:
    Phone(sf::Drawable* pDrawable, sf::Window* pWindow);
    virtual ~Phone();

    virtual void Draw(sf::RenderWindow* pWindow);
    virtual void Update();
    void SetDate(string Date);
    void SDDisplay(int32_t Show);
    void MailReceive(int32_t Show);
    void UpdateOpenMode(int32_t OpenMode);
    void UpdateMode(uint8_t NewMode);
    void SetPriority(int32_t Priority);
    void MouseMoved(sf::Vector2i Pos);
    void LeftMouseClicked(sf::Vector2i Pos);
    void RightMouseClicked(SGInterpreter* pInterpreter);

private:
    void UpdateAnim();

    bool ShowSD;
    bool ShowOverlay;
    uint8_t Mode;
    uint8_t State;
    int8_t AnimRow;
    int8_t AnimColumn;
    int8_t MailMenuHighlight; // Currently highlighted text
    sf::Clock AnimClock;
    sf::Texture* pWhite;
    sf::Texture* pWallpaper;
    sf::Texture* pPhoneOpenTex; // Open/Close animation frames
    sf::Texture* pPhoneTex;
    sf::Texture* pHighlight;
    sf::Sprite Mask;
    sf::Sprite Wallpaper;
    sf::Sprite Header;
    sf::Sprite Overlay;
    sf::Sprite OverlayRed;
    sf::Sprite BlueHeader;
    sf::Sprite Highlight;
    sf::Text HeaderText;
    sf::Text Contacts[5];
    sf::Text MailMenuText[2];
    sf::Window* pWindow; // Needed to move mouse pointer
    PhoneMode* pMode;
    PhoneSD* pSD;
};
