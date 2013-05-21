#define GLEW_STATIC
#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>


using namespace std;

class Player
{
public:
    Player(int startpos_x,int startpos_y, char left_key, char right_key);

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;



	glm::mat4 getViewMatrix();
	glm::mat4 getModelMatrix();
	glm::mat4 getProjectionMatrix();


	void computeMatricesFromInputs();

    // Initial Field of View
    float initialFoV = 45.0f;

    float x_pos, y_pos, r_pos=0.0 , model_speed=0.0 , rot_speed=0.0f, rot_max_speed=150.0f,rot_acceleration=200.0f, speed=0.0f , max_speed = 80.0f, acceleration=120.0f;
    char leftKey, rightKey;




private:
	glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;
    glm::mat4 ModelMatrix;
};

#endif // PLAYER_H
