#pragma once

#include <OreRenderer/Mesh.h>
#include <OreRenderer/Camera.h>
#include <OreRenderer/Light.h>
#include <OreRenderer/Material.h>
#include <OreRenderer/ShaderLibrary.h>

#include <glm/glm.hpp>
#include <string>

enum class RenderMode
{
    Polygon,
    Wireframe,
    PointCloud
};

class Renderer
{
public:
    Renderer(const std::string& shaderDirectory);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Camera& GetCamera();
    Light& GetLight();
    Material& GetMaterial();
    ShaderLibrary& GetShaderLibrary();
    GoochParams& GetGoochParams();
    PBRParams& GetPBRParams();

    void SetMesh(const Mesh& mesh);
    void SetRenderMode(RenderMode mode);

    void SetModelMatrix(const glm::mat4& model);
    void SetProjectionMatrix(const glm::mat4& proj);

    void Draw();

private:
    class Impl;
    Impl* m_Impl;
};