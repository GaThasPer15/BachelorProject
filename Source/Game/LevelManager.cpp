#include "LevelManager.h"
#include "../Game.h"

void RayEngine::LevelManager::ChangeLevel(const std::string &name) const{
    if(!_Game){
        return;
    }
    _Game->RequestLevelChange(name);
}