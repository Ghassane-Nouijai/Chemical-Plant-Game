#include "InputProcesses.h"

#include "Camera.h"

InputProcesses* InputProcesses::s_Instance = nullptr;

void InputProcesses::SetCallbacks(GLFWwindow* window, Camera& camera)
{
    m_Camera = &camera;
    s_Instance = this;
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
}

void InputProcesses::processInput(GLFWwindow* window, Camera& camera, float deltaTime)
{
    if (!m_Enabled)
        return;

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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void InputProcesses::mouse_callback(GLFWwindow*, double xposIn, double yposIn)
{
    if (!s_Instance || !s_Instance->m_Enabled || !s_Instance->m_Camera)
        return;

    if (s_Instance->m_FirstMouse)
    {
        s_Instance->m_LastX = static_cast<float>(xposIn);
        s_Instance->m_LastY = static_cast<float>(yposIn);
        s_Instance->m_FirstMouse = false;
    }

    const float xoffset = static_cast<float>(xposIn) - s_Instance->m_LastX;
    const float yoffset = s_Instance->m_LastY - static_cast<float>(yposIn);
    s_Instance->m_LastX = static_cast<float>(xposIn);
    s_Instance->m_LastY = static_cast<float>(yposIn);
    s_Instance->m_Camera->ProcessMouseMovement(xoffset, yoffset);
}

void InputProcesses::scroll_callback(GLFWwindow*, double, double yoffset)
{
    if (s_Instance && s_Instance->m_Enabled && s_Instance->m_Camera)
        s_Instance->m_Camera->ProcessMouseScroll(static_cast<float>(yoffset));
}