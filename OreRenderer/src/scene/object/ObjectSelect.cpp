#include "ObjectSelect.h"
#include "mesh/ObjLoader.h"

#include <algorithm>
#include <filesystem>

ObjectSelect::ObjectSelect(const std::string& directory)
{
    loadObjects(directory);
}

size_t ObjectSelect::count() const
{
    return m_Objects.size();
}

const std::string& ObjectSelect::name(size_t index) const
{
    return m_Objects[index].name;
}

size_t ObjectSelect::findIndex(const std::string& name, size_t fallback) const
{
    for (size_t i = 0; i < m_Objects.size(); ++i)
    {
        if (m_Objects[i].name == name)
            return i;
    }
    return fallback;
}

Mesh& ObjectSelect::findMesh(size_t index)
{
    auto& info = m_Objects[index];

    // Load only the first time it is requested
    if (!info.mesh)
    {
        info.mesh = std::make_unique<Mesh>(
            ObjLoader::Load(info.path)
        );
    }

    return *info.mesh;
}

void ObjectSelect::loadObjects(const std::string& directory)
{
    namespace fs = std::filesystem;

    m_Objects.clear();

    for (const auto& entry : fs::directory_iterator(directory))
    {
        if (!entry.is_regular_file())
            continue;

        if (entry.path().extension() != ".obj")
            continue;

        MeshInfo info;

        info.name = entry.path().stem().string();
        info.path = entry.path().string();

        // Mesh is NOT loaded here
        m_Objects.push_back(std::move(info));
    }

    // keep the list in alphabetical order
    std::sort(m_Objects.begin(), m_Objects.end(), [](const MeshInfo& a, const MeshInfo& b)
        {
            return a.name < b.name;
        });
}
