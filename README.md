# Simple Computer Graphic Project - Geunyeong Kim


https://github.com/user-attachments/assets/84a4329f-d25c-4d98-877c-f44e4508f58a

This project is a simple 3D renderer written in C++. It supports wireframe rendering, rasterization, and basic ray tracing of 3D models.

## Features

- **Wireframe Rendering**: Draws 3D models as wireframes.
- **Rasterization**: Renders filled triangles using rasterization techniques.
- **Ray Tracing**: Implements basic ray tracing with lighting and shadows.
- **Camera Controls**: Move and rotate the camera using keyboard inputs.
- **Lighting Controls**: Adjust the position of the light source.
- **Model Loading**: Loads 3D models from `.obj` files.

## Dependencies

- **SDL2**: Used for window management and rendering.
- **GLM**: OpenGL Mathematics library for vector and matrix operations.

## How to Use

Compile the code with a C++ compiler and ensure all dependencies are properly linked.

### Controls

- **Camera Movement**:
  - `LEFT` / `RIGHT` arrows: Move camera left/right.
  - `UP` / `DOWN` arrows: Move camera up/down.
  - `4` / `5`: Move camera forward/backward.

- **Camera Rotation**:
  - `W` / `S`: Rotate camera around the X-axis.
  - `A` / `D`: Rotate camera around the Y-axis.

- **Orientation Adjustment**:
  - `T` / `G`: Adjust camera orientation around the X-axis.
  - `F` / `H`: Adjust camera orientation around the Y-axis.

- **Light Movement**:
  - `Z` / `X`: Move light along the X-axis.
  - `C` / `V`: Move light along the Y-axis.
  - `B` / `N`: Move light along the Z-axis.

- **Rendering Modes**:
  - `1`: Wireframe rendering.
  - `2`: Rasterization rendering.
  - `3`: Ray tracing rendering.

- **Miscellaneous**:
  - `L`: Reset camera to look at the origin.
  - `O`: Toggle automatic orbit rotation.
  - Mouse Click: Save the current frame as `output.ppm` and `output.bmp`.

## License

This project is open-source and available under the MIT License.

---
