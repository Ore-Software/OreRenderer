#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "Object.h"

struct ObjectInfo
{
    std::string name;
    std::string path;
    std::unique_ptr<Object> object;   // Loaded only when needed
};

class ObjectSelect
{
public:
    ObjectSelect(const std::string& directory = "res/objects");
    ~ObjectSelect() = default;

    size_t count() const; // number of loaded objects
    const std::string& name(size_t index) const; // name for the UI

    Object& findObj(size_t index);

private:
    void loadObjects(const std::string& directory);

    std::vector<ObjectInfo> m_Objects;
};
