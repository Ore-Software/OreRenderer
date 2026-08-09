#pragma once

#include <OreRenderer/Camera.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class CameraController
{
public:
    CameraController(Camera& camera, unsigned int screenWidth, unsigned int screenHeight);

    void Update(GLFWwindow* window, float deltaTime);

    bool Rotated() const;
    glm::mat4 ApplyModelRotation(const glm::mat4& modelMatrix) const;

    bool ProjectionChanged() const;

private:
    Camera& m_Camera;
    unsigned int m_ScreenWidth;
    unsigned int m_ScreenHeight;

    // mouse movement variables
    double m_LastXpos = 0.0;
    double m_LastYpos = 0.0;
    float m_Sens = 200.0f;

    float m_RotationAngle = 0.0f;
    bool m_Rotated = false;
    bool m_ProjectionChanged = false;
};
