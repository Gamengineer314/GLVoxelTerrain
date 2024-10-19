# Optimized voxel terrain rendering with OpenGL

## Features :
- Indirect rendering (using glMultiDrawArraysIndirectCount) for minimal CPU-GPU interactions
- Packed mesh data (8 bytes per rectangle + 32 bytes per mesh)
- Frustrum culling in a compute shader
- Fast greedy mesher
- Slight random color variation for each voxel
- Basic flying camera controller

If you find any other improvements, please feel free to add them :)