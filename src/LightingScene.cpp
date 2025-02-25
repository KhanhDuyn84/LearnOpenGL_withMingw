#include "LightingScene.h"
#include "Camera.h"


LightingScene::LightingScene()
{   
    m_lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);

    initBuffer();
    m_objectShader = new GLSLShader(CUBE_VS, CUBE_FS);
    // m_objectShader->addUniform("material.ambient");
    m_objectShader->addUniform("material.diffuse");
    m_objectShader->addUniform("material.specular");
    m_objectShader->addUniform("material.shininess");

    m_objectShader->addUniform("light.position");
    m_objectShader->addUniform("light.direction");
    m_objectShader->addUniform("light.cutOff");
    m_objectShader->addUniform("light.outerCutOff");
    m_objectShader->addUniform("light.ambient");
    m_objectShader->addUniform("light.diffuse");
    m_objectShader->addUniform("light.specular");
    m_objectShader->addUniform("light.constant");
    m_objectShader->addUniform("light.linear");
    m_objectShader->addUniform("light.quadratic");


    m_objectShader->addUniform("viewPos");
    m_objectShader->addUniform("MVP");
    m_objectShader->addUniform("model");
    //m_objectShader->addUniform("view");

    m_objectShader->use();
    //for material
    //m_objectShader->setVec3("material.ambient", glm::vec3(0.24725,	0.1995,	0.0745));
    m_objectShader->setInt("material.diffuse", 0);
    m_objectShader->setInt("material.specular", 1);
    m_objectShader->setFloat("material.shininess", 32);

    //for light
    m_objectShader->setVec3("light.position", m_lightPosition);
    //m_objectShader->setVec3("light.direction", Ca);
    m_objectShader->setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    m_objectShader->setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
    m_objectShader->setVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    m_objectShader->setVec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f ));
    m_objectShader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    m_objectShader->setFloat("light.constant", 1.0f);
    m_objectShader->setFloat("light.linear", 0.09f);
    m_objectShader->setFloat("light.quadratic", 0.032f);

    m_lightShader = new GLSLShader(LIGHT_VS, LIGHT_FS);
    m_lightShader->addUniform("MVP");
    m_lightShader->use();

    // init texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    unsigned char *data = SOIL_load_image(CONTAINER_2_PNG, &width, &height, &channels, SOIL_LOAD_AUTO);
    if (data)
    {
        if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    SOIL_free_image_data(data);

    // init texture
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = SOIL_load_image(CONTAINER_2_SPECULAR_PNG, &width, &height, &channels, SOIL_LOAD_AUTO);
    if (data)
    {
        if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    SOIL_free_image_data(data);

}

void LightingScene::initBuffer()
{
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };  

    //init buffer for object
    glGenVertexArrays(1, &m_objectVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(m_objectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    //init buffer for light
    glGenVertexArrays(1, &m_lightVAO);
    glBindVertexArray(m_lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //release binding buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

LightingScene::~LightingScene()
{
    glDeleteVertexArrays(1, &m_objectVAO);
    glDeleteVertexArrays(1, &m_lightVAO);
    glDeleteBuffers(1, &VBO);
    if (m_objectShader) 
    {
        delete m_objectShader;
        m_objectShader = nullptr;
    }
    if (m_lightShader) 
    {
        delete m_lightShader;
        m_lightShader = nullptr;
    }
}

void LightingScene::renderScene(Camera* camera, float screenWidth, float screenHeight)
{
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 MVP = glm::mat4(1.0f);

    //calculate object matrix
    view = glm::lookAt(camera->getPosition(), camera->getPosition() + camera->getTarget(), camera->getVectorUp());
    projection = glm::perspective(glm::radians(camera->getFov()), screenWidth / screenHeight, 0.1f, 1000.0f);

    m_lightPosition.x = 1.0f + sin(glfwGetTime()) * 2.0f;
    m_lightPosition.y = sin(glfwGetTime() / 2.0f) * 1.0f;
    // render object
    m_objectShader->use();
    m_objectShader->setVec3("viewPos", camera->getPosition());
    m_objectShader->setVec3("light.position", camera->getPosition());
    m_objectShader->setVec3("light.direction", camera->getTarget());

    glBindVertexArray(m_objectVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    for(int i = 0; i < 10; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        MVP = projection * view * model;
        m_objectShader->setMat4("MVP", MVP);
        m_objectShader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //calculate light matrix
    model = glm::mat4(1.0f);
    model = glm::translate(model, m_lightPosition);
    model = glm::scale(model, glm::vec3(0.2f));
    MVP = projection * view * model;

    // render light
    m_lightShader->use();
    m_lightShader->setMat4("MVP", MVP);
    glBindVertexArray(m_lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}