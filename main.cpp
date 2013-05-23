// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <include/Player.h>

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle( "OpenGl" );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Load the texture
	GLuint Texture = loadDDS("bild.DDS");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    //Skapa VAO
	GLuint VertexArrayID[2];

	//Skapa buffers
    GLuint vertexbuffer[2];

    GLuint uvbuffer[2];

    GLuint normalbuffer[2];

    GLuint elementbuffer[2];

    //Skapa spelare 1
    Player player1(0,0,'W','S','A','D');

    glGenVertexArrays(1, &VertexArrayID[0]);
    glBindVertexArray(VertexArrayID[0]);
	// Load it into a VBO
	glGenBuffers(1, &vertexbuffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, player1.indexed_vertices.size() * sizeof(glm::vec3), &player1.indexed_vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glGenBuffers(1, &uvbuffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, player1.indexed_uvs.size() * sizeof(glm::vec2), &player1.indexed_uvs[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glGenBuffers(1, &normalbuffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, player1.indexed_normals.size() * sizeof(glm::vec3), &player1.indexed_normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
	// Generate a buffer for the indices as well
	glGenBuffers(1, &elementbuffer[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, player1.indices.size() * sizeof(unsigned short), &player1.indices[0] , GL_STATIC_DRAW);

    glBindVertexArray(0);
	/// FÖRSÖK TILL SPELARE 2!

	 //Skapa spelare 2
    Player player2(5,5,'U','J','H','K');

    glGenVertexArrays(1, &VertexArrayID[1]);
    glBindVertexArray(VertexArrayID[1]);
	// Load it into a VBO
	glGenBuffers(1, &vertexbuffer[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, player2.indexed_vertices.size() * sizeof(glm::vec3), &player2.indexed_vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    glGenBuffers(1, &uvbuffer[1]);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, player2.indexed_uvs.size() * sizeof(glm::vec2), &player2.indexed_uvs[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
    glGenBuffers(1, &normalbuffer[1]);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, player2.indexed_normals.size() * sizeof(glm::vec3), &player2.indexed_normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

	// Generate a buffer for the indices as well
	glGenBuffers(1, &elementbuffer[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, player2.indices.size() * sizeof(unsigned short), &player2.indices[0] , GL_STATIC_DRAW);


    glBindVertexArray(0);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

        // glfwGetTime is called only once, the first time this function is called
        static double lastTime = glfwGetTime();

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float timeDelay = float(currentTime - lastTime);

        player1.computeMatricesFromInputs(timeDelay);
        player2.computeMatricesFromInputs(timeDelay);

    std::cout << "player 1 modelMatrix.[0][0] = " << player1.getModelMatrix()[0][0] << "\n";
    std::cout << "player 2 modelMatrix.[0][0] = " << player2.getModelMatrix()[0][0] << "\n";
    std::cout << "\n";
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ModelMatrix;
		glm::mat4 MVP;

		// Compute the MVP matrix from keyboard and mouse input
		ProjectionMatrix = player1.getProjectionMatrix();
		ViewMatrix = player1.getViewMatrix();
		ModelMatrix = player1.getModelMatrix();
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glm::vec3 lightPos = glm::vec3(0,0,-100);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

        glBindVertexArray(VertexArrayID[0]);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer[0]);

		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			player1.indices.size(),    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
		);

        glBindVertexArray(0);

        // Compute the MVP matrix from keyboard and mouse input
		//uProjectionMatrix = player2.getProjectionMatrix();
		//ViewMatrix = player2.getViewMatrix();
		ModelMatrix = player2.getModelMatrix();
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glBindVertexArray(VertexArrayID[1]);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer[1]);

		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			player2.indices.size(),    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
		);

        glBindVertexArray(0);

		// Swap buffers

        // For the next frame, the "last time" will be "now"
        lastTime = currentTime;
		glfwSwapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ) );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer[0]);
	glDeleteBuffers(1, &uvbuffer[0]);
	glDeleteBuffers(1, &normalbuffer[0]);
	glDeleteBuffers(1, &elementbuffer[0]);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID[0]);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

