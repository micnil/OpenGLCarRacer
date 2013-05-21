#include <vector>
#include <string>
#include <stdio.h>
#include <cstring>

#include <glm/glm.hpp>



#ifndef OBJLOADER_H_INCLUDED
#define OBJLOADER_H_INCLUDED

class objLoader{
public:

objLoader(const char* path);
~objLoader();

std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
std::vector<glm::vec2> uvs;

/*GLuint vertexBuffer;
GLuint normalBuffer;
GLuint uvBuffer;*/

private:

bool loadOBJ(const char* path);

};



#endif // OBJLOADER_H_INCLUDED
