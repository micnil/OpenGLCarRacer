#define GLEW_STATIC
#ifndef PLAYER_H
#define PLAYER_H
#define _USE_MATH_DEFINES
#include <cmath>
#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GL/glfw.h>
#include "SOIL.h"
#include "png++/png.hpp"
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace png;
using namespace std;

class Player
{
    public:
        Player(string lvlName,int strPlayer,char leftT, char rightT, int startPos);
        bool loadOBJ(
            const char * path,
            std::vector < glm::vec3 > & out_vertices,
            std::vector < glm::vec2 > & out_uvs,
            std::vector < glm::vec3 > & out_normals
            );
        void keyPress(float timeDelay);
        void calCubeSpeed(int state, double timeDelay);
        bool collisionCheck(float x, float y);


        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;



        char leftK, rightK;
        float cubeXT, cubeYT , cubeR , cubeSpeed;
        int cubeRot, cubeMaxSpeed ,cubeAcc, strPlayer;
        vector< vector<int> > collArray;

    protected:
    private:

};

#endif // PLAYER_H
