#include "rendering/vk_utils.h"

const VkImageSubresourceRange subresource_range_color_all_mips = {
  .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
  .baseMipLevel = 0,
  .levelCount = VK_REMAINING_MIP_LEVELS,
  .baseArrayLayer = 0,
  .layerCount = VK_REMAINING_MIP_LEVELS,
};

const VkClearColorValue clear_color_black_unorm = {
  .float32 = {0.0f, 0.0f, 0.0f, 1.0f}
};
