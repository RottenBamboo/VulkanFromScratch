//
// Created by rottenbamboo on 2025/4/12.
//
#include "RBPipelineUtils.h"
#include <fstream>
#include <stdexcept>

namespace RottenBamboo {

#ifndef __ANDROID__
    std::vector<char> RBPipelineUtils::readFile(const std::string &filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }
        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }
#else
    std::vector<char> RBPipelineUtils::readFile(const std::string &filename) {
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", "Reading shader file: %s", filename.c_str());
    
    // sdl load file
    SDL_IOStream* file = SDL_IOFromFile(filename.c_str(), "rb");
    if (!file) {
        // try alternative path
        std::string altPath = "assets/" + filename;
        file = SDL_IOFromFile(altPath.c_str(), "rb");
        if (!file) {
            __android_log_print(ANDROID_LOG_ERROR, "RottenBamboo", 
                               "Failed to open shader file: %s, error: %s", 
                               filename.c_str(), SDL_GetError());
            throw std::runtime_error("failed to open file!");
        } else {
            __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                               "Successfully opened shader file from alt path: %s", altPath.c_str());
        }
    }
    
    // load file into memory
    Sint64 fileSize = SDL_GetIOSize(file);
    if (fileSize <= 0) {
        SDL_CloseIO(file);
        throw std::runtime_error("failed to get file size!");
    }
    
    std::vector<char> buffer(fileSize);
    size_t bytesRead = SDL_ReadIO(file, buffer.data(), fileSize);
    SDL_CloseIO(file);
    
    if (bytesRead != static_cast<size_t>(fileSize)) {
        throw std::runtime_error("failed to read entire file!");
    }
    
    __android_log_print(ANDROID_LOG_INFO, "RottenBamboo", 
                       "Shader file loaded successfully: %s, size: %zu bytes", 
                       filename.c_str(), buffer.size());
    
    return buffer;
    }
#endif
    int RBPipelineUtils::SpirvReflectExample(const void* spirv_code, size_t spirv_nbytes)
    {
        // Generate reflection data for a shader
        SpvReflectShaderModule module;
        SpvReflectResult result = spvReflectCreateShaderModule(spirv_nbytes, spirv_code, &module);
        if (result != SPV_REFLECT_RESULT_SUCCESS) {
            return -1; //error
        }

        // 1. get Descriptor Bindings
        uint32_t binding_count = 0;
        result = spvReflectEnumerateDescriptorBindings(&module, &binding_count, nullptr);
        if (result != SPV_REFLECT_RESULT_SUCCESS) {
            spvReflectDestroyShaderModule(&module);
            return -1;
        }

        std::vector<SpvReflectDescriptorBinding*> bindings(binding_count);
        result = spvReflectEnumerateDescriptorBindings(&module, &binding_count, bindings.data());
        if (result != SPV_REFLECT_RESULT_SUCCESS) {
            spvReflectDestroyShaderModule(&module);
            return -1;
        }

        // print all descriptor bindings
        std::cout << "=== Descriptor Bindings ===" << std::endl;
        for (uint32_t i = 0; i < binding_count; ++i) {
            const SpvReflectDescriptorBinding* binding = bindings[i];
            std::cout << "Binding: " << binding->binding << std::endl;
            std::cout << "  Name: " << (binding->name ? binding->name : "NULL") << std::endl;
            std::cout << "  Type: " << GetDescriptorTypeName(binding->descriptor_type) << std::endl;
            std::cout << "  Set: " << binding->set << std::endl;
            std::cout << "  Count: " << binding->count << std::endl;
        }

        // 2. get Descriptor Sets
        uint32_t set_count = 0;
        result = spvReflectEnumerateDescriptorSets(&module, &set_count, nullptr);
        if (result == SPV_REFLECT_RESULT_SUCCESS && set_count > 0) {
            std::vector<SpvReflectDescriptorSet*> sets(set_count);
            result = spvReflectEnumerateDescriptorSets(&module, &set_count, sets.data());

            std::cout << "\n=== Descriptor Sets ===" << std::endl;
            for (uint32_t i = 0; i < set_count; ++i) {
                const SpvReflectDescriptorSet* set = sets[i];
                std::cout << "Set: " << set->set << std::endl;
                std::cout << "  Bindings count: " << set->binding_count << std::endl;
            }
        }

        // 3. get input variables from vertex shader
        uint32_t input_count = 0;
        result = spvReflectEnumerateInputVariables(&module, &input_count, nullptr);
        if (result == SPV_REFLECT_RESULT_SUCCESS && input_count > 0) {
            std::vector<SpvReflectInterfaceVariable*> inputs(input_count);
            result = spvReflectEnumerateInputVariables(&module, &input_count, inputs.data());

            std::cout << "\n=== Input Variables ===" << std::endl;
            for (uint32_t i = 0; i < input_count; ++i) {
                const SpvReflectInterfaceVariable* input = inputs[i];
                if (input->built_in == -1) { // not built-in variable
                    std::cout << "Location: " << input->location << std::endl;
                    std::cout << "  Name: " << (input->name ? input->name : "NULL") << std::endl;
                    std::cout << "  Format: " << input->format << std::endl;
                }
            }
        }

        // 4. get varying variables
        uint32_t output_count = 0;
        result = spvReflectEnumerateOutputVariables(&module, &output_count, nullptr);
        if (result == SPV_REFLECT_RESULT_SUCCESS && output_count > 0) {
            std::vector<SpvReflectInterfaceVariable*> outputs(output_count);
            result = spvReflectEnumerateOutputVariables(&module, &output_count, outputs.data());

            std::cout << "\n=== Output Variables ===" << std::endl;
            for (uint32_t i = 0; i < output_count; ++i) {
                const SpvReflectInterfaceVariable* output = outputs[i];
                if (output->built_in == -1) { // not built-in variable
                    std::cout << "Location: " << output->location << std::endl;
                    std::cout << "  Name: " << (output->name ? output->name : "NULL") << std::endl;
                }
            }
        }

        // 5. get Push Constants
        uint32_t push_constant_count = 0;
        result = spvReflectEnumeratePushConstantBlocks(&module, &push_constant_count, nullptr);
        if (result == SPV_REFLECT_RESULT_SUCCESS && push_constant_count > 0) {
            std::vector<SpvReflectBlockVariable*> push_constants(push_constant_count);
            result = spvReflectEnumeratePushConstantBlocks(&module, &push_constant_count, push_constants.data());

            std::cout << "\n=== Push Constants ===" << std::endl;
            for (uint32_t i = 0; i < push_constant_count; ++i) {
                const SpvReflectBlockVariable* push_constant = push_constants[i];
                std::cout << "Name: " << (push_constant->name ? push_constant->name : "NULL") << std::endl;
                std::cout << "  Size: " << push_constant->size << std::endl;
            }
        }

        // shader stage
        std::cout << "\n=== Shader Stage ===" << std::endl;
        std::cout << "Stage: " << GetShaderStageName(module.shader_stage) << std::endl;
        std::cout << "Entry Point: " << module.entry_point_name << std::endl;

        // destroy shader module
        spvReflectDestroyShaderModule(&module);

        return 0;
    }

    // Helper function: Get Descriptor Type Name
    const char* RBPipelineUtils::GetDescriptorTypeName(SpvReflectDescriptorType type) 
    {
        switch (type) 
        {
        case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER: return "SAMPLER";
        case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return "COMBINED_IMAGE_SAMPLER";
        case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE: return "SAMPLED_IMAGE";
        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE: return "STORAGE_IMAGE";
        case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER: return "UNIFORM_TEXEL_BUFFER";
        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: return "STORAGE_TEXEL_BUFFER";
        case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER: return "UNIFORM_BUFFER";
        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER: return "STORAGE_BUFFER";
        case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: return "UNIFORM_BUFFER_DYNAMIC";
        case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: return "STORAGE_BUFFER_DYNAMIC";
        case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: return "INPUT_ATTACHMENT";
        case SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: return "ACCELERATION_STRUCTURE_KHR";
        default: return "UNKNOWN";
        }
    }

    // Helper function: Get Shader Stage Name
    const char* RBPipelineUtils::GetShaderStageName(SpvReflectShaderStageFlagBits stage) 
    {
        switch (stage) 
        {
            case SPV_REFLECT_SHADER_STAGE_VERTEX_BIT: return "VERTEX";
            case SPV_REFLECT_SHADER_STAGE_TESSELLATION_CONTROL_BIT: return "TESSELLATION_CONTROL";
            case SPV_REFLECT_SHADER_STAGE_TESSELLATION_EVALUATION_BIT: return "TESSELLATION_EVALUATION";
            case SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT: return "GEOMETRY";
            case SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT: return "FRAGMENT";
            case SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT: return "COMPUTE";
            case SPV_REFLECT_SHADER_STAGE_RAYGEN_BIT_KHR: return "RAYGEN";
            case SPV_REFLECT_SHADER_STAGE_ANY_HIT_BIT_KHR: return "ANY_HIT";
            case SPV_REFLECT_SHADER_STAGE_CLOSEST_HIT_BIT_KHR: return "CLOSEST_HIT";
            case SPV_REFLECT_SHADER_STAGE_MISS_BIT_KHR: return "MISS";
            case SPV_REFLECT_SHADER_STAGE_INTERSECTION_BIT_KHR: return "INTERSECTION";
            case SPV_REFLECT_SHADER_STAGE_CALLABLE_BIT_KHR: return "CALLABLE";
            case SPV_REFLECT_SHADER_STAGE_TASK_BIT_NV: return "TASK";
            case SPV_REFLECT_SHADER_STAGE_MESH_BIT_NV: return "MESH";
            default: return "UNKNOWN";
        }
    }
}