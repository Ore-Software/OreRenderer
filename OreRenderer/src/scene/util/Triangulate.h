#pragma once

#include <vector>
#include <limits>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>

#include "PlaneProjection.h"

// can assume faceIdx.size() > 3
std::vector<std::vector<unsigned int>> triangulatePolygonalFace(const std::vector<unsigned int>& faceIdx, const std::vector<glm::vec3>& allVertices);
