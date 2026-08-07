include(FetchContent)

# Keep dependency configuration out of the normal build UI.
mark_as_advanced(FETCHCONTENT_BASE_DIR)

# ---------------------------------------------------------------------------
# GLFW
# ---------------------------------------------------------------------------

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.5.1
    GIT_SHALLOW    TRUE
)

# ---------------------------------------------------------------------------
# GLM
# ---------------------------------------------------------------------------

set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.3
    GIT_SHALLOW    TRUE
)

# ---------------------------------------------------------------------------
# GLAD 2
# ---------------------------------------------------------------------------

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG 658f48e72aee3c6582e80b05ac0f8787a64fe6bb
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR cmake
)

# ---------------------------------------------------------------------------
# Dear ImGui
#
# Dear ImGui does not provide the application-specific target we need, so
# obtain its sources here and define the target below.
# ---------------------------------------------------------------------------

FetchContent_Declare(
    imgui_source
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.9b
    GIT_SHALLOW    TRUE
)

# ---------------------------------------------------------------------------
# stb
# ---------------------------------------------------------------------------

FetchContent_Declare(
    stb_source
    GIT_REPOSITORY https://github.com/nothings/stb.git

    # Prefer a full commit SHA here rather than tracking master.
    # Replace this placeholder with the revision matching your migration.
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(
    glfw
    glm
    glad
    imgui_source
    stb_source
)

# Generate one GLAD target for the OpenGL profile used by the application.
#
# Change 4.6 to the minimum context actually requested by Ore renderer.
glad_add_library(
    ore_renderer_glad
    STATIC
    REPRODUCIBLE
    API gl:core=4.6
)

# Dear ImGui core plus the GLFW/OpenGL 3 backends.
add_library(ore_renderer_imgui STATIC)

target_sources(
    ore_renderer_imgui
    PRIVATE
        "${imgui_source_SOURCE_DIR}/imgui.cpp"
        "${imgui_source_SOURCE_DIR}/imgui_draw.cpp"
        "${imgui_source_SOURCE_DIR}/imgui_tables.cpp"
        "${imgui_source_SOURCE_DIR}/imgui_widgets.cpp"
        "${imgui_source_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
        "${imgui_source_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

target_include_directories(
    ore_renderer_imgui
    SYSTEM
    PUBLIC
        "${imgui_source_SOURCE_DIR}"
        "${imgui_source_SOURCE_DIR}/backends"
)

target_link_libraries(
    ore_renderer_imgui
    PUBLIC
        glfw
        ore_renderer_glad
)

# Avoid GLFW including the platform OpenGL headers before GLAD.
target_compile_definitions(
    ore_renderer_imgui
    PUBLIC
        GLFW_INCLUDE_NONE
)

add_library(ore_renderer_stb INTERFACE)

target_include_directories(
    ore_renderer_stb
    SYSTEM
    INTERFACE
        "${stb_source_SOURCE_DIR}"
)

# Stable project-owned aliases. The application does not need to know how
# each dependency was obtained.
add_library(OreRenderer::GLAD  ALIAS ore_renderer_glad)
add_library(OreRenderer::ImGui ALIAS ore_renderer_imgui)
add_library(OreRenderer::STB   ALIAS ore_renderer_stb)
