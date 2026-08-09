#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <set>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_uint2.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/geometric.hpp>

enum Shading
{
	FLAT,
	MIXED,
	SMOOTH
};

struct VertexRecord
{
	glm::vec3 position;
	std::vector<unsigned int> adjEdgesIdx;
	std::vector<unsigned int> adjFacesIdx;
};

struct EdgeRecord
{
	unsigned endPoint1Idx; // maintain that endpoint1 < endpoint2 for searching
	unsigned endPoint2Idx;
	glm::vec3 midEdgePoint;
	std::vector<unsigned int> adjFacesIdx; // should have 1 or 2 adjacent faces
};

struct FaceRecord
{
	glm::vec3 facePoint;
	std::vector<unsigned int> verticesIdx; // each face can have n vertices
	std::unordered_map<unsigned int, std::vector<unsigned int>> verticesEdges; // each vertex will have 2 edges
	std::vector<unsigned int> edgesIdx; // each face can have n edges
};

struct ValidPair
{
	unsigned int vertOne;
	unsigned int vertTwo;
	bool edge;
	float error;
	glm::vec3 newVert;
	// weight parameter balancing point vs line quadrics
	// only for augmented version
	float alpha;
};

struct CompareValidPairs
{
	bool operator()(const ValidPair& a, const ValidPair& b) const
	{
		return a.error > b.error; // min heap
	}
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	// Base geometry
	void Rescale();
	void TriangulateFaces();
	void MakeTriangleMesh();
	void DestroyGeometry();

	// Topology
	void BuildTopology();
	void DestroyTopology();

	// given the position of a vertex, find index in the set inputted
	// if it does not exist, insert into our map and return
	unsigned int GetVertIndex(glm::vec3 vertPos,
		std::vector<glm::vec3>& AllVertexPos,
		std::unordered_map<float, std::unordered_map<float, std::unordered_map<float, unsigned int>>>& VertIdxLookup);
	// given the pair of indices in m_Vertices, find index in m_Edges
	// if it does not exist, insert into our map and return
	unsigned int GetEdgeIndex(glm::uvec2 vertPair);

	glm::vec3 ComputeFaceNormal(FaceRecord face);
	glm::vec3 ComputeFaceNormal(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2);

	// Rendering
	void BuildFaceNormals();
	void BuildVerticesIndices();
	void DestroyRenderData();

	void Rebuild();
	void Rebuild(int shading);

public:
	// Base geometry
	glm::vec3 m_Min{0};
	glm::vec3 m_Max{0};
	std::vector<glm::vec3> m_VertexPos;
	std::vector<std::vector<unsigned int>> m_FaceIndices;
	std::vector<std::vector<unsigned int>> m_TriFaceIndices;
	std::unordered_map<unsigned int, unsigned int> m_NumPolygons;

	// Topology
	std::vector<VertexRecord> m_TopoVertices;
	std::vector<EdgeRecord> m_TopoEdges;
	std::vector<FaceRecord> m_TopoFaces;
	std::unordered_map<unsigned int, std::unordered_map<unsigned int, unsigned int>> m_EdgeIdxLookup;
	bool m_TopologyChanged = true;

	// Rendering
	int m_ShadingType = FLAT;
	std::vector<glm::vec3> m_FaceNormals;

	unsigned int m_OutNumVert = 0;
	float* m_OutVertices = nullptr;
	unsigned int m_OutNumIdx = 0;
	unsigned int* m_OutIndices = nullptr;
	bool m_RenderChanged = true;

private:
	std::priority_queue<ValidPair, std::vector<ValidPair>, CompareValidPairs> m_QuadricErrorHeap;
};
