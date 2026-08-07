# Ore Renderer

[![GitHub license](https://img.shields.io/github/license/Naereen/StrapDown.js.svg)](https://github.com/Ore-Software/OreRenderer/blob/main/LICENSE)
[![Build Status](https://github.com/Ore-Software/OreRenderer/actions/workflows/ci.yml/badge.svg)](https://github.com/Ore-Software/OreRenderer/actions/workflows/ci.yml)

Ore Renderer is a `C++17` and `OpenGL` implementation of an interactive mesh renderer/viewer for `.obj` files, with various various shading techniques.

## Features

- [x] Objects
  - [x] Custom .obj parser
- [x] Dynamic object selection
  - [x] Simple geometric objects
  - [x] Complex test objects
  - [x] Automatic rescaling
  - [x] Caching
- [x] Shading options
  - [x] Flat shading (Per-face normals)
  - [x] Smooth shading (Per-vertex normals)
  - [x] Mixed shading (Per-Corner normals)
- [x] Shader options
  - [x] [Normal](<https://en.wikipedia.org/wiki/Normal_(geometry)>) shading
  - [x] [Gourand shading](https://en.wikipedia.org/wiki/Gouraud_shading)
  - [x] [Phong shading](https://en.wikipedia.org/wiki/Phong_shading)
  - [x] [Blinn-Phong shading](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model)
  - [x] [Gooch shading](https://en.wikipedia.org/wiki/Gooch_shading)
  - [x] [Cel shading](https://en.wikipedia.org/wiki/Cel_shading)
  - [x] [Cook-Torrance shading](https://inst.eecs.berkeley.edu/~cs283/sp13/lectures/cookpaper.pdf)
- [x] Material controls
  - [x] Ambient
  - [x] Diffuse
  - [x] Specular
  - [x] Shine
- [x] Lighting controls
  - [x] Position
  - [x] Color
  - [x] Multiple lights
- [x] Render modes
  - [x] Mesh polygons
  - [x] Wireframe
  - [x] Point cloud
- [x] Interactivity
  - [x] Rotate model
  - [x] Move camera
  - [x] Rotate camera
- [x] Information
  - [x] Framerate counter
  - [x] Number of polygons in current mesh
- [x] Screenshot to PNG

## Image Gallery

<details>
<summary>Comparison of different shading techniques</summary>

![Sample armadillo rendering 1](gallery/Screenshot_2024-4-25_181621.png)

> Armadillo mesh, Blinn-Phong shader, not smoothened

![Sample armadillo rendering 2](gallery/Screenshot_2024-4-25_181732.png)

> Armadillo mesh, Cook-Torrance shader, not smoothened

![Sample armadillo rendering 3](gallery/Screenshot_2024-4-25_181641.png)

> Armadillo mesh, Cel shader, not smoothened

</details>

## Dependencies

- `GLFW`
- `GLM`
- `Dear ImGui`

## List of 3D Models Used

Here I try to list all sources wherever possible.

### Geometric models

| Model Name   | Source                                                                                     |
| ------------ | ------------------------------------------------------------------------------------------ |
| Crumpled     | [Keenan Crane](https://www.cs.cmu.edu/~kmcrane/Projects/ModelRepository/)                  |
| Cube         | Me                                                                                         |
| Double Torus | [Geometry Collective](https://github.com/GeometryCollective/ddg-exercises/tree/main/input) |
| Fandisk      | CAD part Pratt & Whitney/Hughes Hoppe                                                      |
| Icosahedron  | [John Burkardt](https://people.sc.fsu.edu/~jburkardt/data/obj/obj.html)                    |
| Octahedron   | [John Burkardt](https://people.sc.fsu.edu/~jburkardt/data/obj/obj.html)                    |
| Oloid        | [Keenan Crane](https://www.cs.cmu.edu/~kmcrane/Projects/ModelRepository/)                  |
| Sphere       | [Geometry Collective](https://github.com/GeometryCollective/ddg-exercises/tree/main/input) |
| Star         | Unknown                                                                                    |
| T-shape      | Me                                                                                         |
| Torus        | [Geometry Collective](https://github.com/GeometryCollective/ddg-exercises/tree/main/input) |
| Tubes        | Me                                                                                         |

### Test models

| Model Name   | Source                                                                                                 |
| ------------ | ------------------------------------------------------------------------------------------------------ |
| Ankylosaurus | Unknown                                                                                                |
| Armadillo    | [Stanford](http://graphics.stanford.edu/data/3Dscanrep/)                                               |
| Bob          | [Keenan Crane](https://www.cs.cmu.edu/~kmcrane/Projects/ModelRepository/)                              |
| Bunny        | [Stanford](http://graphics.stanford.edu/data/3Dscanrep/)                                               |
| Cow          | [Viewpoint Animation Engineering / Sun Microsystems](https://gfx.cs.princeton.edu/proj/sugcon/models/) |
| Cow head     | [Geometry Collective](https://github.com/GeometryCollective/ddg-exercises/tree/main/input)             |
| Face         | [Geometry Collective](https://github.com/GeometryCollective/ddg-exercises/tree/main/input)             |
| Gargoyle     | [Geometry Collective](https://github.com/GeometryCollective/ddg-exercises/tree/main/input)             |
| Kitten       | [Geometry Collective](https://github.com/GeometryCollective/ddg-exercises/tree/main/input)             |
| Shuttle      | [John Burkardt](https://people.sc.fsu.edu/~jburkardt/data/obj/obj.html)                                |
| Suzanne      | [Blender](https://www.blender.org/)                                                                    |
| Teapot       | [Martin Newell](https://users.cs.utah.edu/~dejohnso/models/teapot.html)                                |
| Teddy        | Unknown                                                                                                |

## Build

Ensure that python and jinja 2 are installed as GLAD requires them.
Create the solution file by running:

```{cmd}
mkdir build
cd build
cmake ..
```

Open the solution file however you like.
