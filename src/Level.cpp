#include "../include/Level.h"

Level::Level(int lvlSize)
{
    float lvlModelT[20] = {//  Position         Texcoords
        -lvlSize,  lvlSize, 0.0f, 0.0f, 1.0f, // Top-left
         lvlSize,  lvlSize, 0.0f, 1.0f, 1.0f, // Top-right
         lvlSize, -lvlSize, 0.0f, 1.0f, 0.0f, // Bottom-right
        -lvlSize, -lvlSize, 0.0f, 0.0f, 0.0f  // Bottom-left
    };
    for (int i=0;i<20;i++)
        lvlModel[i] = lvlModelT[i];

    float lvlElementsT[6] = {
        0, 1, 2,
        2, 3, 0
    };
    for (int i=0;i<6;i++)
        lvlElements[i] = lvlElementsT[i];

}




