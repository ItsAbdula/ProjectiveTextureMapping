#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OpenGLWrapper.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 20.0f, 2.0f);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewInit();

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	auto cam_programID = build_program("Camera");
	auto lighting = build_program("Lighting_Specular");
	auto lamp = build_program("Lighting_Lamp");
	auto lightmap = build_program("Lighting_Maps");
	auto texture_shader = build_program("Texture");

	auto cube = make_mesh("cube.obj");

	auto black = load_image("black.png");
	auto magenta = load_image("magenta.png");
	auto orange = load_image("orange.png");
	auto white = load_image("white.png");
	auto transparent = load_image("transparent.png");

	auto container_diffuse = load_image("container2.png");
	auto container_specular = load_image("container2_specular.png");

	auto defaultMaterial = new Material(lightmap, orange, transparent);
	auto cubeMaterial = new Material(lightmap, container_diffuse, container_specular);
	auto planeMaterial = new Material(lightmap, magenta, white);

	auto teapot = make_render_object(make_mesh("teapot.obj"));
	{
		teapot->set_translate(glm::vec3(0.0f, -10.0f, -40.0f));
		teapot->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
	}
	{
		teapot->set_material(defaultMaterial);
	}

	auto cube1 = make_render_object(cube);
	{
		cube1->set_translate(glm::vec3(0, -15, -40));
		cube1->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
		cube1->set_scale(glm::vec3(10, 10, 4));
	}
	{
		cube1->set_material(cubeMaterial);
	}

	auto cube2 = make_render_object(cube);
	{
		cube2->set_translate(glm::vec3(0, -10, -40));
		cube1->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
		cube2->set_scale(glm::vec3(12, 12, 1));
	}
	{
		cube2->set_material(cubeMaterial);
	}

	auto plane = make_render_object(make_mesh("plane.obj"));
	{
		plane->set_scale(glm::vec3(10, 10, 1));
	}
	{
		plane->set_material(planeMaterial);
	}

	auto cube3 = make_render_object(cube);
	{
		cube3->set_translate(glm::vec3(0.0f, -20.0f, -40.0f));
		cube3->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
		cube3->set_scale(glm::vec3(50, 50, 0.5));
	}
	{
		cube3->set_material(cubeMaterial);
	}

	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			teapot->render(camera);
			cube1->render(camera);
			cube2->render(camera);
			cube3->render(camera);
			plane->render(camera);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}