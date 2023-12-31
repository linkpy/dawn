// Copyright 2021 The Dawn Authors
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

#include "src/dawn/node/binding/GPUCommandBuffer.h"

#include <utility>

#include "src/dawn/node/utils/Debug.h"

namespace wgpu::binding {

////////////////////////////////////////////////////////////////////////////////
// wgpu::bindings::GPUCommandBuffer
////////////////////////////////////////////////////////////////////////////////

GPUCommandBuffer::GPUCommandBuffer(const wgpu::CommandBufferDescriptor& desc,
                                   wgpu::CommandBuffer cmd_buf)
    : cmd_buf_(std::move(cmd_buf)), label_(desc.label ? desc.label : "") {}

std::string GPUCommandBuffer::getLabel(Napi::Env) {
    return label_;
}

void GPUCommandBuffer::setLabel(Napi::Env, std::string value) {
    cmd_buf_.SetLabel(value.c_str());
    label_ = value;
}

}  // namespace wgpu::binding
