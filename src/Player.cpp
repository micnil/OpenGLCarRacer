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
	loadOBJ("car.obj", vertices, uvs, normals);



	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
	generateBuffers();

    //Hämta hörnpunkter för kollisionshantering
    up_right_values = getURVertexValues();
    up_left_values = getULVertexValues();
    down_right_values = getDRVertexValues();
    down_left_values = getDLVertexValues();

    collArray.resize(512);
    png::image<png::rgb_pixel> img("image/test2.png");       //Laddar in kartans kollisioner
    for(int i=0;i<512;i++) //Räknar ut kollision, allt svart(röd under 10, tihihi) kan man inte åka på
    {
        vector<int> row;        //Lägga in alla rader först för att kunna snedriva/spegla skiten(arrayen) bilden läses ej från 0.0)
        row.resize(512);

        collArray.push_back(row);
    }

    for(int i=0;i<512;i++) //Räknar ut kollision, allt svart kan man inte åka på
    {
		for(int j=0;j<512;j++)
		{
			if(img[i][j].red < 11 && img[i][j].green < 11 && img[i][j].blue < 11)
			{
				collArray[j].push_back( 1 );
			}
			else if(250 < img[i][j].green && img[i][j].green < 256){
				collArray[j].push_back( 2 );
			}
			else
                collArray[j].push_back( 0 );
		}
	}

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

   if (glfwGetKey(leftKey) == GLFW_PRESS){
        if(rot_speed>-rot_max_speed)
            rot_speed-=rot_acceleration*timeDelay;
    }
    else if ( glfwGetKey(rightKey) == GLFW_PRESS){
        if(rot_speed<rot_max_speed)
            rot_speed+=rot_acceleration*timeDelay;
    }
    else{
        if(rot_speed>0)
            rot_speed-=300*timeDelay;
        else if(rot_speed<0)
            rot_speed+=300*timeDelay;
    }
    if (glfwGetKey(upKey) == GLFW_PRESS){
        if(speed<max_speed)
            speed+=acceleration*timeDelay;
        else
            speed-=acceleration*timeDelay;
    }
    else if(glfwGetKey(downKey) == GLFW_PRESS){
        if(speed>-max_speed)
            speed-=acceleration*timeDelay;
    }
    else{
        if(speed>0)
            speed-=50*timeDelay;
        if(speed<0)
            speed+=50*timeDelay;
    }

    if(abs(speed)>7 && collisionCheck(timeDelay, r_pos)!=1)
        r_pos = r_pos + (rot_speed * timeDelay);
    if(collisionCheck(timeDelay, r_pos)!=1)
    {
        x_pos = x_pos + cos((r_pos*M_PI)/180)  * speed * timeDelay;
        y_pos = y_pos + sin((r_pos*M_PI)/180)  * speed * timeDelay;
    }
    else if (rot_speed > 100 && collisionCheck(timeDelay,r_pos+80)!=1)
    {
        x_pos = x_pos + cos(((r_pos+80)*M_PI)/180)  * speed * timeDelay;
        y_pos = y_pos + sin(((r_pos+80)*M_PI)/180)  * speed * timeDelay;
    }
    else if (rot_speed < -100 && collisionCheck(timeDelay,r_pos-80)!=1)
    {
        x_pos = x_pos + cos(((r_pos-80)*M_PI)/180)  * speed * timeDelay;
        y_pos = y_pos + sin(((r_pos-80)*M_PI)/180)  * speed * timeDelay;
    }


	float FoV = initialFoV - 5 * glfwGetMouseWheel();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV,(float)800 / (float)800, 0.1f, 513.0f );

	// Camera matrix
	glm::vec3 position = glm::vec3( 0.0f, 0.0f, -512.0f );
    glm::vec3 direction = glm::vec3( 0, 0, 1.0f );
    glm::vec3 up = glm::vec3( 0,  1.0f, 0);
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );
    ModelMatrix= glm::scale(glm::mat4(1.0f),glm::vec3(0.5f, 0.5f ,0.5f));
    ModelMatrix = glm::translate(ModelMatrix,glm::vec3(x_pos,y_pos,1.0f));//4
    ModelMatrix = glm::rotate(ModelMatrix, r_pos, glm::vec3( 0.0f, 0.0f, 1.0f ) );//3
    //ModelMatrix = glm::rotate(ModelMatrix, -90.0f, glm::vec3( 0.0f, 0.0f, 1.0f ) );//2
    //ModelMatrix = glm::rotate(ModelMatrix, 0.0f, glm::vec3( 0.0f, 0.0f, 1.0f ) );//2
	ModelMatrix = glm::rotate(ModelMatrix, 270.0f, glm::vec3( 1.0f, 0.0f, 0.0f ) );//1

}

void Player::generateBuffers()
{
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void Player::deleteBuffers()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
}

int Player::collisionCheck(float timeDelay, float rotation){
    // kolla hörnpunkterna endast
    int maxH = 512/2, maxW = 512/2;
    glm::vec4 corner_coords_DL = ModelMatrix * down_left_values;
    glm::vec4 corner_coords_UR = ModelMatrix * up_right_values;
    glm::vec4 corner_coords_DR = ModelMatrix * down_right_values;
    glm::vec4 corner_coords_UL = ModelMatrix * up_left_values;

    corner_coords_DL.x=corner_coords_DL.x+cos((rotation*M_PI)/180)  * speed * timeDelay;
    corner_coords_DL.y=corner_coords_DL.y+sin((rotation*M_PI)/180)  * speed * timeDelay;

    corner_coords_UR.x=corner_coords_UR.x+cos((rotation*M_PI)/180)  * speed * timeDelay;
    corner_coords_UR.y=corner_coords_UR.y+sin((rotation*M_PI)/180)  * speed * timeDelay;

    corner_coords_DR.x=corner_coords_DR.x+cos((rotation*M_PI)/180)  * speed * timeDelay;
    corner_coords_DR.y=corner_coords_DR.y+sin((rotation*M_PI)/180)  * speed * timeDelay;

    corner_coords_UL.x=corner_coords_UL.x+cos((rotation*M_PI)/180)  * speed * timeDelay;
    corner_coords_UL.y=corner_coords_UL.y+sin((rotation*M_PI)/180)  * speed * timeDelay;


    if (collArray[corner_coords_DL.x + maxW][corner_coords_DL.y + maxH] == 1 ||
        collArray[corner_coords_UR.x + maxW][corner_coords_UR.y + maxH] == 1 ||
        collArray[corner_coords_DR.x + maxW][corner_coords_DR.y + maxH] == 1 ||
        collArray[corner_coords_UL.x + maxW][corner_coords_UL.y + maxH] == 1 ){ //Kollision
            return 1;
    }
    if (collArray[corner_coords_DL.x + maxW][corner_coords_DL.y + maxH] == 2 ||
        collArray[corner_coords_UR.x + maxW][corner_coords_UR.y + maxH] == 2 ||
        collArray[corner_coords_DR.x + maxW][corner_coords_DR.y + maxH] == 2 ||
        collArray[corner_coords_UL.x + maxW][corner_coords_UL.y + maxH] == 2 ){ //speed
            speed=200;
            return 0;
    }

    return 0;
    //Bilden laddas in från X0-XN och Y0-YN medans planet laddas in i origo i mitten
    //därför körs collArrray på +höjd/bredd
}

glm::vec4 Player::getURVertexValues(){
    glm::vec4 coordinates = glm::vec4(indexed_vertices[0], 1);
    for(int i = 1; i<indexed_vertices.size(); ++i){
        if(indexed_vertices[i].x > coordinates.x) coordinates.x = indexed_vertices[i].x;
        if(indexed_vertices[i].y > coordinates.y) coordinates.y = indexed_vertices[i].y;
        if(indexed_vertices[i].z > coordinates.z) coordinates.z = indexed_vertices[i].z;
    }
    return coordinates;
}

glm::vec4 Player::getDLVertexValues(){
    glm::vec4 coordinates = glm::vec4(indexed_vertices[0], 1);
    for(int i = 1; i<indexed_vertices.size(); ++i){
        if(indexed_vertices[i].x < coordinates.x) coordinates.x = indexed_vertices[i].x;
        if(indexed_vertices[i].y < coordinates.y) coordinates.y = indexed_vertices[i].y;
        if(indexed_vertices[i].z < coordinates.z) coordinates.z = indexed_vertices[i].z;
    }
    return coordinates;
}

glm::vec4 Player::getDRVertexValues(){
    glm::vec4 coordinates = glm::vec4(indexed_vertices[0], 1);
    for(int i = 1; i<indexed_vertices.size(); ++i){
        if(indexed_vertices[i].x < coordinates.x) coordinates.x = indexed_vertices[i].x;
        if(indexed_vertices[i].y > coordinates.y) coordinates.y = indexed_vertices[i].y;
        if(indexed_vertices[i].z > coordinates.z) coordinates.z = indexed_vertices[i].z;
    }
    return coordinates;
}
glm::vec4 Player::getULVertexValues(){
    glm::vec4 coordinates = glm::vec4(indexed_vertices[0], 1);
    for(int i = 1; i<indexed_vertices.size(); ++i){
        if(indexed_vertices[i].x > coordinates.x) coordinates.x = indexed_vertices[i].x;
        if(indexed_vertices[i].y < coordinates.y) coordinates.y = indexed_vertices[i].y;
        if(indexed_vertices[i].z < coordinates.z) coordinates.z = indexed_vertices[i].z;
    }
    return coordinates;
}
