#pragma once

#include "pch.h"
#include <memory>

class GLFWwindow;
class WindowConfig;
class FPSCalculator;
class Camera;
class Terrain;
class LightingScene;

class Application : public NonCopyable
{
private:
    GLFWwindow *m_MainWindow;
    std::unique_ptr<FPSCalculator> m_FpsCalculator;
    std::unique_ptr<WindowConfig> m_WindowConfig;
    Camera *m_Camera;
    Terrain *m_Terrain;
    LightingScene *m_lightScene;

    float screenWidth, screenHeight;

    void initGLFWWindow();
    void createGLFWWindow();
    void initGlad();
    void processInput(float deltaTime);
    void setupCallbackFunc();

public:
	Application();	
	~Application();
    void run();
    Camera* getCamera() const { return m_Camera; }
    float getScreenWidth() const { return screenWidth; }
    float getScreenHeight() const { return screenHeight; }
};