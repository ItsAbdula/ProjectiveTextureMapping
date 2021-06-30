#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OpenGLWrapper.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_btn_callBack(GLFWwindow* window, int btn, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

void Scene1(GLFWwindow* window);
void Scene2(GLFWwindow* window);
void Scene3(GLFWwindow* window);
void Scene4(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));
Camera projector(glm::vec3(0.0f, 0.0f, 50.0f));
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
double decalX = SCR_WIDTH / 2.0f;
double decalY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
double deltaTime = 0.0f;
double lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 20.0f, 2.0f);

int main()
{
    int scene_num;
    std::cout << "1: teapot, 2: human, 3: humvee, 4: house" << std::endl;
    std::cout << "select scene (1-4): ";
    std::cin >> scene_num;

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

    if (scene_num == 1)
        Scene1(window);
    if (scene_num == 2)
        Scene2(window);
    if (scene_num == 3)
        Scene3(window);
    if (scene_num == 4)
        Scene4(window);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
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

void mouse_btn_callBack(GLFWwindow* window, int btn, int action, int mods)
{
    if (btn == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &lastX, &lastY);
    }

    if (btn == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &decalX, &decalX);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // left click and drag: move camera
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        double xoffset = ((xpos - lastX) / (height) * 180);
        double yoffset = ((lastY - ypos) / (width) * 180);
        lastX = xpos;
        lastY = ypos;
        camera.ProcessMouseMovement(xoffset, yoffset);
    }

    // right click and drag: move projector image
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        double xoffset = ((xpos - decalX) / (height) * 180);
        double yoffset = ((decalY - ypos) / (width) * 180);
        decalX = xpos;
        decalY = ypos;
        projector.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void Scene1(GLFWwindow* window)
{
    auto cam_programID = build_program("Camera");
    auto lighting = build_program("Lighting_Specular");
    auto lamp = build_program("Lighting_Lamp");
    auto lightmap = build_program("Lighting_Maps");
    auto texture_shader = build_program("Texture");
    auto projector_shader = build_program("Projector");

    auto cube = make_mesh("cube.obj");

    auto black = load_image("black.png", ImageType::REPEAT);
    auto magenta = load_image("magenta.png", ImageType::REPEAT);
    auto orange = load_image("orange.png", ImageType::REPEAT);
    auto white = load_image("white.png", ImageType::REPEAT);
    auto transparent = load_image("transparent.png", ImageType::REPEAT);

    auto wall = load_image("smileface.png");
    auto wall_tex = load_image("wall.jpg", ImageType::REPEAT);
    auto container_diffuse = load_image("container2.png", ImageType::REPEAT);
    auto container_specular = load_image("container2_specular.png", ImageType::REPEAT);

    auto defaultMaterial = new Material(lightmap, orange, transparent);
    auto cubeMaterial = new Material(lightmap, container_diffuse, container_specular);
    auto planeMaterial = new Material(lightmap, magenta, transparent);

    auto projectorMaterial = new Material(projector_shader, wall_tex, wall);
    auto projectorplaneMaterial = new Material(projector_shader, magenta, wall);

    auto teapot = make_render_object(make_mesh("teapot.obj"));
    {
        auto transform = teapot->get_transform();
        transform->set_translate(glm::vec3(0.0f, -10.0f, -40.0f));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
    }
    {
        teapot->set_material(projectorMaterial);
    }

    auto cube1 = make_render_object(cube);
    {
        auto transform = cube1->get_transform();
        transform->set_translate(glm::vec3(0, -15, -40));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
        transform->set_scale(glm::vec3(10, 10, 4));
    }
    {
        cube1->set_material(cubeMaterial);
    }

    auto cube2 = make_render_object(cube);
    {
        auto transform = cube2->get_transform();
        transform->set_translate(glm::vec3(0, -10, -40));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
        transform->set_scale(glm::vec3(12, 12, 1));
    }
    {
        cube2->set_material(cubeMaterial);
    }

    auto plane = make_render_object(make_mesh("plane.obj"));
    {
        auto transform = plane->get_transform();
        transform->set_translate(glm::vec3(0.0f, 0.0f, -60.0f));
        transform->set_scale(glm::vec3(40, 40, 1));
    }
    {
        plane->set_material(projectorplaneMaterial);
    }

    auto cube3 = make_render_object(cube);
    {
        auto transform = cube3->get_transform();
        transform->set_translate(glm::vec3(0.0f, -20.0f, -40.0f));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
        transform->set_scale(glm::vec3(50, 50, 0.5));
    }
    {
        cube3->set_material(cubeMaterial);
    }

    auto cube4 = make_render_object(cube);
    {
        auto transform = cube4->get_transform();
        transform->set_translate(glm::vec3(0.0f, 10.0f, 0.0f));
    }
    {
        cube4->set_material(cubeMaterial);
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
            teapot->projective_render(camera, projector);
            cube1->render(camera);
            cube2->render(camera);
            cube3->render(camera);
            plane->projective_render(camera, projector);
            cube4->render(camera);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Scene2(GLFWwindow* window)
{
    auto cam_programID = build_program("Camera");
    auto lightmap = build_program("Lighting_Maps");
    auto projector_shader = build_program("Projector");

    auto cube = make_mesh("cube.obj");

    auto transparent = load_image("transparent.png", ImageType::REPEAT);

    auto human_diffuse = load_image("Body_Colour.jpg", ImageType::REPEAT);
    auto human_tattoo = load_image("tattoo.png");

    //auto wall_diffuse = load_texture("store.png");
    //auto wall_specular = load_texture("storeNM.png");

    auto floor_diffuse = load_image("concrete_light.jpg", ImageType::REPEAT);

    auto cube_diffuse = load_image("container2.png", ImageType::REPEAT);
    auto cube_specular = load_image("container2_specular.png", ImageType::REPEAT);

    auto wall = load_image("tattoo.png");
    auto wall_tex = load_image("store.png", ImageType::REPEAT);

    auto humanMaterial = new Material(projector_shader, human_diffuse, human_tattoo);
    auto wallMaterial = new Material(projector_shader, wall_tex, wall);
    auto floorMaterial = new Material(lightmap, floor_diffuse, transparent);
    auto cubeMaterial = new Material(lightmap, cube_diffuse, cube_specular);

    auto human = make_render_object(make_mesh("human.obj"));
    {
        auto transform = human->get_transform();
        transform->set_scale(glm::vec3(12, 12, 12));
        transform->set_translate(glm::vec3(0.0f, -15.2f, -30.0f));
    }
    {
        human->set_material(humanMaterial);
    }

    auto floor = make_render_object(cube);
    {
        auto transform = floor->get_transform();
        transform->set_translate(glm::vec3(0.0f, -20.0f, -40.0f));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
        transform->set_scale(glm::vec3(50, 30, 0.5));
    }
    {
        floor->set_material(floorMaterial);
    }

    auto wall_1 = make_render_object(cube);
    {
        auto transform = wall_1->get_transform();
        transform->set_translate(glm::vec3(25.0f, 0.0f, -70.0f));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
        transform->set_scale(glm::vec3(25, 0.5, 20));
    }
    {
        wall_1->set_material(wallMaterial);
    }

    auto wall_2 = make_render_object(cube);
    {
        auto transform = wall_2->get_transform();
        transform->set_translate(glm::vec3(-25.0f, 0.0f, -70.0f));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
        transform->set_scale(glm::vec3(25, 0.5, 20));
    }
    {
        wall_2->set_material(wallMaterial);
    }

    auto table_bot = make_render_object(cube);
    {
        auto transform = table_bot->get_transform();
        transform->set_translate(glm::vec3(0, -17.5, -30));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, -0.0f));
        transform->set_scale(glm::vec3(5, 5, 2));
    }
    {
        table_bot->set_material(cubeMaterial);
    }

    auto table_top = make_render_object(cube);
    {
        auto transform = table_top->get_transform();
        transform->set_translate(glm::vec3(0, -15.5, -30));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
        transform->set_scale(glm::vec3(6, 6, 0.5));
    }
    {
        table_top->set_material(cubeMaterial);
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
            human->projective_render(camera, projector);
            floor->render(camera);
            wall_1->projective_render(camera, projector);
            wall_2->projective_render(camera, projector);
            table_bot->render(camera);
            table_top->render(camera);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Scene3(GLFWwindow* window)
{
    auto cam_programID = build_program("Camera");
    auto lightmap = build_program("Lighting_Maps");
    auto projector_shader = build_program("Projector");

    auto cube = make_mesh("cube.obj");

    auto transparent = load_image("transparent.png", ImageType::REPEAT);

    auto humvee_diffuse = load_image("Tex_0023_1.png", ImageType::REPEAT);
    auto humvee_specular = load_image("Tex_0025_1.png", ImageType::REPEAT);

    auto floor_diffuse = load_image("concrete_light.jpg", ImageType::REPEAT);

    auto wall_diffuse = load_image("foundation_brown_brick.jpg", ImageType::REPEAT);
    auto wall_specular = load_image("foundation_brown_brick_spec.jpg", ImageType::REPEAT);

    auto projImage = load_image("bulletholes.png");
    auto wall_tex = load_image("foundation_brown_brick.jpg", ImageType::REPEAT);

    auto humveeMaterial = new Material(projector_shader, humvee_diffuse, projImage);
    auto floorMaterial = new Material(lightmap, floor_diffuse, transparent);
    auto wallMaterial = new Material(projector_shader, wall_tex, projImage);

    auto humvee = make_render_object(make_mesh("humvee.obj"));
    {
        auto transform = humvee->get_transform();
        transform->set_scale(glm::vec3(0.1, 0.1, 0.1));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 90.0f));
        transform->set_translate(glm::vec3(0.0f, -19.0f, -35.0f));

        humvee->set_material(humveeMaterial);
    }

    auto floor = make_render_object(cube);
    {
        auto transform = floor->get_transform();
        transform->set_translate(glm::vec3(0.0f, -20.0f, -40.0f));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
        transform->set_scale(glm::vec3(50, 30, 0.5));
    }
    {
        floor->set_material(floorMaterial);
    }

    auto wall_1 = make_render_object(cube);
    {
        auto transform = wall_1->get_transform();
        transform->set_translate(glm::vec3(25.0f, 0.0f, -70.0f));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
        transform->set_scale(glm::vec3(25, 0.5, 20));
    }
    {
        wall_1->set_material(wallMaterial);
    }

    auto wall_2 = make_render_object(cube);
    {
        auto transform = wall_2->get_transform();
        transform->set_translate(glm::vec3(-25.0f, 0.0f, -70.0f));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
        transform->set_scale(glm::vec3(25, 0.5, 20));
    }
    {
        wall_2->set_material(wallMaterial);
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
            humvee->projective_render(camera, projector);
            floor->render(camera);
            wall_1->projective_render(camera, projector);
            wall_2->projective_render(camera, projector);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
void Scene4(GLFWwindow* window)
{
    auto cam_programID = build_program("Camera");
    auto lightmap = build_program("Lighting_Maps");
    auto projector_shader = build_program("Projector");

    auto cube = make_mesh("cube.obj");

    auto transparent = load_image("transparent.png", ImageType::REPEAT);

    auto building_diffuse = load_image("house.png", ImageType::REPEAT);
    auto building_specular = load_image("house_specular.png", ImageType::REPEAT);

    auto projImage = load_image("graffiti.png");

    auto floor_diffuse = load_image("grass_ground.jpg", ImageType::REPEAT);

    auto buildingMaterial = new Material(projector_shader, building_diffuse, projImage);
    auto floorMaterial = new Material(projector_shader, floor_diffuse, projImage);

    auto building = make_render_object(make_mesh("House.obj"));
    {
        auto transform = building->get_transform();
        transform->set_scale(glm::vec3(0.05, 0.05, 0.05));
        transform->set_translate(glm::vec3(0.0f, -19.5f, -35.0f));
    }
    {
        building->set_material(buildingMaterial);
    }

    auto floor = make_render_object(cube);
    {
        auto transform = floor->get_transform();
        transform->set_translate(glm::vec3(0.0f, -20.0f, -40.0f));
        transform->set_rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
        transform->set_scale(glm::vec3(50, 30, 0.5));
    }
    {
        floor->set_material(floorMaterial);
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
            building->projective_render(camera, projector);
            floor->projective_render(camera, projector);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}