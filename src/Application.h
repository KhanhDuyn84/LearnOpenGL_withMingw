#pragma once

#include "NonCopyable.h"
#include <memory>

class GLFWwindow;
class WindowConfig;
class FPSCalculator;

class Application : public NonCopyable
{
private:
    GLFWwindow *m_MainWindow;
    std::unique_ptr<FPSCalculator> m_FpsCalculator;
    std::unique_ptr<WindowConfig> m_WindowConfig;

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
};