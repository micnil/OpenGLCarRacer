#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <cmath>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <windows.h>
#include <mmsystem.h>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include "png++/png.hpp"

using namespace std;

class Player
{
public:
    Player(int startpos_x,int startpos_y, int up_key, int down_key, int left_key, int right_key);

    //vetices and attributes
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

    //VAO
    GLuint VertexArrayID;

    //Buffers
    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    GLuint elementbuffer;

	glm::mat4 getViewMatrix();
	glm::mat4 getModelMatrix();
	glm::mat4 getProjectionMatrix();

    //Hämta hörnpunkter på bilen för kollisionshantering UR= up-right, UL=up-left, DR=down-right, DL=down-left.
    glm::vec4 getURVertexValues();
    glm::vec4 getULVertexValues();
    glm::vec4 getDRVertexValues();
    glm::vec4 getDLVertexValues();

    glm::vec4 down_left_values;
    glm::vec4 up_right_values;
    glm::vec4 down_right_values;
    glm::vec4 up_left_values;

	void computeMatricesFromInputs(float deltaTime);
	int collisionCheck(float timeDelay, float rotation);
	void generateBuffers();
	void deleteBuffers();

    // Initial Field of View
    float initialFoV = 52.0f;

    float x_pos, y_pos, r_pos=180.0 , model_speed=0.0 , rot_speed=0.0f, rot_max_speed=150.0f,rot_acceleration=600.0f, speed=0.0f , max_speed = 170.0f, acceleration=120.0f;
    int upKey, downKey, leftKey, rightKey;

    vector< vector<int> > collArray;

private:
	glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 ModelMatrix;
};

#endif // PLAYER_H
