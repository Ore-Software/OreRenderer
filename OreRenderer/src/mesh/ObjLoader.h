#pragma once

#include <string>

#include <OreRenderer/Object.h>

class ObjLoader
{
public:
    static Object Load(const std::string& filename);
};
