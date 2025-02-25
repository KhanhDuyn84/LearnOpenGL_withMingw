#include "Terrain.h"
#include "glad/glad.h"
#include "Camera.h"

Terrain::Terrain(const char* imagePath)
{
    initBuffer(imagePath);

    m_shader = new GLSLShader(TERRAIN_VS, TERRAIN_FS);
    m_shader->use();
    m_shader->addUniform("MVP");
//     m_shader->addUniform("view");
//     m_shader->addUniform("projection"); 
}

Terrain::~Terrain()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    if (m_shader) 
    {
        delete m_shader;
        m_shader = nullptr;
    }
}

void Terrain::initBuffer(const char* imagePath) 
{
    int channels;
    unsigned char *imageData = SOIL_load_image(imagePath, &m_terrainWidth, &m_terrainHeight, &channels, SOIL_LOAD_AUTO);
    int heightSize = m_terrainWidth * m_terrainHeight;
    m_heightMap.resize(heightSize);

    m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 12;
    m_indexCount = m_vertexCount;

    struct VertexType {
        glm::vec3 pos;
        glm::vec4 color;
    };

    std::vector<VertexType> bufferData(m_vertexCount);

    //generate buffer data
    
    int index = 0;
    glm::vec4 whileColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    for(int j = 0; j < m_terrainHeight -1; j++) {
        for(int i = 0; i < m_terrainWidth - 1; i++) {
            
            int index_bl = ((j * m_terrainWidth) + i ) * channels;
            int index_br = ((j * m_terrainWidth) + (i +1)) * channels;
            int index_ur = (((j + 1) * m_terrainWidth) + (i+1)) * channels;
            int index_ul = (((j + 1) * m_terrainWidth) + i) * channels;
            glm::vec3 bl = glm::vec3(i,  (float)imageData[index_bl] / 15.0f, j);
            glm::vec3 br = glm::vec3(i + 1, (float)imageData[index_br] / 15.0f, j);
            glm::vec3 ur = glm::vec3(i + 1, (float)imageData[index_ur] / 15.0f, j + 1);
            glm::vec3 ul = glm::vec3(i, (float)imageData[index_ul] / 15.0f, j + 1);
            
            //first line 
            VertexType vertexType = {ul, whileColor};
            bufferData[index] = vertexType;
            index++;
            vertexType = {ur, whileColor};
            bufferData[index] = vertexType;
            index++;

            //second line
            vertexType = {ur, whileColor};
            bufferData[index] = vertexType;
            index++;
            vertexType = {br, whileColor};
            bufferData[index] = vertexType;
            index++;

            //third line
            vertexType = {br, whileColor};
            bufferData[index] = vertexType;
            index++;
            vertexType = {ul, whileColor};
            bufferData[index] = vertexType;
            index++;

            //forth line
            vertexType = {ul, whileColor};
            bufferData[index] = vertexType;
            index++;
            vertexType = {br, whileColor};
            bufferData[index] = vertexType;
            index++;

            //fifth line
            vertexType = {br, whileColor};
            bufferData[index] = vertexType;
            index++;
            vertexType = {bl, whileColor};
            bufferData[index] = vertexType;
            index++;

            //sixth line
            vertexType = {bl, whileColor};
            bufferData[index] = vertexType;
            index++;
            vertexType = {ul, whileColor};
            bufferData[index] = vertexType;
            index++;

        }
    }
    SOIL_free_image_data(imageData);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexType), &bufferData.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

}

void Terrain::render(Camera *camera, float screenWidth, float screenHeight)
{

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 MVP = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
    view = glm::lookAt(camera->getPosition(), camera->getPosition() + camera->getTarget(), camera->getVectorUp());
    projection = glm::perspective(glm::radians(camera->getFov()), screenWidth / screenHeight, 0.1f, 1000.0f);
    MVP = projection * view * model;

    glBindVertexArray(m_VAO);
    m_shader->use();
    m_shader->setMat4("MVP", MVP);
    glDrawArrays(GL_LINES, 0, m_vertexCount * 12);
    glBindVertexArray(0);

}