#pragma once

#include <string>

#include <OreRenderer/Mesh.h>

class ObjLoader
{
public:
    static Mesh Load(const std::string& filename);
};
