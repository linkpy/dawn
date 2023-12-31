// Copyright 2019 The Dawn Authors
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

// VulkanBackend.cpp: contains the definition of symbols exported by VulkanBackend.h so that they
// can be compiled twice: once export (shared library), once not exported (static library)

// Include vulkan_platform.h before VulkanBackend.h includes vulkan.h so that we use our version
// of the non-dispatchable handles.
#include "dawn/common/vulkan_platform.h"

#include "dawn/native/VulkanBackend.h"

#include "dawn/native/vulkan/DeviceVk.h"
#include "dawn/native/vulkan/TextureVk.h"
#include "dawn/native/vulkan/PhysicalDeviceVk.h"


namespace dawn::native::vulkan {

VkInstance GetInstance(WGPUDevice device) {
    Device* backendDevice = ToBackend(FromAPI(device));
    return backendDevice->GetVkInstance();
}

DAWN_NATIVE_EXPORT PFN_vkVoidFunction GetInstanceProcAddr(WGPUDevice device, const char* pName) {
    Device* backendDevice = ToBackend(FromAPI(device));
    return (*backendDevice->fn.GetInstanceProcAddr)(backendDevice->GetVkInstance(), pName);
}

uint32_t GetVulkanGraphicsQueueFamilyIndex(WGPUDevice device) {
    Device* dev = ToBackend(FromAPI(device));
    return dev->GetGraphicsQueueFamily();
}

uint32_t GetVulkanGraphicsQueueIndex(WGPUDevice device) {
    // The vulkan backend always creates a single queue, so the queue index will 
    // always be 0.
    return 0; 
}

VulkanFunctionOverrides::VulkanFunctionOverrides() {
    sType = wgpu::SType::VulkanFunctionOverrides;
}

PhysicalDeviceDiscoveryOptions::PhysicalDeviceDiscoveryOptions()
    : PhysicalDeviceDiscoveryOptionsBase(WGPUBackendType_Vulkan) {}

#if DAWN_PLATFORM_IS(LINUX)
ExternalImageDescriptorOpaqueFD::ExternalImageDescriptorOpaqueFD()
    : ExternalImageDescriptorFD(ExternalImageType::OpaqueFD) {}

ExternalImageDescriptorDmaBuf::ExternalImageDescriptorDmaBuf()
    : ExternalImageDescriptorFD(ExternalImageType::DmaBuf) {}

ExternalImageExportInfoOpaqueFD::ExternalImageExportInfoOpaqueFD()
    : ExternalImageExportInfoFD(ExternalImageType::OpaqueFD) {}

ExternalImageExportInfoDmaBuf::ExternalImageExportInfoDmaBuf()
    : ExternalImageExportInfoFD(ExternalImageType::DmaBuf) {}
#endif  // DAWN_PLATFORM_IS(LINUX)

#if DAWN_PLATFORM_IS(ANDROID)
ExternalImageDescriptorAHardwareBuffer::ExternalImageDescriptorAHardwareBuffer()
    : ExternalImageDescriptorVk(ExternalImageType::AHardwareBuffer) {}

ExternalImageExportInfoAHardwareBuffer::ExternalImageExportInfoAHardwareBuffer()
    : ExternalImageExportInfoFD(ExternalImageType::AHardwareBuffer) {}
#endif

WGPUTexture WrapVulkanImage(WGPUDevice device, const ExternalImageDescriptorVk* descriptor) {
    switch (descriptor->GetType()) {
#if DAWN_PLATFORM_IS(ANDROID)
        case ExternalImageType::AHardwareBuffer: {
            Device* backendDevice = ToBackend(FromAPI(device));
            const ExternalImageDescriptorAHardwareBuffer* ahbDescriptor =
                static_cast<const ExternalImageDescriptorAHardwareBuffer*>(descriptor);

            return ToAPI(backendDevice->CreateTextureWrappingVulkanImage(
                ahbDescriptor, ahbDescriptor->handle, ahbDescriptor->waitFDs));
        }
#elif DAWN_PLATFORM_IS(LINUX)
        case ExternalImageType::OpaqueFD:
        case ExternalImageType::DmaBuf: {
            Device* backendDevice = ToBackend(FromAPI(device));
            const ExternalImageDescriptorFD* fdDescriptor =
                static_cast<const ExternalImageDescriptorFD*>(descriptor);

            return ToAPI(backendDevice->CreateTextureWrappingVulkanImage(
                fdDescriptor, fdDescriptor->memoryFD, fdDescriptor->waitFDs));
        }
#endif  // DAWN_PLATFORM_IS(LINUX)

        default:
            return nullptr;
    }
}

bool ExportVulkanImage(WGPUTexture texture,
                       VkImageLayout desiredLayout,
                       ExternalImageExportInfoVk* info) {
    if (texture == nullptr) {
        return false;
    }
#if DAWN_PLATFORM_IS(ANDROID) || DAWN_PLATFORM_IS(LINUX)
    switch (info->GetType()) {
        case ExternalImageType::AHardwareBuffer:
        case ExternalImageType::OpaqueFD:
        case ExternalImageType::DmaBuf: {
            Texture* backendTexture = ToBackend(FromAPI(texture));
            Device* device = ToBackend(backendTexture->GetDevice());
            ExternalImageExportInfoFD* fdInfo = static_cast<ExternalImageExportInfoFD*>(info);

            return device->SignalAndExportExternalTexture(backendTexture, desiredLayout, fdInfo,
                                                          &fdInfo->semaphoreHandles);
        }
        default:
            return false;
    }
#else
    return false;
#endif  // DAWN_PLATFORM_IS(LINUX)
}

bool ExportVulkanImage(WGPUTexture texture, ExternalImageExportInfoVk* info) {
    return ExportVulkanImage(texture, VK_IMAGE_LAYOUT_UNDEFINED, info);
}

}  // namespace dawn::native::vulkan
