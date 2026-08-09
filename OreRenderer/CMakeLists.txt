################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "src/ImguiSections.h"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "include/OreRenderer/Shader.h"
    "include/OreRenderer/ShaderLibrary.h"
    "src/renderer/internal/IndexBuffer.h"
    "src/renderer/Input.h"
    "src/renderer/CameraController.h"
    "src/renderer/SaveImage.h"
    "src/renderer/internal/VertexArray.h"
    "src/renderer/internal/VertexBuffer.h"
    "src/renderer/internal/VertexBufferLayout.h"
    "src/renderer/Window.h"
    "src/scene/object/ObjectSelect.h"
    "src/scene/util/PlaneProjection.h"
    "src/scene/util/Triangulate.h"
    "src/mesh/ObjLoader.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "src/renderer/Camera.cpp"
    "src/renderer/internal/IndexBuffer.cpp"
    "src/renderer/Input.cpp"
    "src/renderer/CameraController.cpp"
    "src/renderer/Shader.cpp"
    "src/renderer/ShaderLibrary.cpp"
    "src/renderer/internal/VertexArray.cpp"
    "src/renderer/internal/VertexBuffer.cpp"
    "src/renderer/Window.cpp"
    "src/scene/Light.cpp"
    "src/scene/Material.cpp"
    "src/scene/Mesh.cpp"
    "src/scene/object/ObjectSelect.cpp"
    "src/scene/util/PlaneProjection.cpp"
    "src/scene/util/Triangulate.cpp"
    "src/stb_image_impl.cpp"
    "src/stb_image_write_impl.cpp"
    "src/renderer/Renderer.cpp"
    "src/mesh/ObjLoader.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(OreRenderer STATIC
    ${ALL_FILES}
)

target_include_directories(OreRenderer
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer"
)

add_executable(OreRendererDemo
    src/main.cpp
    src/ImguiSections.h
)

target_link_libraries(OreRendererDemo
    PRIVATE
        OreRenderer
)

use_props(OreRenderer
    "${CMAKE_CONFIGURATION_TYPES}"
    "${DEFAULT_CXX_PROPS}"
)
set(ROOT_NAMESPACE OreRenderer)

use_props(OreRendererDemo
    "${CMAKE_CONFIGURATION_TYPES}"
    "${DEFAULT_CXX_PROPS}"
)

set_target_properties(OreRendererDemo PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(OreRenderer PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
)

set_target_properties(OreRendererDemo PROPERTIES
    OUTPUT_DIRECTORY_DEBUG
        "${CMAKE_SOURCE_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/$/"
    OUTPUT_DIRECTORY_RELEASE
        "${CMAKE_SOURCE_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/$/"
)

set_target_properties(OreRenderer PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)

set_target_properties(OreRendererDemo PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)

set_target_properties(OreRendererDemo PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:OreRendererDemo>"
)

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(OreRenderer PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "UNICODE;"
        "_UNICODE"
    )
    target_compile_definitions(OreRendererDemo PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(OreRenderer PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "UNICODE;"
        "_UNICODE"
    )
    target_compile_definitions(OreRendererDemo PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(OreRenderer PRIVATE
        $<$<CONFIG:Release>:
            /Oi;
            /Gy
        >
        /permissive-;
        /std:c++17;
        /sdl;
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )


    target_compile_options(OreRendererDemo PRIVATE
        $<$<CONFIG:Release>:
            /Oi;
            /Gy
        >
        /permissive-;
        /std:c++17;
        /sdl;
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )

    target_link_options(OreRendererDemo PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /DEBUG;
        /SUBSYSTEM:CONSOLE
    )
endif()

################################################################################
# Dependencies
################################################################################
set(ORE_RENDERER_DEPENDENCIES
    "opengl32"
    OreRenderer::GLAD
    OreRenderer::ImGui
    OreRenderer::STB
    glfw
    glm::glm
)

set(ORE_RENDERER_DEMO_DEPENDENCIES
    OreRenderer
    OreRenderer::ImGui
    glfw
)

target_link_libraries(OreRenderer
    PRIVATE
        "${ORE_RENDERER_DEPENDENCIES}"
)

target_link_libraries(OreRendererDemo
    PRIVATE
        "${ORE_RENDERER_DEMO_DEPENDENCIES}"
)

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(OreRenderer PRIVATE
        "${CMAKE_SOURCE_DIR}/Dependencies/glfw-3.3.8.bin.WIN64/lib-vc2022"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(OreRenderer PRIVATE
        "${CMAKE_SOURCE_DIR}/Dependencies/glfw-3.3.8.bin.WIN32/lib-vc2022"
    )
endif()

################################################################################
# copy res
################################################################################
add_custom_command(TARGET OreRendererDemo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:OreRendererDemo>/res"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini"
        "$<TARGET_FILE_DIR:OreRendererDemo>/imgui.ini"

    VERBATIM
)