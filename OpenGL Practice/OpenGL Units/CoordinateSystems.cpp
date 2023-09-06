#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<Shader.h>
#include<stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>


using namespace std;
using namespace glm;

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);


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

	//Load GLAD and make sure it works!
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //Load the glad process with the procAddress being OS specific
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}







	//Create Verticies, colors, and tex coords
	float vertecies[] = {
		//Positions			//Colors			//Texture
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f


	};

	unsigned int indicies[] = {

		0,1,3,
		1,2,3

	};

	Shader shader = Shader("vertexShader.vs", "fragmentShader.fs");


	//-----------------------------------------


	/*
	* CONFIGURE VERTEX DATA
	*
	* VBO: Transfers vertex data from CPU to GPU
	* VAO: Holds vertex attributes
	* EBO: Stores indicies
	*
	* */

	//Create the VBO and VAO
	unsigned int VBO, VAO, EBO; //Create VBO and VAO and EBO
	glGenBuffers(1, &VBO); //Create a buffer and store address into VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO); //Bind the VAO

	//Put vertex data into VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Set the buffer type
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW); //Insert data into buffer

	//Put indicies into EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);


	//Set vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Format position data. Start at 0, have 3 data per points, stride by 8 * size of a float, void
	glEnableVertexAttribArray(0); //Enable the verticies to be inputed into the vertex shader location = 0

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Format color data. Start at 3, have 3 data per points, stride by 8 * size of a float, void
	glEnableVertexAttribArray(1); //Enable the colors to be inputed into the vertex shader location = 1

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // Format texture data. Start at 6, have 2 data per points, stride by 8 * size of a float, void
	glEnableVertexAttribArray(2); //Enable the texture coordinates to be inputed into the vertex shader location = 2

	//-----------------------------------------

	//TEXTURE PRESETS
	unsigned int textureID1, textureID2;
	glGenTextures(1, &textureID1); //Generate Texture and store address
	glBindTexture(GL_TEXTURE_2D, textureID1); //Bind the texture type to the address

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); //Repeat texture on S axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); //Repeat texture on T axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Mipmap linear for minifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Linear for magnifying

	//-----------------------------------------

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

	//-----------------------------------------


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


	shader.use();
	glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0); //Set uniform for sampler 1
	shader.setInt("texture2", 1); //Set uniform for sampler 2


	mat4 model = mat4(1.0f);
	model = rotate(model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
	mat4 view = mat4(1.0f);
	view = translate(view, vec3(0.0f, 0.0f, -3.0f));
	mat4 projection = mat4(1.0f);
	projection = perspective(radians(45.0f), 800.0f / 600.0f, .01f, 100.0f);

	unsigned int Matlocation = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(Matlocation, 1, GL_FALSE, value_ptr(model));

	Matlocation = glGetUniformLocation(shader.ID, "view");
	glUniformMatrix4fv(Matlocation, 1, GL_FALSE, value_ptr(view));

	Matlocation = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(Matlocation, 1, GL_FALSE, value_ptr(projection));



	//Render Loop
	while (!glfwWindowShouldClose(window)) //If we haven't closed the window
	{
		//Input detection
		ProcessInput(window);


		//Rendering
		glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureID2);




		glBindVertexArray(VAO); //Get the vertex attributes
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Buffering and event processing
		glfwSwapBuffers(window); //Swap window buffers
		glfwPollEvents(); //Check for triggers

	}

	glfwTerminate();
	return 0;

}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}


