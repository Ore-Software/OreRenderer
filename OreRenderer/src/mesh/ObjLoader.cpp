#include "ObjLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    void customSplit(
        const std::string& str,
        char separator,
        std::vector<std::string>& strings)
    {
        int startIndex = 0;
        int endIndex = 0;

        for (unsigned int i = 0; i <= str.size(); i++)
        {
            if (i == str.size() || str[i] == separator)
            {
                endIndex = i;

                std::string temp;
                temp.append(str, startIndex, endIndex - startIndex);
                strings.push_back(temp);

                startIndex = endIndex + 1;
            }
        }
    }
}

Object ObjLoader::Load(const std::string& filename)
{
    Object object;

    object.m_Min = glm::vec3{
        1000000,
        1000000,
        1000000
    };

    object.m_Max = glm::vec3{
        -1000000,
        -1000000,
        -1000000
    };

    std::ifstream in(filename);

    if (!in)
    {
        std::cerr << "Cannot open " << filename << std::endl;
        return object;
    }

    std::string line;

    while (getline(in, line))
    {
        if (line.substr(0, 2) == "v ")
        {
            std::istringstream s(line.substr(2));

            glm::vec3 v;
            s >> v.x;
            s >> v.y;
            s >> v.z;

            object.m_VertexPos.push_back(v);

            for (unsigned int coord = 0; coord < 3; coord++)
            {
                if (v[coord] > object.m_Max[coord])
                    object.m_Max[coord] = v[coord];

                if (v[coord] < object.m_Min[coord])
                    object.m_Min[coord] = v[coord];
            }
        }
        else if (line.substr(0, 2) == "f ")
        {
            std::string s(line.substr(2));

            std::vector<std::string> face;
            customSplit(s, ' ', face);

            std::vector<unsigned int> faceIndices;

            for (const std::string& corner : face)
            {
                if (corner.empty())
                    continue;

                std::vector<std::string> vertex;
                customSplit(corner, '/', vertex);

                unsigned int vertIdx = std::stoul(vertex[0]);
                vertIdx--;

                faceIndices.push_back(vertIdx);
            }

            object.m_FaceIndices.push_back(faceIndices);

            object.m_NumPolygons[
                static_cast<unsigned int>(faceIndices.size())
            ]++;
        }
    }

    object.Rescale();
    object.TriangulateFaces();

    return object;
}
