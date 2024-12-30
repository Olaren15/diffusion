#include "rendering/vk_utils.h"

const VkImageSubresourceRange subresource_range_color_all_mips = {
  .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
  .baseMipLevel = 0,
  .levelCount = VK_REMAINING_MIP_LEVELS,
  .baseArrayLayer = 0,
  .layerCount = VK_REMAINING_MIP_LEVELS,
};

const VkClearColorValue clear_color_white_unorm = {
  .float32 = {1.0f, 1.0f, 1.0f, 1.0f}
};
