#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <cmath>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"



glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 ModelMatrix;



glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}
glm::mat4 getModelMatrix(){
	return ModelMatrix;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0.0f, 0.0f, -200.0f );
glm::vec3 direction = glm::vec3( 0, 0, 1.0f );
glm::vec3 up = glm::vec3( 0,  1.0f, 0);

// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float x_pos=0.0, y_pos=0.0, r_pos=0.0 , model_speed=0.0 , rot_speed=0.0f, rot_max_speed=100.0f,rot_acceleration=200.0f, speed=0.0f , max_speed = 80.0f, acceleration=120.0f;
static int state=0;


void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float timeDelay = float(currentTime - lastTime);

   if ((glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey('A') == GLFW_PRESS)){
        if(rot_speed>-rot_max_speed)
            rot_speed-=rot_acceleration*timeDelay;
    }
    else if ((glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey('D') == GLFW_PRESS)){
        if(rot_speed<rot_max_speed)
            rot_speed+=rot_acceleration*timeDelay;
    }
    else{
        if(rot_speed>0)
            rot_speed-=150*timeDelay;
        else if(rot_speed<0)
            rot_speed+=150*timeDelay;
    }
    if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey('W') == GLFW_PRESS){
        if(speed<max_speed)
            speed+=acceleration*timeDelay;
    }
    else if(glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey('S') == GLFW_PRESS){
        if(speed>-max_speed)
            speed-=acceleration*timeDelay;
    }
    else{
        if(speed>0)
            speed-=50*timeDelay;
        if(speed<0)
            speed+=50*timeDelay;
    }

    if(abs(speed)>3)
        r_pos = r_pos + (rot_speed * timeDelay);
    x_pos=x_pos+cos((r_pos*M_PI)/180)  * speed * timeDelay;
    y_pos=y_pos+sin((r_pos*M_PI)/180)  * speed * timeDelay;


	float FoV = initialFoV - 5 * glfwGetMouseWheel();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 512.0f);

	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

    ModelMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(x_pos,y_pos,1.0f));//4
    ModelMatrix = glm::rotate(ModelMatrix, r_pos, glm::vec3( 0.0f, 0.0f, 1.0f ) );//3
    ModelMatrix = glm::rotate(ModelMatrix, -90.0f, glm::vec3( 0.0f, 0.0f, 1.0f ) );//2
	ModelMatrix = glm::rotate(ModelMatrix, 270.0f, glm::vec3( 1.0f, 0.0f, 0.0f ) );//1


	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

