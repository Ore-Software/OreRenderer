set(PROJECT_NAME OreRenderer)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "src/ImguiSections.h"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "src/renderer/Camera.h"
    "src/renderer/IndexBuffer.h"
    "src/renderer/Input.h"
    "src/renderer/SaveImage.h"
    "src/renderer/Shader.h"
    "src/renderer/VertexArray.h"
    "src/renderer/VertexBuffer.h"
    "src/renderer/VertexBufferLayout.h"
    "src/renderer/Window.h"
    "src/scene/Light.h"
    "src/scene/Material.h"
    "src/scene/Mesh.h"
    "src/scene/object/Object.h"
    "src/scene/object/ObjectSelect.h"
    "src/scene/surface/Surface.h"
    "src/scene/util/OrderVertices.h"
    "src/scene/util/PlaneProjection.h"
    "src/scene/util/Triangulate.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "src/main.cpp"
    "src/renderer/Camera.cpp"
    "src/renderer/IndexBuffer.cpp"
    "src/renderer/Input.cpp"
    "src/renderer/Shader.cpp"
    "src/renderer/VertexArray.cpp"
    "src/renderer/VertexBuffer.cpp"
    "src/renderer/Window.cpp"
    "src/scene/Light.cpp"
    "src/scene/Material.cpp"
    "src/scene/Mesh.cpp"
    "src/scene/object/Object.cpp"
    "src/scene/object/ObjectSelect.cpp"
    "src/scene/surface/Surface.cpp"
    "src/scene/util/OrderVertices.cpp"
    "src/scene/util/PlaneProjection.cpp"
    "src/scene/util/Triangulate.cpp"
    "src/stb_image_impl.cpp"
    "src/stb_image_write_impl.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE OreRenderer)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
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
    target_compile_definitions(${PROJECT_NAME} PRIVATE
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
    target_compile_options(${PROJECT_NAME} PRIVATE
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

    target_link_options(${PROJECT_NAME} PRIVATE
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
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "opengl32"
    OreRenderer::GLAD
    OreRenderer::ImGui
    OreRenderer::STB
    glfw
    glm::glm
)
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/Dependencies/glfw-3.3.8.bin.WIN64/lib-vc2022"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/Dependencies/glfw-3.3.8.bin.WIN32/lib-vc2022"
    )
endif()

################################################################################
# copy res
################################################################################
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/imgui.ini"

    VERBATIM
)