#include "UserGame.hpp"


int main(int argc, char *argv[]) {
    RayEngine::Game *game = User_CreateGame();
    game->Run();
    game->Shutdown();
    delete game;
    
    return 0;
}
