#pragma once

#include <vector>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>


std::vector<glm::vec2> projectPolygonToPlane(std::vector<glm::vec3> polygonVertices);
