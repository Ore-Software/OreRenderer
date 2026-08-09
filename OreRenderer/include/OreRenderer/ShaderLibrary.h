#pragma once

#include <OreRenderer/Shader.h>
#include <OreRenderer/Camera.h>
#include <OreRenderer/Light.h>
#include <OreRenderer/Material.h>

#include <string>
#include <glm/glm.hpp>

enum class ShaderType
{
    Normal,
    Gourand,
    Phong,
    BlinnPhong,
    Gooch,
    Cel,
    CookTorrance
};

struct GoochParams
{
    float warm[3]  = {1.f, 0.25f, 0.f};
    float cool[3]  = {0.f, 0.75f, 1.f};
    float alpha = 0.2f;
    float beta  = 0.2f;
};

struct PBRParams
{
    float metallic  = 0.2f;
    float roughness = 0.3f;
};

class ShaderLibrary
{
public:
    ShaderLibrary(const std::string& shaderDirectory);
    ~ShaderLibrary();

    void SetActiveShader(ShaderType type);
    ShaderType GetActiveShader() const;

    void UploadUniforms(const glm::mat4& model,
                        const Camera& camera,
                        const glm::mat4& projection,
                        const Light& light,
                        const Material& material,
                        const GoochParams& gooch,
                        const PBRParams& pbr);

private:
    class Impl;
    Impl* m_Impl;
};
