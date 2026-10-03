#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera;

class InputProcesses
{
public:
    InputProcesses() = default;
    ~InputProcesses() = default;

    void processInput(GLFWwindow* window, Camera& camera, float deltaTime);
    void SetCallbacks(GLFWwindow* window, Camera& camera);
    void SetEnabled(bool enabled) { m_Enabled = enabled; }
    bool IsEnabled() const { return m_Enabled; }
    void ResetMouse() { m_FirstMouse = true; }

private:
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    static InputProcesses* s_Instance;
    Camera* m_Camera = nullptr;
    bool m_Enabled = true;
    float m_LastX = 0.0f;
    float m_LastY = 0.0f;
    bool m_FirstMouse = true;
};