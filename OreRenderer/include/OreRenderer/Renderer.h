#pragma once

#include <OreRenderer/Mesh.h>

enum class RenderMode
{
    Polygon,
    Wireframe,
    PointCloud
};

class Renderer
{
public:
    Renderer(const Mesh& mesh);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void SetMesh(const Mesh& mesh);
    void SetRenderMode(RenderMode mode);

    void Draw();

private:
    class Impl;
    Impl* m_Impl;
};