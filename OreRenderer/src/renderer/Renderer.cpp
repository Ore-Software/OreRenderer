#include <OreRenderer/Renderer.h>

#include "internal/VertexArray.h"
#include "internal/VertexBuffer.h"
#include "internal/IndexBuffer.h"
#include "internal/VertexBufferLayout.h"


class Renderer::Impl
{
public:
    VertexBufferLayout layout;
    VertexArray vertexArray;
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;

    Impl(const Mesh& mesh)
        : layout(),
          vertexArray(),
          vertexBuffer(
              mesh.m_OutVertices,
              mesh.m_OutNumVert * sizeof(float),
              DRAW_MODE::STATIC),
          indexBuffer(
              mesh.m_OutIndices,
              mesh.m_OutNumIdx,
              DRAW_MODE::STATIC)
    {
        layout.Push<float>(3);
        layout.Push<float>(3);

        vertexArray.AddBuffer(vertexBuffer, layout);
    }
};

Renderer::Renderer(const Mesh& mesh)
    : m_Impl(new Impl(mesh))
{
}

Renderer::~Renderer()
{
    delete m_Impl;
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

void Renderer::Draw()
{
    m_Impl->vertexArray.Bind();
    m_Impl->indexBuffer.Bind();

    glDrawElements(
        GL_TRIANGLES,
        m_Impl->indexBuffer.GetCount(),
        GL_UNSIGNED_INT,
        nullptr);
}
