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
#include "sginterpreter.hpp"
#include "phone.hpp"
#include "game.hpp"
#include "phonemodesendmailedit.hpp"
#include "phonemodedefaultoperatable.hpp"
#include "phonemodeaddressbook.hpp"
#include "phonemodemailmenu.hpp"
#include "phonemodereceivingmail.hpp"
#include "phonesd.hpp"
#include <SFML/Graphics/Texture.hpp>

enum PhoneState
{
    PHONE_CLOSING = 0,
    PHONE_OPENING = 1,
    PHONE_OPENING_DONE, // Switch from last opening animation frame to open phone frame
    PHONE_CLOSING_DONE, // Same as above, except that phone needs to be removed
    PHONE_OPEN,
    PHONE_CLOSED
};

// UNUSED
enum PhoneSDText
{
    PHONE_SD_TEXT_NEW_MAIL = 0,
    PHONE_SD_TEXT_UNK1 = 1,
    PHONE_SD_TEXT_UNK2 = 2,
    PHONE_SD_TEXT_MAX = 3
};
const int16_t PHONE_SD_TEXT_TEX_X[PHONE_SD_TEXT_MAX] =
{
    135, 168, 200
};

const char* HeaderString[] =
{
    "アドレス帳",
    "受信メールBOX",
    "送信メールBOX"
};

const int16_t WALLPAPER_WIDTH = 220;
const int16_t WALLPAPER_HEIGHT = 244;

static const sf::IntRect OverlayContentTexRect[] =
{
    sf::IntRect(717, 9, 49, 29),
    sf::IntRect(880, 576, 50, 38)
};

Phone::Phone(sf::Drawable* pDrawable, sf::Window* pWindow) :
DrawableBase(pDrawable, -1, DRAWABLE_TEXTURE),
State(PHONE_CLOSED),
ShowSD(false),
MailReceived(false),
Mode(MODE_POWER_OFF),
pWindow(pWindow),
pMode(nullptr),
OverlayAnimProgress(0)
{
    pWallpaper = LoadTextureFromFile("cg/sys/phone/pwcg101.png", sf::IntRect());
    pPhoneTex = LoadTextureFromFile("cg/sys/phone/phone_01.png", sf::IntRect());
    pPhoneOpenTex = LoadTextureFromFile("cg/sys/phone/phone_open_anim.png", sf::IntRect());

    BlueHeader.setTexture(*pPhoneTex);
    BlueHeader.setPosition(BLUE_HEADER_POS_X, BLUE_HEADER_POS_Y);
    Overlay.setTexture(*pPhoneTex);
    Overlay.setPosition(PHONE_OVERLAY_POS_X, PHONE_OVERLAY_POS_Y);
    Overlay.setTextureRect(sf::IntRect(PHONE_NEW_MAIL_TEX_X, PHONE_NEW_MAIL_TEX_Y, PHONE_NEW_MAIL_WIDTH, PHONE_NEW_MAIL_HEIGHT));
    OverlayContent.setTexture(*pPhoneTex);
    Header.setTexture(*pPhoneTex);
    Header.setPosition(PHONE_HEADER_POS_X, PHONE_HEADER_POS_Y);
    Header.setTextureRect(sf::IntRect(PHONE_HEADER_TEX_X, PHONE_HEADER_TEX_Y, PHONE_HEADER_WIDTH, PHONE_HEADER_HEIGHT));

    Wallpaper.setPosition(PHONE_WALLPAPER_X, PHONE_WALLPAPER_Y);
    ToSprite()->setPosition(PHONE_POS_X, PHONE_POS_Y);

    for (int i = 0; i < MODE_INVALID; ++i)
        PhoneModes[i] = nullptr;

    PhoneModes[MODE_MAIL_MENU] = new PhoneModeMailMenu(this);
    PhoneModes[MODE_DEFAULT_OPERATABLE] = new PhoneModeDefaultOperatable(this);
    PhoneModes[MODE_ADDRESS_BOOK] = new PhoneModeAddressBook(this);
    PhoneModes[MODE_SEND_MAIL_EDIT] = new PhoneModeSendMailEdit(this);
    PhoneModes[MODE_RECEIVING_MAIL] = new PhoneModeReceivingMail(this);
    pSD = new PhoneSD();
}

Phone::~Phone()
{
    for (int i = 0; i < MODE_INVALID; ++i)
        delete PhoneModes[i];

    delete pPhoneTex;
    delete pPhoneOpenTex;
    delete pWallpaper;
    delete pSD;
}

void Phone::UpdateOpenMode(int32_t OpenMode)
{
    if (!(State == PHONE_OPEN || State == PHONE_CLOSED))
        return;

    ToSprite()->setTexture(*pPhoneOpenTex);
    State = OpenMode;
    UpdateAnim();
    AnimClock.restart();
}

void Phone::Draw(sf::RenderWindow* pWindow)
{
    // Update
    if (!(AnimClock.getElapsedTime().asMilliseconds() < PHONE_ANIM_SPEED
        || State == PHONE_OPEN || State == PHONE_CLOSED))
    {
        UpdateAnim();
        AnimClock.restart();
    }

    if (OverlayClock.getElapsedTime().asMilliseconds() > 500)
        UpdateOverlayAnim();

    // Draw
    DrawableBase::Draw(pWindow);
    if (State == PHONE_OPEN)
    {
        if (Mode != MODE_POWER_OFF && Mode != MODE_RECEIVING_MAIL)
        {
            pWindow->draw(Wallpaper);
            pWindow->draw(Header);
            if (MailReceived)
            {
                pWindow->draw(Overlay);
                pWindow->draw(OverlayContent);
            }
        }
        if (Mode == MODE_ADDRESS_CONFIRM_DIAL)
            PhoneModes[MODE_ADDRESS_BOOK]->Draw(pWindow);
        if (Mode == MODE_ADDRESS_CONFIRM_MAIL)
            PhoneModes[MODE_SEND_MAIL_EDIT]->Draw(pWindow);
        if (pMode)
            pMode->Draw(pWindow);
    }
    if (ShowSD)
        pSD->Draw(pWindow);
}

void Phone::UpdateAnim()
{
    //
    // See: cg/sys/phone/phone_open_anim.png
    //

    // Animation finished in last call
    if (State == PHONE_OPENING_DONE)
    {
        sf::IntRect ClipArea(PHONE_TEX_X, PHONE_TEX_Y, PHONE_WIDTH, PHONE_HEIGHT);
        ToSprite()->setTexture(*pPhoneTex);
        ToSprite()->setTextureRect(ClipArea);
        State = PHONE_OPEN;
        return;
    }
    else if (State == PHONE_CLOSING_DONE)
    {
        delete pDrawable;
        pDrawable = new sf::Sprite;
        ToSprite()->setPosition(PHONE_POS_X, PHONE_POS_Y);
        State = PHONE_CLOSED;
        return;
    }

    // Animation is not done: set next frame
    sf::IntRect ClipArea(AnimColumn * PHONE_WIDTH, AnimRow * PHONE_HEIGHT, PHONE_WIDTH, PHONE_HEIGHT);
    ToSprite()->setTextureRect(ClipArea);

    // Check if animation is done
    switch (State)
    {
        case PHONE_OPENING:
            if (AnimColumn == 0 && AnimRow == 0)
                State = PHONE_OPENING_DONE;
            break;
        case PHONE_CLOSING:
            if (AnimColumn == PHONE_ANIM_COLUMN_MAX && AnimRow == PHONE_ANIM_ROW_MAX)
                State = PHONE_CLOSING_DONE;
            break;
    }

    // Advance animation progress
    switch (State)
    {
        case PHONE_OPENING: --AnimColumn; break;
        case PHONE_CLOSING: ++AnimColumn; break;
        case PHONE_OPENING_DONE: case PHONE_CLOSING_DONE: return;
    }

    // Go to previous row
    if (AnimColumn == PHONE_ANIM_UNDERFLOW)
    {
        AnimColumn = PHONE_ANIM_COLUMN_MAX;
        --AnimRow;
    }
    // Go to next row
    else if (AnimColumn > PHONE_ANIM_COLUMN_MAX)
    {
        AnimColumn = 0;
        ++AnimRow;
    }
}

void Phone::UpdateOverlayAnim()
{
    const sf::IntRect& TexRect = OverlayContentTexRect[OverlayAnimProgress];
    OverlayContent.setTextureRect(TexRect);
    OverlayContent.setPosition(834 - TexRect.width / 2, 208 - TexRect.height / 2);
    OverlayAnimProgress = !OverlayAnimProgress;
    OverlayClock.restart();
}

void Phone::UpdateMode(uint8_t NewMode)
{
    if (NewMode == Mode || NewMode == MODE_INVALID)
        return;

    pMode = PhoneModes[NewMode];
    switch (NewMode)
    {
        case MODE_DEFAULT:
            Wallpaper.setTexture(*pWallpaper);
            Wallpaper.setTextureRect(sf::IntRect(0, 0, WALLPAPER_WIDTH, WALLPAPER_HEIGHT));
            break;
        default:
            if (pMode)
                pMode->OnOpen(Mode);
    }
    Mode = NewMode;
}

void Phone::MailReceive(int32_t Show)
{
    switch (Show)
    {
        case PHONE_CLOSING: MailReceived = false; break;
        case PHONE_OPENING: MailReceived = true; UpdateOverlayAnim(); break;
        default: assert(false); break;
    }
}

void Phone::SetDate(int32_t Date)
{
    string DateStr = boost::lexical_cast<string>(Date);
    if (DateStr.size() == 3) DateStr = string("0") + DateStr;
    pSD->SetDate(DateStr);
}

void Phone::SDDisplay(int32_t Show)
{
    switch (Show)
    {
        case PHONE_CLOSING: ShowSD = false; break;
        case PHONE_OPENING: ShowSD = true; break;
        default: assert(false);
    }
}

void Phone::SetPriority(int32_t Priority)
{
    this->Priority = Priority;
}

void Phone::MouseMoved(sf::Vector2i Pos)
{
    if (pMode)
        pMode->MouseMoved(Pos);
}

void Phone::LeftMouseClicked(sf::Vector2i Pos)
{
    if (!(Pos.x > PHONE_WALLPAPER_X && Pos.x < PHONE_WALLPAPER_X + WALLPAPER_WIDTH &&
          Pos.y > PHONE_WALLPAPER_Y && Pos.y < PHONE_WALLPAPER_Y + WALLPAPER_HEIGHT))
        return;

    if (pMode)
        UpdateMode(pMode->LeftMouseClicked());
}

void Phone::RightMouseClicked(SGInterpreter* pInterpreter)
{
    switch (Mode)
    {
        case MODE_DEFAULT:
            pInterpreter->PhoneToggle();
            break;
        default:
            if (pMode)
                UpdateMode(pMode->RightMouseClicked());
    }
}

void Phone::SetPhoneCallAllowMask(uint16_t Mask)
{
    PhoneModeAddressBook* pMode = (PhoneModeAddressBook*)PhoneModes[MODE_ADDRESS_BOOK];
    pMode->CallAllowedMask = Mask;
}

void Phone::AddressMenuHighlight(int32_t Index)
{
    PhoneModeAddressBook* pMode = (PhoneModeAddressBook*)PhoneModes[MODE_ADDRESS_BOOK];
    pMode->SetHighlight(Index);
}

void Phone::PhoneSendMailEdit(const string& Subject, const string& Sender, const string& Body)
{
    PhoneModeSendMailEdit* pMode = (PhoneModeSendMailEdit*)PhoneModes[MODE_SEND_MAIL_EDIT];
    pMode->SetText(Subject, Sender, Body);
}

void Phone::AddressbookSet(uint8_t Index)
{
    PhoneModeAddressBook* pMode = (PhoneModeAddressBook*)PhoneModes[MODE_ADDRESS_BOOK];
    pMode->AddContact(Index);
}

void Phone::AddressbookReset(uint8_t Index)
{
    PhoneModeAddressBook* pMode = (PhoneModeAddressBook*)PhoneModes[MODE_ADDRESS_BOOK];
    pMode->DelContact(Index);
}
