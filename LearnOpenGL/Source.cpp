#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OpenGLWrapper.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
	auto texture_shader = build_program("Texture");

	auto teapot = make_mesh("teapot.model");
	auto cube = make_mesh("cube.model");

	GLuint textureID;
	{
		textureID = load_image("container.jpg");

		glUseProgram(texture_shader);

		set_uniform_value(texture_shader, "texture1", glm::ivec1(0));
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
			glUseProgram(texture_shader);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID);
		}

		{
			glUseProgram(lighting);

			auto objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
			set_uniform_value(lighting, "objectColor", objectColor);

			auto lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
			set_uniform_value(lighting, "lightColor", lightColor);
			set_uniform_value(lighting, "lightPos", lightPos);
			set_uniform_value(lighting, "viewPos", camera.Position);

			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			set_uniform_value(lighting, "projection", projection);

			glm::mat4 view = camera.GetViewMatrix();
			set_uniform_value(lighting, "view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -10.0f, -40.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			set_uniform_value(lighting, "model", model);

			draw_mesh(*teapot);

			glUseProgram(0);
		}

		{
			glUseProgram(lighting);

			auto objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
			set_uniform_value(lighting, "objectColor", objectColor);

			auto lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
			set_uniform_value(lighting, "lightColor", lightColor);
			set_uniform_value(lighting, "lightPos", lightPos);
			set_uniform_value(lighting, "viewPos", camera.Position);

			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			set_uniform_value(lighting, "projection", projection);

			glm::mat4 view = camera.GetViewMatrix();
			set_uniform_value(lighting, "view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -15.0f, -40.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(10, 10, 4));
			set_uniform_value(lighting, "model", model);

			draw_mesh(*cube);

			glUseProgram(0);
		}

		{
			glUseProgram(lighting);

			auto objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
			set_uniform_value(lighting, "objectColor", objectColor);

			auto lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
			set_uniform_value(lighting, "lightColor", lightColor);
			set_uniform_value(lighting, "lightPos", lightPos);
			set_uniform_value(lighting, "viewPos", camera.Position);

			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			set_uniform_value(lighting, "projection", projection);

			glm::mat4 view = camera.GetViewMatrix();
			set_uniform_value(lighting, "view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -10.0f, -40.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(12, 12, 1));
			set_uniform_value(lighting, "model", model);

			draw_mesh(*cube);

			glUseProgram(0);
		}

		{
			glUseProgram(lighting);

			auto objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
			set_uniform_value(lighting, "objectColor", objectColor);

			auto lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
			set_uniform_value(lighting, "lightColor", lightColor);
			set_uniform_value(texture_shader, "lightPos", lightPos);
			set_uniform_value(texture_shader, "viewPos", camera.Position);

			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			set_uniform_value(texture_shader, "projection", projection);

			glm::mat4 view = camera.GetViewMatrix();
			set_uniform_value(texture_shader, "view", view);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -20.0f, -40.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(50, 50, 0.5f));
			set_uniform_value(texture_shader, "model", model);

			draw_mesh(*cube);

			glUseProgram(0);
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