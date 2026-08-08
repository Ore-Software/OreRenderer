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

Object& ObjectSelect::findObj(size_t index)
{
    auto& info = m_Objects[index];

    // Load only the first time it is requested
    if (!info.object)
    {
        info.object = std::make_unique<Object>(
            ObjLoader::Load(info.path)
        );
    }

    return *info.object;
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

        ObjectInfo info;

        info.name = entry.path().stem().string();
        info.path = entry.path().string();

        // Object is NOT loaded here
        m_Objects.push_back(std::move(info));
    }

    // keep the list in alphabetical order
    std::sort(m_Objects.begin(),m_Objects.end(), [](const ObjectInfo& a, const ObjectInfo& b)
        {
            return a.name < b.name;
        });
}
