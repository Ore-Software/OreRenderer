#include <OreRenderer/ShaderLibrary.h>

#include <vector>
#include <memory>

class ShaderLibrary::Impl
{
public:
    ShaderType activeType = ShaderType::Normal;

    std::unique_ptr<ShaderProgram> normalShader;
    std::unique_ptr<ShaderProgram> gourandShader;
    std::unique_ptr<ShaderProgram> phongShader;
    std::unique_ptr<ShaderProgram> blinnPhongShader;
    std::unique_ptr<ShaderProgram> goochShader;
    std::unique_ptr<ShaderProgram> celShader;
    std::unique_ptr<ShaderProgram> cookTorranceShader;

    Impl(const std::string& dir)
    {
        std::string phongVert = dir + "/phong.vert";
        std::string normalVert = dir + "/normal.vert";
        std::string gourandVert = dir + "/gourand.vert";

        normalShader       = std::make_unique<ShaderProgram>(normalVert,  dir + "/normal.frag");
        gourandShader      = std::make_unique<ShaderProgram>(gourandVert, dir + "/gourand.frag");
        phongShader        = std::make_unique<ShaderProgram>(phongVert,   dir + "/phong.frag");
        blinnPhongShader   = std::make_unique<ShaderProgram>(phongVert,   dir + "/blinnPhong.frag");
        goochShader        = std::make_unique<ShaderProgram>(phongVert,   dir + "/gooch.frag");
        celShader          = std::make_unique<ShaderProgram>(phongVert,   dir + "/cel.frag");
        cookTorranceShader = std::make_unique<ShaderProgram>(phongVert,   dir + "/cookTorrance.frag");

        GetActive().Bind();
    }

    ShaderProgram& GetActive()
    {
        switch (activeType)
        {
        case ShaderType::Gourand:       return *gourandShader;
        case ShaderType::Phong:         return *phongShader;
        case ShaderType::BlinnPhong:    return *blinnPhongShader;
        case ShaderType::Gooch:         return *goochShader;
        case ShaderType::Cel:           return *celShader;
        case ShaderType::CookTorrance:  return *cookTorranceShader;
        default:                        return *normalShader;
        }
    }

    bool NeedsLighting() const
    {
        return activeType != ShaderType::Normal;
    }
};

ShaderLibrary::ShaderLibrary(const std::string& shaderDirectory)
    : m_Impl(new Impl(shaderDirectory))
{
}

ShaderLibrary::~ShaderLibrary()
{
    delete m_Impl;
}

void ShaderLibrary::SetActiveShader(ShaderType type)
{
    if (type != m_Impl->activeType)
    {
        m_Impl->activeType = type;
        m_Impl->GetActive().Bind();
    }
}

ShaderType ShaderLibrary::GetActiveShader() const
{
    return m_Impl->activeType;
}

void ShaderLibrary::UploadUniforms(const glm::mat4& model,
                                   const Camera& camera,
                                   const glm::mat4& projection,
                                   const Light& light,
                                   const Material& material,
                                   const GoochParams& gooch,
                                   const PBRParams& pbr)
{
    ShaderProgram& shader = m_Impl->GetActive();

    shader.SetUniformMat4f("u_Model", model);
    shader.SetUniformMat4f("u_View", camera.GetViewMatrix());
    shader.SetUniformMat4f("u_Projection", projection);

    if (m_Impl->NeedsLighting())
    {
        shader.SetUniform3fv("light_pos", 3, light.m_Pos.data());
        shader.SetUniform3fv("light_col", 3, light.m_Col.data());
        shader.SetUniform3f("light_brightness",
                            light.m_Brightness[0],
                            light.m_Brightness[1],
                            light.m_Brightness[2]);

        std::vector<int> toggled(3);
        for (unsigned int i = 0; i < 3; i++)
            toggled[i] = static_cast<int>(light.m_LightsToggled[i]);
        shader.SetUniform1iv("light_toggled", 3, toggled.data());

        shader.SetUniform3fv("ambient", 1, material.m_Ambient.data());
        shader.SetUniform3fv("diffuse", 1, material.m_Diffuse.data());
        shader.SetUniform3fv("specular", 1, material.m_Specular.data());
        shader.SetUniform1f("shine", material.m_Shine);

        if (m_Impl->activeType == ShaderType::Gooch)
        {
            shader.SetUniform3fv("warm", 1, gooch.warm);
            shader.SetUniform3fv("cool", 1, gooch.cool);
            shader.SetUniform1f("alpha", gooch.alpha);
            shader.SetUniform1f("beta", gooch.beta);
        }

        if (m_Impl->activeType == ShaderType::CookTorrance)
        {
            shader.SetUniform1f("metallic", pbr.metallic);
            shader.SetUniform1f("roughness", pbr.roughness);
        }
    }
}
