// Copyright 2017 The Dawn Authors
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

#include "dawn/native/metal/PipelineLayoutMTL.h"

#include "dawn/common/BitSetIterator.h"
#include "dawn/native/BindGroupLayoutInternal.h"
#include "dawn/native/metal/DeviceMTL.h"

namespace dawn::native::metal {

// static
Ref<PipelineLayout> PipelineLayout::Create(Device* device,
                                           const PipelineLayoutDescriptor* descriptor) {
    return AcquireRef(new PipelineLayout(device, descriptor));
}

PipelineLayout::PipelineLayout(Device* device, const PipelineLayoutDescriptor* descriptor)
    : PipelineLayoutBase(device, descriptor) {
    // Each stage has its own numbering namespace in CompilerMSL.
    for (auto stage : IterateStages(kAllStages)) {
        uint32_t bufferIndex = 0;
        uint32_t samplerIndex = 0;
        uint32_t textureIndex = 0;

        for (BindGroupIndex group : IterateBitSet(GetBindGroupLayoutsMask())) {
            mIndexInfo[stage][group].resize(GetBindGroupLayout(group)->GetBindingCount());

            for (BindingIndex bindingIndex{0};
                 bindingIndex < GetBindGroupLayout(group)->GetBindingCount(); ++bindingIndex) {
                const BindingInfo& bindingInfo =
                    GetBindGroupLayout(group)->GetBindingInfo(bindingIndex);
                if (!(bindingInfo.visibility & StageBit(stage))) {
                    continue;
                }

                switch (bindingInfo.bindingType) {
                    case BindingInfoType::Buffer:
                        mIndexInfo[stage][group][bindingIndex] = bufferIndex;
                        bufferIndex++;
                        break;

                    case BindingInfoType::Sampler:
                        mIndexInfo[stage][group][bindingIndex] = samplerIndex;
                        samplerIndex++;
                        break;

                    case BindingInfoType::Texture:
                    case BindingInfoType::StorageTexture:
                    case BindingInfoType::ExternalTexture:
                        mIndexInfo[stage][group][bindingIndex] = textureIndex;
                        textureIndex++;
                        break;
                }
            }
        }

        mBufferBindingCount[stage] = bufferIndex;
    }
}

PipelineLayout::~PipelineLayout() = default;

const PipelineLayout::BindingIndexInfo& PipelineLayout::GetBindingIndexInfo(
    SingleShaderStage stage) const {
    return mIndexInfo[stage];
}

uint32_t PipelineLayout::GetBufferBindingCount(SingleShaderStage stage) const {
    return mBufferBindingCount[stage];
}

}  // namespace dawn::native::metal
