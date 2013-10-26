#include "game.hpp"

int main(int argc, char** argv)
{
    // Note that scripts call .nss files, NOT nsb. This is a hack
    Game* pGame = new Game({"cg.npa", "nss.npa", "voice.npa"}, "nss/0_boot.nsb");
    pGame->Run();
    delete pGame;
}
