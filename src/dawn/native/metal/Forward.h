// Copyright 2018 The Dawn Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SRC_DAWN_NATIVE_METAL_FORWARD_H_
#define SRC_DAWN_NATIVE_METAL_FORWARD_H_

#include "dawn/native/ToBackend.h"

namespace dawn::native::metal {

class BindGroup;
class BindGroupLayout;
class Buffer;
class CommandBuffer;
class ComputePipeline;
class Device;
class Framebuffer;
class PhysicalDevice;
class PipelineLayout;
class QuerySet;
class Queue;
class RenderPipeline;
class Sampler;
class SharedFence;
class SharedTextureMemory;
class ShaderModule;
class SwapChain;
class Texture;
class TextureView;

struct MetalBackendTraits {
    using BindGroupType = BindGroup;
    using BindGroupLayoutType = BindGroupLayout;
    using BufferType = Buffer;
    using CommandBufferType = CommandBuffer;
    using ComputePipelineType = ComputePipeline;
    using DeviceType = Device;
    using PhysicalDeviceType = PhysicalDevice;
    using PipelineLayoutType = PipelineLayout;
    using QuerySetType = QuerySet;
    using QueueType = Queue;
    using RenderPipelineType = RenderPipeline;
    using SamplerType = Sampler;
    using ShaderModuleType = ShaderModule;
    using SharedFenceType = SharedFence;
    using SharedTextureMemoryType = SharedTextureMemory;
    using SwapChainType = SwapChain;
    using TextureType = Texture;
    using TextureViewType = TextureView;
};

template <typename T>
auto ToBackend(T&& common) -> decltype(ToBackendBase<MetalBackendTraits>(common)) {
    return ToBackendBase<MetalBackendTraits>(common);
}

}  // namespace dawn::native::metal

#endif  // SRC_DAWN_NATIVE_METAL_FORWARD_H_
