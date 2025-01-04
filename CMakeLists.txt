cmake_minimum_required(VERSION 3.20)

# Project details
project(diffusion VERSION 0.1.0 LANGUAGES C)
set(PROJECT_AUTHOR "Catherine Gilbert")
set(PROJECT_IDENTIFIER "dev.olaren.diffusion")
set(PROJECT_LICENSE_SHORT "Copyright (C) 2024  Catherine Gilbert\\n\\nThis program is free software: you can redistribute it and/or modify\\nit under the terms of the GNU General Public License as published by\\nthe Free Software Foundation, either version 3 of the License, or\\n(at your option) any later version.\\n\\nThis program is distributed in the hope that it will be useful,\\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\\nGNU General Public License for more details.\\n\\nYou should have received a copy of the GNU General Public License\\nalong with this program.  If not, see <https://www.gnu.org/licenses/>.")

# Generate compile_commands.json for clang based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
include(cmake/compile_options.cmake)

# Lib
configure_file(src/project.c.in ${CMAKE_BINARY_DIR}/generated/src/project.c)
add_library(
        ${PROJECT_NAME}
        STATIC
        ${CMAKE_BINARY_DIR}/generated/src/project.c
        src/engine.c

        src/core/dynamic_array.c
        src/core/file.c
        src/core/math.c
        src/core/slice.c

        src/platform/win32/window.c
        src/platform/vulkan.c
        src/platform/win32/win32_utils.c

        src/rendering/frame.c
        src/rendering/frame_sync.c
        src/rendering/memory/gpu_allocated_buffer.c
        src/rendering/memory/gpu_allocation.c
        src/rendering/memory/gpu_memory.c
        src/rendering/memory/rebar_gpu_allocator.c
        src/rendering/model/gpu_mesh.c
        src/rendering/render_context.c
        src/rendering/render_device.c
        src/rendering/swapchain.c
        src/rendering/triangle_pipeline.c
        src/rendering/vk_utils.c
)
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Headers)

# Shaders
add_subdirectory(shaders)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-shaders)

# Executable
add_executable(${PROJECT_NAME}-bin src/main.c)
target_link_libraries(${PROJECT_NAME}-bin PUBLIC ${PROJECT_NAME})
set_compile_options(${PROJECT_NAME}-bin)
set_target_properties(${PROJECT_NAME}-bin PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if (BUILD_TESTING)
        add_subdirectory(test)
    endif ()
endif ()
