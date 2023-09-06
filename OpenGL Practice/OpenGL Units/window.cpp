#include<glad/glad.h>
#include<GLFW/glfw3.h>


using namespace std;

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);


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
	
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //apply window hints
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback); //Allows the user to adjust window size by calling FrameBufferSizeCallback fucntion. Links event

	//Load GLAD and make sure it works!
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //Load the glad process with the procAddress being OS specific
	{
		return -1;
	}

	return 0;


	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}


void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}