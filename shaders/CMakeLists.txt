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

    list(APPEND COMPILE_COMMAND ${SLANGC_EXECUTABLE})
    list(APPEND COMPILE_COMMAND ${SHADER_SOURCE})
    list(APPEND COMPILE_COMMAND "-target")
    list(APPEND COMPILE_COMMAND "spirv")
    list(APPEND COMPILE_COMMAND "-fvk-use-entrypoint-name")
    list(APPEND COMPILE_COMMAND "-matrix-layout-row-major")
    if (SHADER_DEBUG_INFO)
        list(APPEND COMPILE_COMMAND "-g")
    endif ()
    list(APPEND COMPILE_COMMAND "-o")
    list(APPEND COMPILE_COMMAND ${SHADER_OUTPUT_FILE})

    message(${COMPILE_COMMAND})

    add_custom_command(
            OUTPUT ${SHADER_OUTPUT_FILE}
            DEPENDS ${SHADER_SOURCE}
            COMMAND ${COMPILE_COMMAND}
            COMMENT "Compiling shader ${SHADER_SOURCE}"
    )

    list(APPEND SHADER_PRODUCTS ${SHADER_OUTPUT_FILE})
endforeach ()

add_custom_target(
        ${PROJECT_NAME}-shaders
        DEPENDS ${SHADER_PRODUCTS}
)
