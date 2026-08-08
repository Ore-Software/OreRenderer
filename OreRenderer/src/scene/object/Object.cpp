#include <OreRenderer/Object.h>
#include "scene/util/Triangulate.h"

Object::Object()
    : m_Min(0), m_Max(0)
{
}

Object::~Object()
{
    Destroy();
}


void Object::Rescale()
{
    glm::vec3 lengths = m_Max - m_Min;
    float longest = lengths.x;
    if (lengths.y > longest)
        longest = lengths.y;
    if (lengths.z > longest)
        longest = lengths.z;
    glm::vec3 ratios = lengths / longest;

    for (unsigned int i = 0; i < m_VertexPos.size(); i++)
    {
        glm::vec3 vert = m_VertexPos[i];
        glm::vec3 newVert { 0, 0, 0 };
        for (unsigned int coord = 0; coord < 3; coord++)
        {
            newVert[coord] = ratios[coord] * ((vert[coord] - m_Min[coord]) * 2 / lengths[coord] - 1);
        }
        m_VertexPos[i] = newVert;
    }
}

void Object::Destroy()
{
    m_VertexPos.clear();
    m_FaceIndices.clear();
}

void Object::TriangulateFaces()
{
    std::vector<std::vector<unsigned int>> triFaces;

    for (unsigned int i = 0; i < m_FaceIndices.size(); i++)
    {
        if (m_FaceIndices[i].size() == 3)
            triFaces.push_back(m_FaceIndices[i]);
        else if (m_FaceIndices[i].size() > 3)
        {
            unsigned int sides = static_cast<unsigned int>(m_FaceIndices[i].size());
            std::vector<unsigned int> polygon = m_FaceIndices[i];

            std::vector<std::vector<unsigned int>> triangulate = triangulatePolygonalFace(polygon, m_VertexPos);
            for (std::vector<unsigned int> triangle : triangulate)
            {
                triFaces.push_back({polygon[triangle[0]], polygon[triangle[1]] , polygon[triangle[2]] });
            }
        }
    }

    m_TriFaceIndices = triFaces;
}

void Object::MakeTriangleMesh()
{
    m_FaceIndices = m_TriFaceIndices;
    m_NumPolygons = { {3, static_cast<unsigned int>(m_TriFaceIndices.size())} };
}
