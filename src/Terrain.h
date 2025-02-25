#pragma once
#include "pch.h"

class Camera;
class GLSLShader;

class Terrain 
{
private:
	int m_terrainWidth, m_terrainHeight;
	unsigned int m_vertexCount, m_indexCount;
    unsigned int m_VAO, m_VBO;
    GLSLShader *m_shader;
    std::vector<glm::vec3> m_heightMap;

    void initBuffer(const char* imagePath);

public:
    Terrain(const char* imagePath);
    ~Terrain();
    void render(Camera *camera, float screenWidth, float screenHeight);
};