#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<Shader.h>
#include<Camera.h>
#include<stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>


using namespace std;
using namespace glm;

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xMousePos, double yMousePos);
void ScollCallback(GLFWwindow* window, double xScroll, double yScroll);


Camera camera(vec3(0.0f, 0.0f, 3.0f));

float lastX = 300.0f, lastY = 400.0f; //Last X and Y position our mouse was at


float deltaTime = 0.0f; //time in between frames
float lastFrame = 0.0f; //time of the last frame



int main(void)
{

	//Initialize GLFW and set OpenGL version
	glfwInit(); //Initalize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Set OpenGL version major
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Set OpenGL version minor
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Make sure we only use OpenGL core


	//Create window and make sure it works
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL); //Create a 800x600 window
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //apply window hints
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback); //Allows the user to adjust window size by calling FrameBufferSizeCallback fucntion. Links event
	glfwSetCursorPosCallback(window, MouseCallback); //Whenever we move the mouse, MouseCallback is called
	glfwSetScrollCallback(window, ScollCallback);

	//Load GLAD and make sure it works!
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //Load the glad process with the procAddress being OS specific
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//----------------------------------------------------------------------------



	//Create Verticies, and tex coords
	float vertecies[] = {
		//Positions				TexCoords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f


	};

	vec3 cubePositions[] = {

		vec3(0.0f, 0.0f, -6.0f),

	};

	vec3 lightPosition = vec3(1.2f, 0.0f, -6.0f);




	Shader shader = Shader("vertexShader.vs", "fragmentShader.fs");
	Shader lightShader = Shader("lightVertexShader.vs", "lightFragmentShader.fs");


	//----------------------------------------------------------------------------


	/*
	* CONFIGURE VERTEX DATA
	*
	* VBO: Transfers vertex data from CPU to GPU
	* VAO: Holds vertex attributes
	* EBO: Stores indicies
	*
	* */

	//Create the VBO and VAO
	unsigned int VBO, VAO, lightVAO; //Create VBO and VAO 
	glGenVertexArrays(1, &VAO); //Generate an array and store address into VAO
	glGenBuffers(1, &VBO); //Create a buffer and store address into VBO


	//CUBE

	//Put vertex data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Set the buffer type
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW); //Insert data into buffer


	glBindVertexArray(VAO); //Bind the VAO

	//Set vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Format position data. Start at 0, have 3 data per points, stride by 5 * size of a float, void
	glEnableVertexAttribArray(0); //Enable the verticies to be inputed into the vertex shader location = 0


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // Format texture data. Start at 6, have 2 data per points, stride by 5 * size of a float, void
	glEnableVertexAttribArray(1); //Enable the texture coordinates to be inputed into the vertex shader location = 1

	//LIGHT


	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Format position data. Start at 0, have 3 data per points, stride by 5 * size of a float, void
	glEnableVertexAttribArray(0); //Enable the verticies to be inputed into the vertex shader location = 0


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // Format texture data. Start at 6, have 2 data per points, stride by 5 * size of a float, void
	glEnableVertexAttribArray(1); //Enable the texture coordinates to be inputed into the vertex shader location = 1






	//----------------------------------------------------------------------------

	//TEXTURE PRESETS
	unsigned int textureID1, textureID2;
	glGenTextures(1, &textureID1); //Generate Texture and store address
	glBindTexture(GL_TEXTURE_2D, textureID1); //Bind the texture type to the address

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); //Repeat texture on S axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); //Repeat texture on T axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Mipmap linear for minifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Linear for magnifying


	//----------------------------------------------------------------------------



	//TEXTURE 1
	int width, height, nrChannels;
	unsigned char* imageData = stbi_load("Crate.jpg", &width, &height, &nrChannels, 0); //load image
	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); //generate the texture with image
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else cout << "FAILED TO LOAD TEXTURE:\n" << stbi_failure_reason() << endl;

	stbi_image_free(imageData);



	//----------------------------------------------------------------------------



	//TEXTURE 2
	glGenTextures(1, &textureID2); //Generate Texture and store address
	glBindTexture(GL_TEXTURE_2D, textureID2);
	stbi_set_flip_vertically_on_load(true);
	imageData = stbi_load("Renamon.png", &width, &height, &nrChannels, 0); //load image
	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData); //generate the texture with image
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else cout << "FAILED TO LOAD TEXTURE:\n" << stbi_failure_reason() << endl;



	stbi_image_free(imageData);



	//Lighting

	shader.use();
	glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0); //Set uniform for sampler 1
	shader.setInt("texture2", 1); //Set uniform for sampler 2

	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 objectColor = vec3(1.0f, 0.5f, 0.31f);

	unsigned int lightLocation = glGetUniformLocation(shader.ID, "lightColor");
	glUniform3fv(lightLocation, 1, value_ptr(lightColor));

	lightLocation = glGetUniformLocation(shader.ID, "objectColor");
	glUniform3fv(lightLocation, 1, value_ptr(objectColor));




	//----------------------------------------------------------------------------




	//Render Loop
	while (!glfwWindowShouldClose(window)) //If we haven't closed the window
	{
		//Input detection
		ProcessInput(window);


		//Rendering
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID1); //bind texture based on texture ID
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureID2);

		
	

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;



		//Camera
		mat4 view = mat4(1.0f);
		view = camera.GetViewMatrix();
		unsigned int Matlocation = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(Matlocation, 1, GL_FALSE, value_ptr(view));

		//Projection
		mat4 projection = mat4(1.0f);
		projection = perspective(radians(camera.Zoom), 800.0f / 600.0f, .01f, 100.0f);


		Matlocation = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(Matlocation, 1, GL_FALSE, value_ptr(projection));

		glBindVertexArray(VAO); //Get the vertex attributes

		mat4 model =  translate(model, cubePositions[0]) * scale(model, vec3(50.0f));
		Matlocation = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(Matlocation, 1, GL_FALSE, value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);




		lightShader.use();

		Matlocation = glGetUniformLocation(lightShader.ID, "view");
		glUniformMatrix4fv(Matlocation, 1, GL_FALSE, value_ptr(view));
		Matlocation = glGetUniformLocation(lightShader.ID, "projection");
		glUniformMatrix4fv(Matlocation, 1, GL_FALSE, value_ptr(projection));

		model = mat4(1.0f);
		model = translate(model, lightPosition) * scale(model, vec3(0.2f)) * model;
		Matlocation = glGetUniformLocation(lightShader.ID, "model");
		glUniformMatrix4fv(Matlocation, 1, GL_FALSE, value_ptr(model));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);




		//Buffering and event processing
		glfwSwapBuffers(window); //Swap window buffers
		glfwPollEvents(); //Check for triggers

	}

	glfwTerminate();
	return 0;

}


//Called when user adjusts window
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//Process keys
void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

}

//Called when user moves mouse
void MouseCallback(GLFWwindow* window, double xMousePos, double yMousePos)
{
	float xPos = static_cast<float>(xMousePos);
	float yPos = static_cast<float>(yMousePos);

	float xOffset = xPos - lastX;
	float yOffset = yPos - lastY;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

//Called when user scrolls
void ScollCallback(GLFWwindow* window, double xScroll, double yScroll)
{
	camera.ProcessMouseScroll(static_cast<float>(yScroll));
}