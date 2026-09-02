# VoxTerGen Changelog

All notable changes to VoxTerGen will be documented in this file.  
The format follows Keep a Changelog and Semantic Versioning.

---

## [Unreleased]

### Added
-

### Changed
-

### Fixed
-

---

## [0.6.1] - 2 September 2026

### Added
- Added rendering of transparent blocks by splitting `ChunkMeshRenderPass` into `ChunkOpaqueRenderPass` and `ChunkTransparentRenderPass`.
- Added `ChunkWireframeRenderPass` for rendering the wireframe of the currently selected chunk.
- Added a screen-centered crosshair using `UIRenderPass`.
- Added a fog effect to hide chunk streaming artifacts.
- Added `EventDispatcher` for basic event distribution.

### Changed
- Split rendering classes into separate 2D and 3D rendering classes.
- Renamed and refactored numerous functions to improve consistency and code organization.

### Fixed
- Fixed chunk meshes not being rebuilt correctly when neighboring chunks are loaded or unloaded.

---

## [0.6.0] - 16 August 2026

### Added
- Added multithreaded chunk streaming and mesh generation.
- Added custom `ThreadPool`, `ThreadSafeQueue`, and `ThreadSafePriorityQueue` classes.
- Added a custom `Timer` class for profiling.
- Added priority-based chunk streaming based on distance from the observer.

### Changed
- Redesigned chunk meshing to use snapshots of neighboring chunks instead of querying the world during mesh generation.
- Added chunk mesh generation cancellation using `std::stop_token`.
- Improved chunk lifetime management during asynchronous mesh generation using `std::shared_ptr`.

---

## [0.5.3] - 24 July 2026

### Added
- Implemented frustum culling to avoid rendering chunks outside the camera's view.

---

## [0.5.2] - 22 July 2026

### Added
- Added support for loading cubemap textures from a texture atlas.
- Added skybox rendering.

---

## [0.5.1] - 17 July 2026

### Added
- Implemented Digital Differential Analyzer (DDA) raycasting for voxel traversal.
- Added block selection highlight rendered with a dedicated render pass.

---

## [0.5.0] - 30 June 2026

### Added
- Basic AABB collision detection and collision resolution against voxel terrain.
- Observer physics system.
- Walking with acceleration, friction and maximum movement speed.
- Gravity and jumping.
- Airborne and Grounded movement states.
- Noclip mode toggle.
- Fixed-timestep movement simulation.
- Horizontal movement independent of camera pitch.
- Velocity-based movement with collision clipping.

### Changed
- Refactored observer movement to use acceleration and velocity instead of direct position changes.
- Separated input gathering from movement simulation.
- Camera now follows observer movement after physics simulation.

---

## [0.4.0] - 2 May 2026

### Added
- Basic gradient and cellular noise algorithms to static lib project for procedural generation of terrain and caves.
- Fractal class for combining multiple octaves of noise.
- Vector field based walker for generating procedural caves.

---

## [0.3.0] - 3 April 2026

### Added
- Chunk streaming logic for loading and unloading chunks based on observer's position.
- Chunk event queue to handle communication between engine components.
- Improved rendering pipeline to support chunk streaming.

---

## [0.2.0] - 28 March 2026

### Added
- Basic single-threaded chunk mesh rendering pipeline.
- Chunk build queue to limit mesh generation per frame.
- Separation of CPU-side and GPU-side chunk data for mesh generation and upload.

---

## [0.1.0] - 4 March 2026

### Added
- Chunk and block management system for efficient world data storage and access.
- World initialization system for generating chunks and blocks.
- Greedy and naive mesh generation algorithms for chunk geometry.

---

## [0.0.0] - 17 December 2025

### Added
- Initial project setup with three components:
  - Engine application
  - Static algorithms library
  - CLI benchmarking application
- OpenGL and SDL2 integration with window creation and graphics context initialization.
- Basic fixed-timestep game loop handling input, state updates, and rendering.
- Empty scene with a movable camera.
- Input handling system for camera navigation.
