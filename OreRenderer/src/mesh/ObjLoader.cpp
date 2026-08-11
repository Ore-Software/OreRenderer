#include "ObjLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <charconv>

namespace
{
    void customSplit(
        std::string_view str,
        char separator,
        std::vector<std::string_view>& strings)
    {
        size_t startIndex = 0;

        for (size_t i = 0; i <= str.size(); i++)
        {
            if (i == str.size() || str[i] == separator)
            {
                strings.push_back(str.substr(startIndex, i - startIndex));

                startIndex = i + 1;
            }
        }
    }
}

Mesh ObjLoader::Load(const std::string& filename)
{
    Mesh mesh;

    mesh.m_Min = glm::vec3{
        1000000,
        1000000,
        1000000
    };

    mesh.m_Max = glm::vec3{
        -1000000,
        -1000000,
        -1000000
    };

    std::ifstream in(filename, std::ios::binary);

    if (!in)
    {
        std::cerr << "Cannot open " << filename << std::endl;
        return mesh;
    }

    std::string line;

    while (getline(in, line))
    {
        if (line.substr(0, 2) == "v ")
        {
            std::istringstream s{ std::string(line.substr(2)) };

            glm::vec3 v;
            s >> v.x;
            s >> v.y;
            s >> v.z;

            mesh.m_VertexPos.push_back(v);

            for (unsigned int coord = 0; coord < 3; coord++)
            {
                if (v[coord] > mesh.m_Max[coord])
                    mesh.m_Max[coord] = v[coord];

                if (v[coord] < mesh.m_Min[coord])
                    mesh.m_Min[coord] = v[coord];
            }
        }
        else if (line.substr(0, 2) == "f ")
        {
            std::string s(line.substr(2));

            std::vector<std::string_view> face;
            customSplit(s, ' ', face);

            std::vector<unsigned int> faceIndices;
            faceIndices.reserve(face.size());

            for (std::string_view corner : face)
            {
                if (corner.empty())
                    continue;

                std::vector<std::string_view> vertex;
                customSplit(corner, '/', vertex);

                unsigned int vertIdx = 0;
                auto result = std::from_chars(vertex[0].data(), vertex[0].data() + vertex[0].size(), vertIdx);
                if (result.ec != std::errc{})
                {
                    std::cerr << "Bad vertex index in face: '" << vertex[0] << "'" << std::endl;
                    continue;
                }
                vertIdx--;

                faceIndices.push_back(vertIdx);
            }

            mesh.m_FaceIndices.push_back(std::move(faceIndices));

            mesh.m_NumPolygons[
                static_cast<unsigned int>(faceIndices.size())
            ]++;
        }
    }

    mesh.Rescale();
    mesh.TriangulateFaces();

    return mesh;
}
