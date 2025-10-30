#include <iostream>
#include "../../MathLib/MathLib.h"

namespace GTP{
    namespace Map{
        void CalcTileCoord(Vector2** tiles, int &iSize, int mapWidth, int mapHeigh, float hexScale){
            iSize = mapWidth * mapHeigh;
            *tiles = new Vector2[iSize];
            Vector2 Coord;
            Vector2 moveVecHorizontal = {hexScale, 0}, moveVecVertical = {hexScale*0.5f, static_cast<float>(sqrt(0.75)*hexScale)};

            *tiles[0] = {0.0f-mapWidth*0.5f*hexScale, 0.0f-mapHeigh*0.5f*hexScale};
            for(int i=1; i<=iSize; i++){
                if(i%mapWidth){    
                    (*tiles)[i] = GTPMath::Vector2Add((*tiles)[i-1], moveVecHorizontal);
                }
                else{
                    (*tiles)[i] = GTPMath::Vector2Add((*tiles)[i-mapWidth], moveVecVertical);
                    
                    moveVecVertical.x = moveVecVertical.x * (-1);
                }
            }
        }


        
    };
};