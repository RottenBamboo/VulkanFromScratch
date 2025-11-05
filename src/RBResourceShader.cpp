#include "RBResourceShader.h"
#include "RBPipelineUtils.h"

namespace RottenBamboo {

    void RBResourceShader::Load(const std::string& path)
    {
        shaderCode = RBPipelineUtils::readFile(path);
    }
}