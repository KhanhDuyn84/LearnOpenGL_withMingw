#include "Application.h"
#include "WindowConfig.h"
#include "FPSCalculator.h"
#include "Terrain.h"
#include "Camera.h"
#include "LightingScene.h"

Application::Application()
{
    m_FpsCalculator = std::make_unique<FPSCalculator>();
    m_WindowConfig = std::make_unique<WindowConfig>(WINDOW_PROPERTIES);
    screenWidth = m_WindowConfig->getWidth();
    screenHeight = m_WindowConfig->getHeight();

    initGLFWWindow();

	createGLFWWindow();

	initGlad();

    glViewport(0, 0, screenWidth, screenHeight);

    glfwSetWindowUserPointer(m_MainWindow, static_cast<void*>(this));

	setupCallbackFunc();

    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    m_Camera = new Camera(cameraPos, cameraFront, cameraUp);

    m_Terrain = new Terrain(HEIGHT_MAP_0113_GIF);

    m_lightScene = new LightingScene();

}

Application::~Application()
{
    if (m_Camera) {
        delete m_Camera;
        m_Camera = nullptr;
    }

    if (m_Terrain) {
        delete m_Terrain;
        m_Terrain = nullptr;
    }

    if (m_lightScene) {
        
    }
    glfwDestroyWindow(m_MainWindow);
	glfwTerminate();
}

void Application::initGLFWWindow()
{
    if (!glfwInit())
	{
		std::cout << "FAILED TO INITIALIZE GLFW WINDOW" << std::endl;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Application::createGLFWWindow()
{   
    const char* title = m_WindowConfig->getTitle().c_str();
    m_MainWindow = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr);

	if (m_MainWindow == nullptr)
	{
		std::cout << "FAILED TO CREATE GLFW WINDOW" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(m_MainWindow);
}

void Application::initGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "FAILED TO INITIALIZE GLAD" << std::endl;
		glfwTerminate();
	}
}

void Application::setupCallbackFunc() 
{
	glfwSetFramebufferSizeCallback(m_MainWindow, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    glfwSetCursorPosCallback(m_MainWindow,[](GLFWwindow *window, double xpos, double ypos)
    {
        auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));
        auto camera = application->getCamera();
        static bool firstMouse = false;
        static float lastX = application->getScreenWidth() / 2;
        static float lastY = application->getScreenHeight() / 2;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            if (firstMouse)
            {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
            lastX = xpos;
            lastY = ypos;

            camera->inputMouse(xoffset, yoffset);
        }
        else
        {
            firstMouse = true;
        }
    });

    glfwSetScrollCallback(m_MainWindow, [](GLFWwindow *window, double xoffset, double yoffset)
    {
        auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));
        auto camera = application->getCamera();
        camera->inputScroll(yoffset);
    });

}

void Application::processInput(float deltaTime)
{
    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(m_MainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
		glfwSetWindowShouldClose(m_MainWindow, GL_TRUE);
    }
    if (glfwGetKey(m_MainWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_Camera->inputKeyBoard(Camera_Movement::FORWARD, deltaTime);
	}
	if (glfwGetKey(m_MainWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_Camera->inputKeyBoard(Camera_Movement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(m_MainWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_Camera->inputKeyBoard(Camera_Movement::LEFT, deltaTime);
	}
	if (glfwGetKey(m_MainWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_Camera->inputKeyBoard(Camera_Movement::RIGHT, deltaTime);
	}
}

void Application::run() {

    // float vertices[] = {
    //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    //     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    // };

    // glm::vec3 cubePositions[] = {
    //     glm::vec3( 0.0f,  0.0f,  0.0f),
    //     glm::vec3( 2.0f,  5.0f, -15.0f),
    //     glm::vec3(-1.5f, -2.2f, -2.5f),
    //     glm::vec3(-3.8f, -2.0f, -12.3f),
    //     glm::vec3( 2.4f, -0.4f, -3.5f),
    //     glm::vec3(-1.7f,  3.0f, -7.5f),
    //     glm::vec3( 1.3f, -2.0f, -2.5f),
    //     glm::vec3( 1.5f,  2.0f, -2.5f),
    //     glm::vec3( 1.5f,  0.2f, -1.5f),
    //     glm::vec3(-1.3f,  1.0f, -1.5f)
    // };

	// unsigned int VAO, VBO;
	// glGenVertexArrays(1, &VAO);
	// glGenBuffers(1, &VBO);

	// glBindVertexArray(VAO);

	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);

	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	// unsigned int texture1, texture2;
    // // texture 1
    // // ---------
    // glGenTextures(1, &texture1);
    // glBindTexture(GL_TEXTURE_2D, texture1);
    //  // set the texture wrapping parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // set texture filtering parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load image, create texture and generate mipmaps
    // int width, height, nrChannels;
    // // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.

    // unsigned char *data = SOIL_load_image(CONTAINER_JPG, &width, &height, &nrChannels, SOIL_LOAD_AUTO);
    // if (data)
    // {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // SOIL_free_image_data(data);
    // // texture 2
    // // ---------
    // glGenTextures(1, &texture2);
    // glBindTexture(GL_TEXTURE_2D, texture2);
    // // set the texture wrapping parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // set texture filtering parameters
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load image, create texture and generate mipmaps
    // data = SOIL_load_image(AWESOMEFACE_PNG, &width, &height, &nrChannels, 0);
    // if (data)
    // {
    //     // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // SOIL_free_image_data(data);

	// GLSLShader shader(TRIANGLE_VS, TRIANGLE_FS);
    // shader.addUniform("model");
    // shader.addUniform("view");
    // shader.addUniform("projection");
	// shader.addUniform("ourTexture1");
	// shader.addUniform("ourTexture2");

	// shader.use();
	// shader.setInt("ourTexture1", 0);
	// shader.setInt("ourTexture2", 1);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(m_MainWindow))
	{
		m_FpsCalculator->doFrame();
        processInput(m_FpsCalculator->getDeltaTime());

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //m_Terrain->render(m_Camera, screenWidth, screenHeight);

        m_lightScene->renderScene(m_Camera, screenWidth, screenHeight);

		// glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);

        // glBindVertexArray(VAO);

        // glm::mat4 view = glm::mat4(1.0f);
        // view = glm::lookAt(m_Camera->getPosition(), m_Camera->getPosition() + m_Camera->getTarget(), m_Camera->getVectorUp());

        // glm::mat4 projection = glm::mat4(1.0f);
        // projection = glm::perspective(glm::radians(m_Camera->getFov()), screenWidth/ screenHeight, 0.1f, 100.0f);

        // shader.use();
        // shader.setMat4("view", view);
        // shader.setMat4("projection", projection);

        // for(int i = 0; i < 10; i++)
        // {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, cubePositions[i]);
        //     float angle = 20.0f * i;
        //     if(i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
        //         angle = glfwGetTime() * 25.0f;
        //     model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //     shader.use();
        //     shader.setMat4("model", model);
        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }

        glfwSwapBuffers(m_MainWindow);
		glfwPollEvents();
    }
}