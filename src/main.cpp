#include "game.hpp"

int main(int argc, char** argv)
{
    Game* pGame = new Game({"cg.npa", "nss.npa", "voice.npa"}, "boot.nss");
    pGame->Run();
    delete pGame;
}
