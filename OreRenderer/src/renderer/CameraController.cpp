#include "CameraController.h"
#include "Input.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

CameraController::CameraController(Camera& camera, unsigned int screenWidth, unsigned int screenHeight)
    : m_Camera(camera), m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight)
{
}

void CameraController::Update(GLFWwindow* window, float deltaTime)
{
    m_Rotated = false;
    m_ProjectionChanged = false;

    // close the window
    if (Input::IsKeyDown(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);

    // Move forward
    if (Input::IsKeyDown(GLFW_KEY_W))
        m_Camera.MoveCamera(m_Camera.GetCameraFront(), deltaTime * 5.0f);
    // Move backward
    if (Input::IsKeyDown(GLFW_KEY_S))
        m_Camera.MoveCamera(-m_Camera.GetCameraFront(), deltaTime * 5.0f);
    // Strafe left
    if (Input::IsKeyDown(GLFW_KEY_A))
        m_Camera.MoveCamera(m_Camera.GetCameraRight(), deltaTime * 5.0f);
    // Strafe right
    if (Input::IsKeyDown(GLFW_KEY_D))
        m_Camera.MoveCamera(-m_Camera.GetCameraRight(), deltaTime * 5.0f);
    // fly up
    if (Input::IsKeyDown(GLFW_KEY_SPACE))
        m_Camera.MoveCamera(m_Camera.GetCameraUp(), deltaTime * 5.0f);
    // drop down
    if (Input::IsKeyDown(GLFW_KEY_LEFT_CONTROL))
        m_Camera.MoveCamera(-m_Camera.GetCameraUp(), deltaTime * 5.0f);

    // move closer
    if (Input::IsKeyDown(GLFW_KEY_K))
        m_Camera.RotateCamera(0, 0, -deltaTime);
    // move further
    if (Input::IsKeyDown(GLFW_KEY_J))
        m_Camera.RotateCamera(0, 0, deltaTime);
    // rotate left
    if (Input::IsKeyDown(GLFW_KEY_LEFT))
        m_Camera.RotateCamera(0, deltaTime, 0);
    // rotate right
    if (Input::IsKeyDown(GLFW_KEY_RIGHT))
        m_Camera.RotateCamera(0, -deltaTime, 0);
    // rotate up
    if (Input::IsKeyDown(GLFW_KEY_UP))
        m_Camera.RotateCamera(deltaTime, 0, 0);
    // rotate down
    if (Input::IsKeyDown(GLFW_KEY_DOWN))
        m_Camera.RotateCamera(-deltaTime, 0, 0);

    // mouse movement
    double currXpos, currYpos;
    glfwGetCursorPos(window, &currXpos, &currYpos);
    double deltaX = (currXpos - m_LastXpos) / m_ScreenWidth;
    double deltaY = (currYpos - m_LastYpos) / m_ScreenHeight;
    m_LastXpos = currXpos;
    m_LastYpos = currYpos;

    // rotate model according to mouse movement
    if (Input::IsMouseButtonDown(GLFW_MOUSE_BUTTON_1) && !ImGui::GetIO().WantCaptureMouse)
    {
        m_RotationAngle = (float)deltaX * m_Sens;
        m_RotationAngle > 360.0f ? m_RotationAngle -= 360.0f : 0;
        m_Rotated = true;
    }

    // adjust FOV using vertical scroll
    if (Input::GetScrollY() != 0)
    {
        m_Camera.changeFOV(Input::GetScrollY());
        Input::ResetScroll();
        m_ProjectionChanged = true;
    }
}

bool CameraController::Rotated() const
{
    return m_Rotated;
}

glm::mat4 CameraController::ApplyModelRotation(const glm::mat4& modelMatrix) const
{
    return glm::rotate(modelMatrix, glm::radians(m_RotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
}

bool CameraController::ProjectionChanged() const
{
    return m_ProjectionChanged;
}
