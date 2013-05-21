#ifndef LEVEL_H
#define LEVEL_H
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glfw.h>
#include <cmath>
#include "SOIL.h"
#include "png++/png.hpp"

using namespace png;
using namespace std;

class Level
{
    public:
        Level(int lvlSize);
        float lvlModel[20];
        GLuint lvlElements[6];


    protected:
    private:
};

#endif // LEVEL_H
