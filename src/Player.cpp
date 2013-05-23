#include "../include/Player.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Player::Player(int startpos_x,int startpos_y,char up_key, char down_key, char left_key, char right_key)
{

    x_pos = startpos_x;
    y_pos = startpos_y;
    leftKey = left_key;
    rightKey = right_key;
    upKey = up_key;
    downKey = down_key;

    // Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	loadOBJ("testBil2.obj", vertices, uvs, normals);

	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

}

glm::mat4 Player::getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 Player::getProjectionMatrix(){
	return ProjectionMatrix;
}
glm::mat4 Player::getModelMatrix(){
	return ModelMatrix;
}

void Player::computeMatricesFromInputs(float timeDelay){

   if ((glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(leftKey) == GLFW_PRESS)){
        if(rot_speed>-rot_max_speed)
            rot_speed-=rot_acceleration*timeDelay;
    }
    else if ((glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(rightKey) == GLFW_PRESS)){
        if(rot_speed<rot_max_speed)
            rot_speed+=rot_acceleration*timeDelay;
    }
    else{
        if(rot_speed>0)
            rot_speed-=250*timeDelay;
        else if(rot_speed<0)
            rot_speed+=250*timeDelay;
    }
    if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(upKey) == GLFW_PRESS){
        if(speed<max_speed)
            speed+=acceleration*timeDelay;
    }
    else if(glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(downKey) == GLFW_PRESS){
        if(speed>-max_speed)
            speed-=acceleration*timeDelay;
    }
    else{
        if(speed>0)
            speed-=50*timeDelay;
        if(speed<0)
            speed+=50*timeDelay;
    }

    if(abs(speed)>7)
        r_pos = r_pos + (rot_speed * timeDelay);
    x_pos=x_pos+cos((r_pos*M_PI)/180)  * speed * timeDelay;
    y_pos=y_pos+sin((r_pos*M_PI)/180)  * speed * timeDelay;


	float FoV = initialFoV - 5 * glfwGetMouseWheel();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 512.0f);

	// Camera matrix
	glm::vec3 position = glm::vec3( 0.0f, 0.0f, -200.0f );
    glm::vec3 direction = glm::vec3( 0, 0, 1.0f );
    glm::vec3 up = glm::vec3( 0,  1.0f, 0);
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

    ModelMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(x_pos,y_pos,1.0f));//4
    ModelMatrix = glm::rotate(ModelMatrix, r_pos, glm::vec3( 0.0f, 0.0f, 1.0f ) );//3
    ModelMatrix = glm::rotate(ModelMatrix, -90.0f, glm::vec3( 0.0f, 0.0f, 1.0f ) );//2
	ModelMatrix = glm::rotate(ModelMatrix, 270.0f, glm::vec3( 1.0f, 0.0f, 0.0f ) );//1

}
