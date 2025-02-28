﻿#ifndef DIFFUSION_RENDERING_MEMORY_UTILS_H
#define DIFFUSION_RENDERING_MEMORY_UTILS_H

#include "platform/vulkan.h"

VkDeviceSize get_next_offset_for_alignment(VkDeviceSize offset, VkDeviceSize alignment);

VkDeviceSize get_previous_offset_for_alignment(VkDeviceSize offset, VkDeviceSize alignment);

#endif // DIFFUSION_RENDERING_MEMORY_UTILS_H
