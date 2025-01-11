set(
        SHADER_SOURCES
        triangle.slang
)

set(SHADER_COMMANDS)
set(SHADER_PRODUCTS)

cmake_path(APPEND VULKAN_SDK_BIN_DIR $ENV{VULKAN_SDK} "bin")
find_program(SLANGC_EXECUTABLE slangc PATHS VULKAN_SDK_BIN_DIR REQUIRED)

foreach (SHADER_SOURCE IN LISTS SHADER_SOURCES)
    cmake_path(ABSOLUTE_PATH SHADER_SOURCE NORMALIZE)
    cmake_path(GET SHADER_SOURCE FILENAME SHADER_NAME)

    cmake_path(APPEND SHADER_OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR} ${SHADER_NAME})
    cmake_path(REPLACE_EXTENSION SHADER_OUTPUT_FILE "spv")

    add_custom_command(
            OUTPUT ${SHADER_OUTPUT_FILE}
            DEPENDS ${SHADER_SOURCE}
            COMMAND ${SLANGC_EXECUTABLE} ${SHADER_SOURCE} -target spirv -fvk-use-entrypoint-name -matrix-layout-row-major -o ${SHADER_OUTPUT_FILE}
            COMMENT "Compiling shader ${SHADER_SOURCE}"
    )

    list(APPEND SHADER_PRODUCTS ${SHADER_OUTPUT_FILE})
endforeach ()

add_custom_target(
        ${PROJECT_NAME}-shaders
        DEPENDS ${SHADER_PRODUCTS}
)
