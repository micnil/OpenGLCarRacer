#include "../include/Player.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
Player::Player(string lvlName,int strPlayer,char leftT, char rightT,int startPos)
{
    Player::loadOBJ("testBil.obj",vertices, uvs,normals);

    cubeXT = 0;
    cubeYT = startPos;
    cubeR = 0;
    cubeSpeed = 0;
    cubeRot = 0;
    cubeMaxSpeed = 100;
    cubeAcc = 66;
    cubeRot = 133.7;
    leftK = leftT;
    rightK = rightT;

    collArray.resize(512);
    image<rgb_pixel> img(lvlName);       //Laddar in kartans kollisioner
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

                //img[i][j].alpha
		}
	}
}

void Player::keyPress(float timeDelay){

    float   tempXT = cubeXT,
            tempYT = cubeYT;

    if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(leftK) == GLFW_PRESS)
        cubeR = cubeR + (cubeRot * timeDelay);

    if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(rightK) == GLFW_PRESS)
        cubeR = cubeR - (cubeRot * timeDelay);

    /*if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey('W') == GLFW_PRESS){
        calCubeSpeed(1, timeDelay);

    }
    else if(glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey('S') == GLFW_PRESS){
        calCubeSpeed(0, timeDelay);
    }
    else
        calCubeSpeed(2, timeDelay);*/
    calCubeSpeed(1, timeDelay);     //Konstant gas

    bool reducespeed = false;
    if( -0.7 < cos((cubeR*M_PI)/180) && cos((cubeR*M_PI)/180)< 0.7){
        if (collisionCheck(cubeXT, cubeYT) == 1){   //Två kollar för att skapa en glideffekt mot väggar
            cubeXT = tempXT;
            reducespeed = true;
        }
        if (collisionCheck(cubeXT, cubeYT) == 1){
            cubeYT = tempYT;
            cubeSpeed = 0;
        }
    }
    else{
        if (collisionCheck(cubeXT, cubeYT) == 1){
            cubeYT = tempYT;
            reducespeed = true;
        }
        if (collisionCheck(cubeXT, cubeYT) == 1){
            cubeXT = tempXT;
            cubeSpeed = 0;
        }
     }

     if (reducespeed == true){//Reducerar hastigheten om man krockar, dock inte så hastigheten blir noll
        if (cubeSpeed > 10)
            cubeSpeed -= cubeAcc * timeDelay * 5;
        else if (cubeSpeed < -10)
            cubeSpeed += cubeAcc * timeDelay * 5;
     }
    //Så att det inte blir idiot stora värden på cubeRot
    if(cubeRot > 360)
      cubeRot -= 360;
    if(cubeRot < -360)
      cubeRot += 360;
}

//State: 0=Backa, 1=Gasa, 2=Går ner/upp till noll
void Player::calCubeSpeed(int state, double timeDelay){

    if (cubeSpeed > cubeMaxSpeed)
        cubeSpeed -= cubeAcc * timeDelay * 5;
    else if (state == 1)
        cubeSpeed += cubeAcc * timeDelay;
    else if (state == 2){
        if (cubeSpeed > 0)
            cubeSpeed -= cubeAcc * timeDelay * 5;
        else
            cubeSpeed += cubeAcc * timeDelay * 5;
    }
    cubeXT = cubeXT + cos((cubeR*M_PI)/180)  * cubeSpeed * timeDelay;
    cubeYT = cubeYT + sin((cubeR*M_PI)/180)  * cubeSpeed * timeDelay;

}

bool Player::collisionCheck(float x, float y){
    int colW = 1 + strPlayer; //Kollar av kollisionen på väggen/ storleken på kuben plus väggens tjocklek(1)
    int maxH = 512/2, maxW = 512/2;
    //Laddar in värden för kollisions arrayen så den bara kollar av värden där kuben kan åka inom den "framen", +/-5 för att jag har ingen aning om varför, men ska vara där annars funkar den dåligt, kan ökas
    int checkBox = strPlayer + 1; //Hur många ut från mitten av kuben som ska kollas av. Kubens storlek(5) + väggens storlek(1) = 6
    double checkAreaUpp = cubeYT +checkBox, checkAreaNer = cubeYT -checkBox, checkAreaHoger = cubeXT +checkBox, checkAreaVanster = cubeXT -checkBox;

    //Kollar av så att man inte kollar utanför arrayen
    if (checkAreaUpp > maxH)
        checkAreaUpp = maxH;
    if (checkAreaNer < -maxH)
        checkAreaNer = -maxH;
    if (checkAreaHoger > maxW)
        checkAreaHoger = maxW;
    if (checkAreaVanster < -maxW)
        checkAreaVanster = -maxW;

    for(int i=checkAreaVanster;i<checkAreaHoger;i++)
    {
        for(int j=checkAreaNer;j<checkAreaUpp;j++)
        {
            if (collArray[i+maxH][j+maxW] == 1) //Kollision
            {
                if (i-colW < x && x < i+colW && j-strPlayer < y && y < j+colW)
                    return 1;
            }
            if (collArray[i+maxH][j+maxW] == 2) //Boost
            {
                if ( i-colW < x && x < i+colW && j-strPlayer < y && y < j+colW){
                    cubeSpeed = 200.0;
                    return 0;
                }
            }
        }
    }
    return 0;
    //Bilden laddas in från X0-XN och Y0-YN medans planet laddas in i origo i mitten
    //därför körs collArrray på +höjd/bredd
}
bool Player::loadOBJ(
    const char * path,
    std::vector < glm::vec3 > & out_vertices,
    std::vector < glm::vec2 > & out_uvs,
    std::vector < glm::vec3 > & out_normals
){
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

    FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		return false;
	}

	while( 1 ){
            char lineHeader[128];
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);

        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
        if (matches != 9){
            printf("File can't be read by our simple parser : ( Try exporting with other options\n");
            return false;
        }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }else{
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }
    // For each vertex of each triangle
    for( unsigned int i=0; i<vertexIndices.size(); i++ ){

   		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
    }
}
