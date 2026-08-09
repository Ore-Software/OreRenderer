#include <OreRenderer/Renderer.h>

#include "internal/VertexArray.h"
#include "internal/VertexBuffer.h"
#include "internal/IndexBuffer.h"
#include "internal/VertexBufferLayout.h"

#include <glad/gl.h>

class Renderer::Impl
{
public:
    VertexBufferLayout layout;
    VertexArray vertexArray;
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    bool hasMesh = false;

    Camera camera;
    Light light;
    Material material;
    ShaderLibrary shaderLibrary;
    GoochParams gooch;
    PBRParams pbr;

    glm::mat4 modelMatrix{1.0f};
    glm::mat4 projMatrix{1.0f};

    Impl(const std::string& shaderDirectory)
        : layout(),
          vertexArray(),
          vertexBuffer(nullptr, 0, DRAW_MODE::STATIC),
          indexBuffer(nullptr, 0, DRAW_MODE::STATIC),
          camera(0.3333f, 1.5f, 3.0f),
          shaderLibrary(shaderDirectory)
    {
        layout.Push<float>(3);
        layout.Push<float>(3);
        vertexArray.AddBuffer(vertexBuffer, layout);
    }
};

Renderer::Renderer(const std::string& shaderDirectory)
    : m_Impl(new Impl(shaderDirectory))
{
}

Renderer::~Renderer()
{
    delete m_Impl;
}

Camera& Renderer::GetCamera()
{
    return m_Impl->camera;
}

Light& Renderer::GetLight()
{
    return m_Impl->light;
}

Material& Renderer::GetMaterial()
{
    return m_Impl->material;
}

ShaderLibrary& Renderer::GetShaderLibrary()
{
    return m_Impl->shaderLibrary;
}

GoochParams& Renderer::GetGoochParams()
{
    return m_Impl->gooch;
}

PBRParams& Renderer::GetPBRParams()
{
    return m_Impl->pbr;
}

void Renderer::SetMesh(const Mesh& mesh)
{
    m_Impl->vertexBuffer.AssignData(
        mesh.m_OutVertices,
        mesh.m_OutNumVert * sizeof(float),
        DRAW_MODE::STATIC);

    m_Impl->indexBuffer.AssignData(
        mesh.m_OutIndices,
        mesh.m_OutNumIdx,
        DRAW_MODE::STATIC);

    m_Impl->hasMesh = true;
}

void Renderer::SetRenderMode(RenderMode mode)
{
    switch (mode)
    {
    case RenderMode::Polygon:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;

    case RenderMode::Wireframe:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;

    case RenderMode::PointCloud:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    }
}

void Renderer::SetModelMatrix(const glm::mat4& model)
{
    m_Impl->modelMatrix = model;
}

void Renderer::SetProjectionMatrix(const glm::mat4& proj)
{
    m_Impl->projMatrix = proj;
}

void Renderer::Draw()
{
    if (!m_Impl->hasMesh)
        return;

    m_Impl->shaderLibrary.UploadUniforms(
        m_Impl->modelMatrix,
        m_Impl->camera,
        m_Impl->projMatrix,
        m_Impl->light,
        m_Impl->material,
        m_Impl->gooch,
        m_Impl->pbr);

    m_Impl->vertexArray.Bind();
    m_Impl->indexBuffer.Bind();

    glDrawElements(
        GL_TRIANGLES,
        m_Impl->indexBuffer.GetCount(),
        GL_UNSIGNED_INT,
        nullptr);
}
