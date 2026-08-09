#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <OreRenderer/Mesh.h>

struct MeshInfo
{
    std::string name;
    std::string path;
    std::unique_ptr<Mesh> mesh;
};

class ObjectSelect
{
public:
    ObjectSelect(const std::string& directory = "res/objects");
    ~ObjectSelect() = default;

    size_t count() const;
    const std::string& name(size_t index) const;
    size_t findIndex(const std::string& name, size_t fallback = 0) const;

    Mesh& findMesh(size_t index);

private:
    void loadObjects(const std::string& directory);

    std::vector<MeshInfo> m_Objects;
};
