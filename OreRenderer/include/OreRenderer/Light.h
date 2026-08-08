#pragma once

#include<vector>

#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>

class Light
{
public:
	Light();
	~Light();

public:
	bool* m_LightsToggled;
	std::vector<float> m_Pos;
	std::vector<float> m_Col;
	std::vector<float> m_Brightness;
};