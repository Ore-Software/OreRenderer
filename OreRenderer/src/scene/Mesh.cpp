#include <OreRenderer/Mesh.h>
#include "scene/util/Triangulate.h"

////////// Base geometry //////////

Mesh::Mesh()
    : m_Min(0), m_Max(0)
{
}

Mesh::~Mesh()
{
    DestroyRenderData();
    DestroyTopology();
    DestroyGeometry();
}

void Mesh::Rescale()
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
        glm::vec3 newVert{0, 0, 0};
        for (unsigned int coord = 0; coord < 3; coord++)
        {
            newVert[coord] = ratios[coord] * ((vert[coord] - m_Min[coord]) * 2 / lengths[coord] - 1);
        }
        m_VertexPos[i] = newVert;
    }
}

void Mesh::TriangulateFaces()
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
                triFaces.push_back({polygon[triangle[0]], polygon[triangle[1]], polygon[triangle[2]]});
            }
        }
    }

    m_TriFaceIndices = triFaces;
}

void Mesh::MakeTriangleMesh()
{
    m_FaceIndices = m_TriFaceIndices;
    m_NumPolygons = {{3, static_cast<unsigned int>(m_TriFaceIndices.size())}};
}

void Mesh::DestroyGeometry()
{
    m_VertexPos.clear();
    m_FaceIndices.clear();
    m_TriFaceIndices.clear();
    m_NumPolygons.clear();
}

////////// Topology (adjacency information) //////////

unsigned int Mesh::GetVertIndex(glm::vec3 vertPos,
    std::vector<glm::vec3>& AllVertexPos,
    std::unordered_map<float, std::unordered_map<float, std::unordered_map<float, unsigned int>>>& VertIdxLookup)
{
    // search if pos x in our lookup
    auto searchx = VertIdxLookup.find(vertPos.x);
    if (searchx != VertIdxLookup.end())
    {
        // search if pos y in our lookup
        auto searchy = searchx->second.find(vertPos.y);
        if (searchy != searchx->second.end())
        {
            // search if pos z in our lookup
            auto searchz = searchy->second.find(vertPos.z);
            if (searchz != searchy->second.end())
            {
                // found, return vert index
                return searchz->second;
            }
        }
    }
    // not found, create new edge and insert at the end of m_Edges
    unsigned int newVertIdx = static_cast<unsigned int>(AllVertexPos.size());
    AllVertexPos.push_back(vertPos);
    VertIdxLookup[vertPos.x][vertPos.y].insert(std::make_pair(vertPos.z, newVertIdx));
    return newVertIdx;
}

unsigned int Mesh::GetEdgeIndex(glm::uvec2 vertPair)
{
    glm::uvec2 orderedVertPair = vertPair;
    if (vertPair.x > vertPair.y) // check if the order is maintained (idx1 < idx2)
    {
        // if it is violated, swap it
        orderedVertPair.x = vertPair.y;
        orderedVertPair.y = vertPair.x;
    }

    // search if starting vertex in our lookup
    auto searchx = m_EdgeIdxLookup.find(orderedVertPair.x);
    if (searchx != m_EdgeIdxLookup.end())
    {
        // search if ending vertex in our lookup
        auto searchy = searchx->second.find(orderedVertPair.y);
        if (searchy != searchx->second.end())
        {
            // found, return edge index
            return searchy->second;
        }
    }
    // not found, create new edge and insert at the end of m_Edges
    unsigned int newEdgeIdx = static_cast<unsigned int>(m_TopoEdges.size());
    EdgeRecord newEdge;
    newEdge.endPoint1Idx = orderedVertPair.x;
    newEdge.endPoint2Idx = orderedVertPair.y;
    newEdge.midEdgePoint = 0.5f * (
        m_TopoVertices[orderedVertPair.x].position +
        m_TopoVertices[orderedVertPair.y].position
    );
    m_TopoEdges.push_back(newEdge);
    m_EdgeIdxLookup[orderedVertPair.x].insert(std::make_pair(orderedVertPair.y, newEdgeIdx));
    return newEdgeIdx;
}

void Mesh::BuildTopology()
{
    DestroyTopology();

    for (glm::vec3 vertPos : m_VertexPos)
    {
        // populate m_TopoVertices with mesh.m_VertexPos data
        VertexRecord newVertex;
        newVertex.position = vertPos;
        m_TopoVertices.push_back(newVertex);
    }

    for (std::vector<unsigned int> faceVertices : m_FaceIndices)
    {
        unsigned int n = static_cast<unsigned int>(faceVertices.size());

        // create face record to be stored
        FaceRecord newFace;
        newFace.verticesIdx = faceVertices;

        // calculate the face points
        glm::vec3 vertexSum{0};
        for (unsigned int j = 0; j < n; j++)
        {
            vertexSum += m_TopoVertices[faceVertices[j]].position;
        }
        newFace.facePoint = vertexSum / ((float)n);

        // get edges next to the current face
        for (unsigned int i = 0; i < n; i++)
        {
            unsigned int startVertex = faceVertices[i];
            unsigned int endVertex = faceVertices[(i + 1) % n];
            unsigned int edgeIndex = GetEdgeIndex({startVertex, endVertex});

            newFace.verticesEdges[startVertex].push_back(edgeIndex);
            newFace.verticesEdges[endVertex].push_back(edgeIndex);
            newFace.edgesIdx.push_back(edgeIndex);

            // add the edges to the connecting vertices
            m_TopoVertices[startVertex].adjEdgesIdx.push_back(edgeIndex);
            m_TopoVertices[endVertex].adjEdgesIdx.push_back(edgeIndex);
        }

        unsigned int faceIndex = static_cast<unsigned int>(m_TopoFaces.size());
        m_TopoFaces.push_back(newFace);

        // add face index to the connecting vertices and edges 
        for (unsigned int i = 0; i < n; i++)
        {
            m_TopoVertices[faceVertices[i]].adjFacesIdx.push_back(faceIndex);
            m_TopoEdges[newFace.edgesIdx[i]].adjFacesIdx.push_back(faceIndex);
        }
    }

    m_TopologyChanged = false;
}

void Mesh::DestroyTopology()
{
    m_TopoVertices.clear();
    m_TopoEdges.clear();
    m_TopoFaces.clear();
    m_EdgeIdxLookup.clear();
    m_TopologyChanged = true;
}

glm::vec3 Mesh::ComputeFaceNormal(FaceRecord face)
{
    glm::vec3 pos0 = m_TopoVertices[face.verticesIdx[0]].position;
    glm::vec3 pos1 = m_TopoVertices[face.verticesIdx[1]].position;
    glm::vec3 pos2 = m_TopoVertices[face.verticesIdx[2]].position;
    return ComputeFaceNormal(pos0, pos1, pos2);
}

glm::vec3 Mesh::ComputeFaceNormal(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2)
{
    return glm::normalize(glm::cross(pos1 - pos0, pos2 - pos0));
}

////////// Rendering //////////

void Mesh::BuildFaceNormals()
{
    // build face normal vectors
    m_FaceNormals.resize(m_TriFaceIndices.size());
    for (unsigned int i = 0; i < m_TriFaceIndices.size(); i++)
    {
        unsigned int ia = m_TriFaceIndices[i][0];
        unsigned int ib = m_TriFaceIndices[i][1];
        unsigned int ic = m_TriFaceIndices[i][2];
        glm::vec3 normal = glm::normalize(
            glm::cross(
                m_VertexPos[ib] - m_VertexPos[ia],
                m_VertexPos[ic] - m_VertexPos[ia]
            )
        );
        m_FaceNormals[i] = normal;
    }
}

void Mesh::BuildVerticesIndices()
{
    unsigned int numFaces = static_cast<unsigned int>(m_TriFaceIndices.size());

    // build output items to OpenGL
    if (m_ShadingType == FLAT)
    {
        // build out the VBO with x,y,z coords of vertices, and normal vectors
        m_OutNumVert = 2 * 3 * 3 * numFaces;
        m_OutVertices = new float[m_OutNumVert]{};
        for (unsigned int i = 0; i < numFaces; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                // ith face, jth corner, xyz coordinates and normals
                m_OutVertices[18 * i + 6 * j + 0] = m_VertexPos[m_TriFaceIndices[i][j]].x;
                m_OutVertices[18 * i + 6 * j + 1] = m_VertexPos[m_TriFaceIndices[i][j]].y;
                m_OutVertices[18 * i + 6 * j + 2] = m_VertexPos[m_TriFaceIndices[i][j]].z;

                m_OutVertices[18 * i + 6 * j + 3] = m_FaceNormals[i].x;
                m_OutVertices[18 * i + 6 * j + 4] = m_FaceNormals[i].y;
                m_OutVertices[18 * i + 6 * j + 5] = m_FaceNormals[i].z;
            }
        }

        // build out IBO indices
        m_OutNumIdx = 3 * 3 * numFaces;
        m_OutIndices = new unsigned int[m_OutNumIdx];
        for (unsigned int i = 0; i < m_OutNumIdx; i++)
        {
            m_OutIndices[i] = i;
        }
    }
    else if (m_ShadingType == MIXED) // mixed shading
    {
        // get vertex-face connectivity: get all faces that touch the a given vertex
        std::unordered_map<unsigned int, std::vector<unsigned int>> vertAdjFaces(m_VertexPos.size());
        for (unsigned int faceIdx = 0; faceIdx < numFaces; faceIdx++)
        {
            std::vector<unsigned int> faceVertIdx = m_TriFaceIndices[faceIdx];
            // add face index to the connecting vertices
            for (unsigned int i = 0; i < 3; i++)
            {
                vertAdjFaces[faceVertIdx[i]].push_back(faceIdx);
            }
        }

        // build mixed vertex normals by first getting current face normal, then average adjacent normals
        std::vector<std::vector<glm::vec3>> cornerVertexNormals(numFaces);
        for (unsigned int currFace = 0; currFace < numFaces; currFace++)
        {
            // get face normal
            glm::vec3 currFaceNormal = m_FaceNormals[currFace];
            std::vector<glm::vec3> faceCornerNormals;

            for (unsigned int currCorner = 0; currCorner < m_TriFaceIndices[currFace].size(); currCorner++)
            {
                // corner normal to be stored
                glm::vec3 currCornerNormal{0};

                for (unsigned int adjFace : vertAdjFaces[m_TriFaceIndices[currFace][currCorner]])
                {
                    glm::vec3 adjFaceNormal = m_FaceNormals[adjFace];
                    // if adjacent face is "close" to current face, then add the adj face normal to current corner normal
                    if (glm::dot(currFaceNormal, adjFaceNormal) > 0.9f) // threshold set to 0.9f here
                    {
                        currCornerNormal += adjFaceNormal;
                    }
                }
                faceCornerNormals.push_back(glm::normalize(currCornerNormal));
            }
            cornerVertexNormals[currFace] = faceCornerNormals;
        }

        // build out the VBO with x,y,z coords of vertices, and normal vectors
        m_OutNumVert = 2 * 3 * 3 * numFaces;
        m_OutVertices = new float[m_OutNumVert]{};
        for (unsigned int i = 0; i < numFaces; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                // ith face, jth corner, xyz coordinates and normals
                m_OutVertices[18 * i + 6 * j + 0] = m_VertexPos[m_TriFaceIndices[i][j]].x;
                m_OutVertices[18 * i + 6 * j + 1] = m_VertexPos[m_TriFaceIndices[i][j]].y;
                m_OutVertices[18 * i + 6 * j + 2] = m_VertexPos[m_TriFaceIndices[i][j]].z;

                m_OutVertices[18 * i + 6 * j + 3] = cornerVertexNormals[i][j].x;
                m_OutVertices[18 * i + 6 * j + 4] = cornerVertexNormals[i][j].y;
                m_OutVertices[18 * i + 6 * j + 5] = cornerVertexNormals[i][j].z;
            }
        }

        // build out IBO indices
        m_OutNumIdx = 3 * 3 * numFaces;
        m_OutIndices = new unsigned int[m_OutNumIdx];
        for (unsigned int i = 0; i < m_OutNumIdx; i++)
        {
            m_OutIndices[i] = i;
        }
    }
    else if (m_ShadingType == SMOOTH) // smooth shading
    {
        // get vertex-face connectivity: get all faces that touch the a given vertex
        std::unordered_map<unsigned int, std::vector<unsigned int>> vertAdjFaces(m_VertexPos.size());
        for (unsigned int faceIdx = 0; faceIdx < numFaces; faceIdx++)
        {
            std::vector<unsigned int> faceVertIdx = m_TriFaceIndices[faceIdx];
            // add face index to the connecting vertices
            for (unsigned int i = 0; i < 3; i++)
            {
                vertAdjFaces[faceVertIdx[i]].push_back(faceIdx);
            }
        }

        // build smooth vertex normals by average neighbouring faces normals
        unsigned int numVertices = static_cast<unsigned int>(m_VertexPos.size());
        std::vector<glm::vec3> smoothVertexNormals(numVertices);
        for (unsigned int i = 0; i < numVertices; i++)
        {
            glm::vec3 currVertNormal = glm::vec3(0, 0, 0);

            // summ through each neighbouring face
            for (unsigned int adjFace : vertAdjFaces[i])
            {
                currVertNormal += m_FaceNormals[adjFace];
            }

            smoothVertexNormals[i] = glm::normalize(currVertNormal);
        }

        // build out the VBO with x,y,z coords of vertices, and normal vectors
        m_OutNumVert = 2 * 3 * numVertices;
        m_OutVertices = new float[m_OutNumVert]{};
        for (unsigned int i = 0; i < numVertices; i++)
        {
            // x value of the vertex
            m_OutVertices[6 * i + 0] = m_VertexPos[i].x;
            // y value of the vertex
            m_OutVertices[6 * i + 1] = m_VertexPos[i].y;
            // z value of the vertex
            m_OutVertices[6 * i + 2] = m_VertexPos[i].z;

            // x value of the normal
            m_OutVertices[6 * i + 3] = smoothVertexNormals[i].x;
            // y value of the normal
            m_OutVertices[6 * i + 4] = smoothVertexNormals[i].y;
            // z value of the normal
            m_OutVertices[6 * i + 5] = smoothVertexNormals[i].z;
        }

        // build out IBO indices
        m_OutNumIdx = 3 * numFaces;
        m_OutIndices = new unsigned int[m_OutNumIdx];
        for (unsigned int i = 0; i < numFaces; i++)
        {
            m_OutIndices[3 * i + 0] = m_TriFaceIndices[i][0];
            m_OutIndices[3 * i + 1] = m_TriFaceIndices[i][1];
            m_OutIndices[3 * i + 2] = m_TriFaceIndices[i][2];
        }
    }

    m_RenderChanged = false;
}

void Mesh::DestroyRenderData()
{
    m_FaceNormals.clear();

    delete[] m_OutVertices;
    m_OutVertices = nullptr;
    delete[] m_OutIndices;
    m_OutIndices = nullptr;
    m_OutNumVert = 0;
    m_OutNumIdx = 0;
    m_RenderChanged = true;
}

void Mesh::Rebuild()
{
    DestroyRenderData();
    BuildFaceNormals();
    BuildVerticesIndices();
}

void Mesh::Rebuild(int shading)
{
    m_ShadingType = shading;
    Rebuild();
}
