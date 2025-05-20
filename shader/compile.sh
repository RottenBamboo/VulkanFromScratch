mkdir -p bin
~/VulkanSDK/1.4.309.0/macOS/bin/glslc gBuffer.vert -o bin/gBufferVert.spv
~/VulkanSDK/1.4.309.0/macOS/bin/glslc gBuffer.frag -o bin/gBufferFrag.spv
~/VulkanSDK/1.4.309.0/macOS/bin/glslc lighting.vert -o bin/lightingVert.spv
~/VulkanSDK/1.4.309.0/macOS/bin/glslc lighting.frag -o bin/lightingFrag.spv