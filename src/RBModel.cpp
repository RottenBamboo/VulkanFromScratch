#include "RBModel.h"

namespace RottenBamboo {

    RBModel::RBModel(RBDevice &device, RBCommandBuffer &commandBuffer)
        : device(device), commandBuffer(commandBuffer)
    {
    }

    RBModel::~RBModel()
    {
    }

    void RBModel::LoadFromFile(const std::string& path)
    {
    }
}
