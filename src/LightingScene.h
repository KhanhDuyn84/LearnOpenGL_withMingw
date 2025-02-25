#pragma once
#include "pch.h"

class Camera;
class GLSLShader;

class LightingScene 
{
private:
    unsigned int m_lightVAO, m_objectVAO;
    unsigned int VBO;
    unsigned int texture, texture1;
    GLSLShader *m_lightShader, *m_objectShader;

    glm::vec3 m_lightPosition;
    void initBuffer();

public:
    LightingScene();
    ~LightingScene();
    void renderScene(Camera* camera, float screenWidth, float screenHeight);
    
};