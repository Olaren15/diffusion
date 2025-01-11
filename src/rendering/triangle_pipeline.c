#include "rendering/triangle_pipeline.h"

#include "core/file.h"
#include "rendering/model/vertex.h"

bool create_shader_module(
  const char* shader_path, const render_device_t* device, VkShaderModule* shader_module);

bool triangle_pipeline_create(
  triangle_pipeline_t* self,
  const render_device_t* device,
  VkFormat color_attachment_format,
  VkDescriptorSetLayout scene_descriptor_set_layout) {

    VkPipelineLayoutCreateInfo pipeline_layout_create_infos = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .setLayoutCount = 1,
      .pSetLayouts = &scene_descriptor_set_layout,
      .pushConstantRangeCount = 0,
      .pPushConstantRanges = NULL,
    };

    if (
      vkCreatePipelineLayout(device->vk_device, &pipeline_layout_create_infos, NULL, &self->layout)
      != VK_SUCCESS) {
        return false;
    }

    VkShaderModule triangle_shader;
    if (!create_shader_module("shaders/triangle.spv", device, &triangle_shader)) {
        return false;
    }

    VkPipelineShaderStageCreateInfo vertex_shader_stage_info = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_VERTEX_BIT,
      .module = triangle_shader,
      .pName = "vertex_main",
    };

    VkPipelineShaderStageCreateInfo fragment_shader_stage_info = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
      .module = triangle_shader,
      .pName = "fragment_main",
    };

    VkDynamicState dynamic_states[2] = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_SCISSOR,
    };

    VkPipelineDynamicStateCreateInfo dynamic_state = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
      .dynamicStateCount = 2,
      .pDynamicStates = dynamic_states,
    };

    VkPipelineShaderStageCreateInfo shader_stages[] = {
      vertex_shader_stage_info, fragment_shader_stage_info};

    VkVertexInputBindingDescription vertex_input_binding_description = {
      .binding = 0,
      .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
      .stride = sizeof(vertex_t),
    };

    VkVertexInputAttributeDescription vertex_input_attribute_descriptions[] = {
      {.binding = 0,
       .location = 0,
       .format = VK_FORMAT_R32G32B32_SFLOAT,
       .offset = offsetof(vertex_t, position)},
      {.binding = 0,
       .location = 1,
       .format = VK_FORMAT_R32G32B32A32_SFLOAT,
       .offset = offsetof(vertex_t, color)   }
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_state = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .vertexBindingDescriptionCount = 1,
      .pVertexBindingDescriptions = &vertex_input_binding_description,
      .vertexAttributeDescriptionCount = 2,
      .pVertexAttributeDescriptions = vertex_input_attribute_descriptions,
    };

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
      .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      .primitiveRestartEnable = VK_FALSE,
    };

    VkPipelineViewportStateCreateInfo viewport_state = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .viewportCount = 1,
      .scissorCount = 1,
    };

    VkPipelineRasterizationStateCreateInfo rasterization_state = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
      .depthClampEnable = VK_FALSE,
      .rasterizerDiscardEnable = VK_FALSE,
      .polygonMode = VK_POLYGON_MODE_FILL,
      .lineWidth = 1.0f,
      .cullMode = VK_CULL_MODE_BACK_BIT,
      .frontFace = VK_FRONT_FACE_CLOCKWISE,
      .depthBiasEnable = VK_FALSE,
    };

    VkPipelineMultisampleStateCreateInfo multisample_state = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
      .sampleShadingEnable = VK_FALSE,
      .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
      .minSampleShading = 1.0f,
      .pSampleMask = NULL,
      .alphaToCoverageEnable = VK_FALSE,
      .alphaToOneEnable = VK_FALSE,
    };

    VkPipelineColorBlendAttachmentState color_blend_attachment_state = {
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
                        | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      .blendEnable = VK_FALSE,
    };

    VkPipelineColorBlendStateCreateInfo color_blend_state = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
      .logicOpEnable = VK_FALSE,
      .attachmentCount = 1,
      .pAttachments = &color_blend_attachment_state,
    };

    VkPipelineRenderingCreateInfo pipeline_rendering = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
      .colorAttachmentCount = 1,
      .pColorAttachmentFormats = &color_attachment_format,
    };

    VkGraphicsPipelineCreateInfo pipeline_create_info = {
      .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      .stageCount = 2,
      .pStages = shader_stages,
      .pVertexInputState = &vertex_input_state,
      .pInputAssemblyState = &input_assembly_state,
      .pTessellationState = NULL,
      .pViewportState = &viewport_state,
      .pRasterizationState = &rasterization_state,
      .pMultisampleState = &multisample_state,
      .pDepthStencilState = NULL,
      .pColorBlendState = &color_blend_state,
      .pDynamicState = &dynamic_state,
      .layout = self->layout,
      .renderPass = NULL,
      .pNext = &pipeline_rendering,
    };

    if (
      vkCreateGraphicsPipelines(
        device->vk_device, VK_NULL_HANDLE, 1, &pipeline_create_info, NULL, &self->vk_pipeline)
      != VK_SUCCESS) {
        return false;
    }

    vkDestroyShaderModule(device->vk_device, triangle_shader, NULL);

    return true;
}

void triangle_pipeline_destroy(triangle_pipeline_t* self, const render_device_t* device) {
    vkDestroyPipeline(device->vk_device, self->vk_pipeline, NULL);
    vkDestroyPipelineLayout(device->vk_device, self->layout, NULL);
}

bool create_shader_module(
  const char* shader_path, const render_device_t* device, VkShaderModule* shader_module) {
    dynamic_array_t shader_bytes = read_file(shader_path);
    if (shader_bytes.data == NULL) {
        return false;
    }

    VkShaderModuleCreateInfo vertex_shader_module_create_info = {
      .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
      .codeSize = shader_bytes.element_count,
      .pCode = (const uint32_t*)shader_bytes.data,
    };

    if (
      vkCreateShaderModule(
        device->vk_device, &vertex_shader_module_create_info, NULL, shader_module)
      != VK_SUCCESS) {
        dynamic_array_free(&shader_bytes);
        return false;
    }

    dynamic_array_free(&shader_bytes);
    return true;
}
